#include "terra/renderer/command_buffer.h"

#include <iostream>

namespace terra {

CommandBuffer::CommandBuffer()
    : m_context(nullptr)
    , m_commandBuffer(VK_NULL_HANDLE)
    , m_valid(false) {}

CommandBuffer::~CommandBuffer() { free(); }

bool CommandBuffer::allocate(VulkanContext& context, VkCommandBufferLevel level) {
    m_context = &context;
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = level;
    allocInfo.commandPool = context.commandPool();
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(context.device(), &allocInfo,
                                 &m_commandBuffer) != VK_SUCCESS) {
        std::cerr << "[renderer] failed to allocate command buffer" << std::endl;
        return false;
    }
    m_valid = true;
    return true;
}

void CommandBuffer::free() {
    if (m_commandBuffer == VK_NULL_HANDLE || !m_context) return;
    vkFreeCommandBuffers(m_context->device(), m_context->commandPool(), 1,
                         &m_commandBuffer);
    m_commandBuffer = VK_NULL_HANDLE;
    m_valid = false;
}

bool CommandBuffer::begin(VkCommandBufferUsageFlags flags) {
    if (!m_valid) return false;
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;
    if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS) {
        return false;
    }
    return true;
}

bool CommandBuffer::end() {
    if (!m_valid) return false;
    if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS) return false;
    return true;
}

void CommandBuffer::reset() {
    if (m_commandBuffer != VK_NULL_HANDLE && m_context) {
        vkResetCommandBuffer(m_commandBuffer, 0);
    }
}

} // namespace terra
