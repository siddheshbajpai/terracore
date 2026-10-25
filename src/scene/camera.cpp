#include "terra/scene/camera.h"

#include <cmath>

namespace terra {

Camera::Camera()
    : m_type(Perspective)
    , m_fovY(60.0f)
    , m_aspect(16.0f / 9.0f)
    , m_zNear(0.1f)
    , m_zFar(1000.0f)
    , m_left(-1.0f)
    , m_right(1.0f)
    , m_bottom(-1.0f)
    , m_top(1.0f)
    , m_position(0.0f, 0.0f, 3.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_dirty(true) {}

void Camera::setPerspective(float fovY, float aspect, float zNear,
                            float zFar) {
    m_type = Perspective;
    m_fovY = fovY;
    m_aspect = aspect;
    m_zNear = zNear;
    m_zFar = zFar;
    m_dirty = true;
}

void Camera::setOrthographic(float left, float right, float bottom,
                             float top, float zNear, float zFar) {
    m_type = Orthographic;
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    m_zNear = zNear;
    m_zFar = zFar;
    m_dirty = true;
}

void Camera::setAspect(float aspect) {
    m_aspect = aspect;
    m_dirty = true;
}

void Camera::setLookAt(const vec3& eye, const vec3& center, const vec3& up) {
    (void)up;
    m_position = eye;
    // Derive euler-ish rotation from forward vector for the MVP.
    vec3 forward = (center - eye).normalized();
    float pitch = degrees(asinf(-forward.y));
    float yaw = degrees(atan2f(forward.x, forward.z));
    m_rotation = vec3(pitch, yaw, 0.0f);
    m_dirty = true;
}

void Camera::updateMatrices() const {
    if (!m_dirty) return;

    m_view = lookAt(m_position,
                    m_position + vec3(
                        sinf(radians(m_rotation.y)) *
                            cosf(radians(m_rotation.x)),
                        sinf(radians(m_rotation.x)),
                        -cosf(radians(m_rotation.y)) *
                            cosf(radians(m_rotation.x))),
                    vec3(0.0f, 1.0f, 0.0f));

    if (m_type == Perspective) {
        m_projection = perspective(m_fovY, m_aspect, m_zNear, m_zFar);
    } else {
        m_projection =
            ortho(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
    }

    m_viewProj = m_projection * m_view;
    m_dirty = false;
}

} // namespace terra
