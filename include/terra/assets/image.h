#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace terra {

// A CPU-side RGBA8 image container. Kept dependency-free so the engine core
// remains reproducible without external image libraries (stb_image can be
// vendored under third_party/ to enable real file loading).
struct Image {
    int width = 0;
    int height = 0;
    int channels = 4; // RGBA8
    std::vector<uint8_t> pixels; // size = width * height * channels
};

// Generate a checkerboard texture (useful for the demo without any file).
Image generateCheckerboard(int width, int height, int blockSize = 8);

// Load an image from file. Returns false unless an image loader (e.g. a
// vendored stb_image) has been wired into asset_manager.
bool loadImageFromFile(const std::string& path, Image& out);

} // namespace terra
