#include "demo.h"

#include <iostream>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    terra::Demo demo;
    if (!demo.init()) {
        std::cerr << "Failed to initialize TerraCore demo" << std::endl;
        return 1;
    }
    return demo.run();
}
