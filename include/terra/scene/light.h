#pragma once

#include <string>

#include "terra/core/math.h"

namespace terra {

struct Light {
    enum Type { Directional, Point };

    Type type;
    vec3 color;
    float intensity;
    vec3 position;
    vec3 direction;

    Light();
};

} // namespace terra
