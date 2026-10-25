#include "terra/scene/light.h"

namespace terra {

Light::Light()
    : type(Directional)
    , color(1.0f, 1.0f, 1.0f)
    , intensity(1.0f)
    , position(0.0f, 0.0f, 0.0f)
    , direction(0.0f, -1.0f, 0.0f) {}

} // namespace terra
