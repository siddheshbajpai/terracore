#include "tests/test_runner.h"

#include "terra/scene/camera.h"

using namespace terra;

// Camera tests are intentionally minimal on this legacy toolchain:
// Camera construction triggers a codegen crash in g++ 6.3, and
// setLookAt()/updateMatrices() depend on it. The free math functions
// (perspective, lookAt) are covered by test_math.cpp instead.
