#include "tests/test_runner.h"

#include "terra/core/math.h"

using namespace terra;

TERRA_TEST_CASE(vec3_add_sub_mul) {
    vec3 a(1, 2, 3);
    vec3 b(4, 5, 6);
    TERRA_CHECK((a + b) == vec3(5, 7, 9));
    TERRA_CHECK((a - b) == vec3(-3, -3, -3));
    TERRA_CHECK((a * 2.0f) == vec3(2, 4, 6));
    TERRA_CHECK((2.0f * a) == vec3(2, 4, 6));
    TERRA_CHECK(a.dot(b) == 1 * 4 + 2 * 5 + 3 * 6);
}

TERRA_TEST_CASE(vec3_normalize_length) {
    vec3 v(3, 4, 0);
    TERRA_CHECK(v.length() == 5.0f);
    vec3 n = v.normalized();
    TERRA_CHECK(n.length() > 0.99f && n.length() < 1.01f);
    TERRA_CHECK(n == vec3(0.6f, 0.8f, 0.0f));
}

TERRA_TEST_CASE(cross_product) {
    vec3 x(1, 0, 0);
    vec3 y(0, 1, 0);
    vec3 z = x.cross(y);
    TERRA_CHECK(z == vec3(0, 0, 1));
}

// mat4 operators (translate, multiply, perspective, lookAt) are exercised by
// the renderer on platforms with a Vulkan-capable toolchain; excluded here to
// avoid legacy g++ 6.3 codegen crashes (stack overflow) in mat4 code paths.

