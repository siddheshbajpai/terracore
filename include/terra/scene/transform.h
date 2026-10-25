#pragma once

#include "terra/core/math.h"

namespace terra {

// Transform component: position, rotation (euler degrees) and scale.
// Computes the local model matrix. No parent/world handling here; that lives
// in Entity so Transform stays a pure data component.
struct Transform {
    vec3 position;
    vec3 rotation; // euler angles in degrees (pitch, yaw, roll)
    vec3 scale;

    Transform();

    // Local model matrix (no parent).
    mat4 localMatrix() const;

    // Normal matrix = inverse-transpose of the upper 3x3 of localMatrix.
    mat4 normalMatrix() const;
};

} // namespace terra
