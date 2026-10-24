#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "terra/core/math.h"

namespace terra {

// A single vertex for conventional mesh rendering.
struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv; // kept as two floats below for C++14 aggregate simplicity
    float u, v;

    Vertex() : position(0,0,0), normal(0,0,0), u(0), v(0) {}
    Vertex(vec3 p, vec3 n, float uu, float vv)
        : position(p), normal(n), u(uu), v(vv) {}
};

// A CPU-side mesh ready to be uploaded to GPU buffers.
class Mesh {
public:
    Mesh();
    ~Mesh();

    // Upload vertex/index data to device-local buffers. Must be called after
    // the VulkanContext is initialized and before drawing.
    bool upload(VulkanContext& context,
                const std::vector<Vertex>& vertices,
                const std::vector<uint32_t>& indices);

    void shutdown();

    void draw(VkCommandBuffer cmdBuffer) const;

    VkBuffer vertexBuffer() const { return m_vertexBuffer; }
    VkBuffer indexBuffer() const { return m_indexBuffer; }
    uint32_t indexCount() const { return m_indexCount; }

    bool isValid() const { return m_valid; }

private:
    VkDevice m_device;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexMemory;
    uint32_t m_indexCount;
    bool m_valid;
};

} // namespace terra
