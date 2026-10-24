#include "terra/assets/asset_manager.h"

namespace terra {

AssetManager::AssetManager() : m_context(nullptr) {}
AssetManager::~AssetManager() { clear(); }

void AssetManager::setContext(VulkanContext* context) { m_context = context; }

Texture* AssetManager::loadTexture(const std::string& name) {
    auto it = m_textures.find(name);
    if (it != m_textures.end()) return &it->second;

    Image img;
    if (!loadImageFromFile(name, img)) {
        TERRA_LOG_WARN(("failed to load texture: " + name).c_str());
        return nullptr;
    }

    Texture tex;
    if (!tex.createFromPixels(*m_context, img.pixels.data(), img.width,
                              img.height, img.channels)) {
        TERRA_LOG_ERROR(("failed to upload texture: " + name).c_str());
        return nullptr;
    }
    m_textures.emplace(name, std::move(tex));
    return &m_textures.at(name);
}

Texture* AssetManager::createCheckerboard(const std::string& name, int width,
                                          int height) {
    auto it = m_textures.find(name);
    if (it != m_textures.end()) return &it->second;

    Image img = generateCheckerboard(width, height);
    Texture tex;
    if (!tex.createFromPixels(*m_context, img.pixels.data(), img.width,
                              img.height, img.channels)) {
        TERRA_LOG_ERROR(("failed to upload checkerboard: " + name).c_str());
        return nullptr;
    }
    m_textures.emplace(name, std::move(tex));
    return &m_textures.at(name);
}

Texture* AssetManager::getTexture(const std::string& name) const {
    auto it = m_textures.find(name);
    return it != m_textures.end() ? &it->second : nullptr;
}

Image* AssetManager::getImage(const std::string& name) {
    auto it = m_images.find(name);
    return it != m_images.end() ? &it->second : nullptr;
}

void AssetManager::clear() {
    m_textures.clear();
    m_images.clear();
}

} // namespace terra
