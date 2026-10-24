#include "terra/assets/image.h"

#include <cstring>
#include <iostream>

namespace terra {

Image generateCheckerboard(int width, int height, int blockSize) {
    Image img;
    img.width = width;
    img.height = height;
    img.channels = 4;
    img.pixels.resize((size_t)width * (size_t)height * 4, 255);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool light = ((x / blockSize) + (y / blockSize)) % 2 == 0;
            size_t idx = ((size_t)y * (size_t)width + (size_t)x) * 4;
            uint8_t v = light ? 240 : 60;
            img.pixels[idx + 0] = v;
            img.pixels[idx + 1] = v;
            img.pixels[idx + 2] = v;
            img.pixels[idx + 3] = 255;
        }
    }
    return img;
}

bool loadImageFromFile(const std::string& path, Image& out) {
    (void)path;
    (void)out;
    TERRA_LOG_WARN("image file loading not implemented; "
                   "vendor stb_image.h to enable");
    return false;
}

} // namespace terra
