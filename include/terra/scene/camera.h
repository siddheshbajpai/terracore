#pragma once

#include "terra/core/math.h"

namespace terra {

class Camera {
public:
    enum Type { Perspective, Orthographic };

    Camera();

    void setPerspective(float fovY, float aspect, float zNear, float zFar);
    void setOrthographic(float left, float right, float bottom, float top,
                         float zNear, float zFar);
    void setAspect(float aspect);

    // Build the view matrix from eye/center/up.
    void setLookAt(const vec3& eye, const vec3& center, const vec3& up);

    void setPosition(const vec3& pos) { m_position = pos; m_dirty = true; }
    void setRotation(const vec3& rot) { m_rotation = rot; m_dirty = true; }

    const vec3& position() const { return m_position; }
    const vec3& rotation() const { return m_rotation; }

    const mat4& viewMatrix() const { return m_view; }
    const mat4& projectionMatrix() const { return m_projection; }
    const mat4& viewProjection() const { return m_viewProj; }

    Type type() const { return m_type; }

    // Recalculate view + projection matrices if anything changed.
    void updateMatrices() const;

private:
    Type m_type;
    float m_fovY;
    float m_aspect;
    float m_zNear;
    float m_zFar;
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;

    vec3 m_position;
    vec3 m_rotation; // euler degrees (pitch, yaw, roll)

    mutable mat4 m_view;
    mutable mat4 m_projection;
    mutable mat4 m_viewProj;
    mutable bool m_dirty;
};

} // namespace terra
