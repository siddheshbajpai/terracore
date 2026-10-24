#include "terra/renderer/mesh.h"

#include <iostream>
#include <cstring>

namespace terra {

Mesh::Mesh()
    : m_device(VK_NULL_HANDLE)
    , m_vertexBuffer(VK_NULL_HANDLE)
    , m_vertexMemory(VK_NULL_HANDLE)
    , m_indexBuffer(VK_NULL_HANDLE)
    , m_indexMemory(VK_NULL_HANDLE)
    , m_indexCount(0)
    , m_valid(false) {}

Mesh::~Mesh() { shutdown(); }

namespace {

bool createBuffer(VkDevice device, VkPhysicalDevice physicalDevice,
                  VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer& buffer,
                  VkDeviceMemory& memory) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        return false;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    uint32_t typeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        if ((memRequirements.memoryTypeBits & (1u << i)) &&
            ((memProperties.memoryTypes[i].propertyFlags & properties) ==
             properties)) {
            typeIndex = i;
            break;
        }
    }
    if (typeIndex == UINT32_MAX) return false;

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = typeIndex;

    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        vkDestroyBuffer(device, buffer, nullptr);
        return false;
    }

    vkBindBufferMemory(device, buffer, memory, 0);
    return true;
}

} // namespace

bool Mesh::upload(VulkanContext& context,
                  const std::vector<Vertex>& vertices,
                  const std::vector<uint32_t>& indices) {
    m_device = context.device();
    shutdown();

    VkDeviceSize vertexSize = sizeof(Vertex) * vertices.size();
    VkDeviceSize indexSize = sizeof(uint32_t) * indices.size();
    m_indexCount = (uint32_t)indices.size();

    // Staging buffers (host-visible) then device-local copies.
    VkBuffer vertStaging;
    VkDeviceMemory vertStagingMem;
    if (!createBuffer(m_device, context.physicalDevice(), vertexSize,
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      vertStaging, vertStagingMem)) {
        return false;
    }

    VkBuffer indexStaging;
    VkDeviceMemory indexStagingMem;
    if (!createBuffer(m_device, context.physicalDevice(), indexSize,
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      indexStaging, indexStagingMem)) {
        vkDestroyBuffer(m_device, vertStaging, nullptr);
        vkFreeMemory(m_device, vertStagingMem, nullptr);
        return false;
    }

    // Map and copy.
    void* data;
    vkMapMemory(m_device, vertStagingMem, 0, vertexSize, 0, &data);
    std::memcpy(data, vertices.data(), (size_t)vertexSize);
    vkUnmapMemory(m_device, vertStagingMem);

    vkMapMemory(m_device, indexStagingMem, 0, indexSize, 0, &data);
    std::memcpy(data, indices.data(), (size_t)indexSize);
    vkUnmapMemory(m_device, indexStagingMem);

    if (!createBuffer(m_device, context.physicalDevice(), vertexSize,
                      VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                      m_vertexBuffer, m_vertexMemory)) {
        return false;
    }

    if (!createBuffer(m_device, context.physicalDevice(), indexSize,
                      VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                          VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                      m_indexBuffer, m_indexMemory)) {
        return false;
    }

    // One-time submit to copy staging -> device local.
    VkResult res = context.oneTimeSubmit([&](VkCommandBuffer cmd) {
        VkBufferCopy region = {};
        region.size = vertexSize;
        vkCmdCopyBuffer(cmd, vertStaging, m_vertexBuffer, 1, &region);
        VkBufferCopy idxRegion = {};
        idxRegion.size = indexSize;
        vkCmdCopyBuffer(cmd, indexStaging, m_indexBuffer, 1, &idxRegion);
        return cmd;
    });

    // Cleanup staging.
    vkDestroyBuffer(m_device, vertStaging, nullptr);
    vkFreeMemory(m_device, vertStagingMem, nullptr);
    vkDestroyBuffer(m_device, indexStaging, nullptr);
    vkFreeMemory(m_device, indexStagingMem, nullptr);

    if (res != VK_SUCCESS) return false;
    m_valid = true;
    return true;
}

void Mesh::shutdown() {
    if (m_device == VK_NULL_HANDLE) return;
    vkDeviceWaitIdle(m_device);
    if (m_indexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device, m_indexBuffer, nullptr);
        m_indexBuffer = VK_NULL_HANDLE;
    }
    if (m_indexMemory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, m_indexMemory, nullptr);
        m_indexMemory = VK_NULL_HANDLE;
    }
    if (m_vertexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
        m_vertexBuffer = VK_NULL_HANDLE;
    }
    if (m_vertexMemory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, m_vertexMemory, nullptr);
        m_vertexMemory = VK_NULL_HANDLE;
    }
    m_indexCount = 0;
    m_valid = false;
}

void Mesh::draw(VkCommandBuffer cmdBuffer) const {
    if (!m_valid) return;
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &m_vertexBuffer, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer, m_indexCount, 1, 0, 0, 0);
}

} // namespace terra
