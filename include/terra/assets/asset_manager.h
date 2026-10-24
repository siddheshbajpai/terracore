#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "terra/assets/image.h"
#include "terra/core/log.h"
#include "terra/renderer/texture.h"

namespace terra {

// Simple asset manager for the MVP. Owns loaded images and textures keyed by
// name. Thread-unsafe by design; synchronization can be added later.
class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    // Set the Vulkan context needed to upload textures to the GPU.
    void setContext(VulkanContext* context);

    // Load/create a texture by name. Returns nullptr on failure.
    Texture* loadTexture(const std::string& name);

    // Create a checkerboard texture by name.
    Texture* createCheckerboard(const std::string& name,
                                int width = 256, int height = 256);

    Texture* getTexture(const std::string& name) const;
    Image* getImage(const std::string& name);

    void clear();

private:
    VulkanContext* m_context;
    std::unordered_map<std::string, Image> m_images;
    std::unordered_map<std::string, Texture> m_textures;
};

} // namespace terra
