#include "terra/scene/transform.h"

#include <cmath>

namespace terra {

Transform::Transform()
    : position(0.0f, 0.0f, 0.0f)
    , rotation(0.0f, 0.0f, 0.0f)
    , scale(1.0f, 1.0f, 1.0f) {}

mat4 Transform::localMatrix() const {
    mat4 t = translate(position);

    // Euler rotation (pitch=x, yaw=y, roll=z) in degrees -> radians.
    float cx = cosf(radians(rotation.x));
    float sx = sinf(radians(rotation.x));
    float cy = cosf(radians(rotation.y));
    float sy = sinf(radians(rotation.y));
    float cz = cosf(radians(rotation.z));
    float sz = sinf(radians(rotation.z));

    mat4 rx = mat4::identity();
    rx.m[5] = cx; rx.m[6] = sx; rx.m[9] = -sx; rx.m[10] = cx;
    mat4 ry = mat4::identity();
    ry.m[0] = cy; ry.m[2] = -sy; ry.m[8] = sy; ry.m[10] = cy;
    mat4 rz = mat4::identity();
    rz.m[0] = cz; rz.m[1] = sz; rz.m[4] = -sz; rz.m[5] = cz;

    mat4 r = rz * ry * rx;
    mat4 s = mat4::identity();
    s.m[0] = scale.x;
    s.m[5] = scale.y;
    s.m[10] = scale.z;

    return t * r * s;
}

mat4 Transform::normalMatrix() const {
    mat4 m = localMatrix();
    mat4 n = mat4::identity();
    // Upper-left 3x3 inverse-transpose approximation (no non-uniform scale
    // assumed for MVP).
    n.m[0] = m.m[0]; n.m[1] = m.m[4]; n.m[2]  = m.m[8];
    n.m[4] = m.m[1]; n.m[5] = m.m[5]; n.m[6]  = m.m[9];
    n.m[8] = m.m[2]; n.m[9] = m.m[6]; n.m[10] = m.m[10];
    return n;
}

} // namespace terra
