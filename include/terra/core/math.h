#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace terra {

// ---------------------------------------------------------------------------
// Compact C++14-compatible math library (kept header-only so the engine core
// stays dependency-free and reproducible).
// ---------------------------------------------------------------------------

#ifndef TERRA_PI
#    define TERRA_PI 3.14159265358979323846f
#endif

inline float radians(float degrees) { return degrees * (TERRA_PI / 180.0f); }
inline float degrees(float radians) { return radians * (180.0f / TERRA_PI); }

struct vec3 {
    float x, y, z;

    vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float v) : x(v), y(v), z(v) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 operator+(const vec3& o) const { return vec3(x + o.x, y + o.y, z + o.z); }
    vec3 operator-(const vec3& o) const { return vec3(x - o.x, y - o.y, z - o.z); }
    vec3 operator*(const vec3& o) const { return vec3(x * o.x, y * o.y, z * o.z); }
    vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
    vec3 operator/(float s) const { return vec3(x / s, y / s, z / s); }

    vec3& operator+=(const vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
    vec3& operator-=(const vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }

    bool operator==(const vec3& o) const {
        return x == o.x && y == o.y && z == o.z;
    }
    bool operator!=(const vec3& o) const { return !(*this == o); }

    float length() const { return sqrtf(x * x + y * y + z * z); }
    float lengthSquared() const { return x * x + y * y + z * z; }
    vec3 normalized() const {
        float l = length();
        if (l > 1e-8f) return vec3(x / l, y / l, z / l);
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float dot(const vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    vec3 cross(const vec3& o) const {
        return vec3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
    }
};

inline vec3 operator*(float s, const vec3& v) { return v * s; }

inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

struct vec4 {
    float x, y, z, w;

    vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    vec4(float v) : x(v), y(v), z(v), w(v) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    explicit vec4(const vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
};

struct mat4 {
    float m[16]; // column-major

    mat4() { identity(); }

    static mat4 identity() {
        mat4 r;
        for (int i = 0; i < 16; ++i) r.m[i] = 0.0f;
        r.m[0] = 1.0f; r.m[5] = 1.0f; r.m[10] = 1.0f; r.m[15] = 1.0f;
        return r;
    }

    void setIdentity() { *this = identity(); }

    float* operator[](int col) { return &m[col * 4]; }
    const float* operator[](int col) const { return &m[col * 4]; }

    bool operator==(const mat4& o) const {
        for (int i = 0; i < 16; ++i) {
            if (m[i] != o.m[i]) return false;
        }
        return true;
    }
    bool operator!=(const mat4& o) const { return !(*this == o); }
};

inline mat4 operator*(const mat4& a, const mat4& b) {
    mat4 r;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            r.m[col * 4 + row] = sum;
        }
    }
    return r;
}

inline vec3 operator*(const mat4& m, const vec3& v) {
    return vec3(
        m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12],
        m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13],
        m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14]
    );
}

inline mat4 translate(const vec3& t) {
    mat4 r = mat4::identity();
    r.m[12] = t.x; r.m[13] = t.y; r.m[14] = t.z;
    return r;
}

inline mat4 scale(const vec3& s) {
    mat4 r = mat4::identity();
    r.m[0] = s.x; r.m[5] = s.y; r.m[10] = s.z;
    return r;
}

inline mat4 perspective(float fovY, float aspect, float zNear, float zFar) {
    mat4 r;
    for (int i = 0; i < 16; ++i) r.m[i] = 0.0f;
    float f = 1.0f / tanf(radians(fovY) * 0.5f);
    float nf = 1.0f / (zNear - zFar);
    r.m[0]  = f / aspect;
    r.m[5]  = f;
    r.m[10] = (zFar + zNear) * nf;
    r.m[11] = -1.0f;
    r.m[14] = (2.0f * zFar * zNear) * nf;
    return r;
}

inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
    vec3 f = (center - eye).normalized();
    vec3 s = f.cross(up.normalized());
    vec3 u = s.cross(f);
    mat4 r = mat4::identity();
    r.m[0] = s.x;   r.m[4] = s.y;   r.m[8]  = s.z;
    r.m[1] = u.x;   r.m[5] = u.y;   r.m[9]  = u.z;
    r.m[2] = -f.x;  r.m[6] = -f.y;  r.m[10] = -f.z;
    r.m[12] = -s.dot(eye);
    r.m[13] = -u.dot(eye);
    r.m[14] = f.dot(eye);
    return r;
}

inline mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
    mat4 r;
    for (int i = 0; i < 16; ++i) r.m[i] = 0.0f;
    r.m[0] = 2.0f / (right - left);
    r.m[5] = 2.0f / (top - bottom);
    r.m[10] = -2.0f / (zFar - zNear);
    r.m[12] = -(right + left) / (right - left);
    r.m[13] = -(top + bottom) / (top - bottom);
    r.m[14] = -(zFar + zNear) / (zFar - zNear);
    r.m[15] = 1.0f;
    return r;
}

} // namespace terra
