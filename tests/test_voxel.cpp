#include "tests/test_runner.h"

#include "terra/voxel/voxel.h"

using namespace terra;

TERRA_TEST_CASE(voxel_air_is_not_solid) {
    TERRA_CHECK(!Voxel::air().isSolid());
}

TERRA_TEST_CASE(voxel_solid_types) {
    TERRA_CHECK(Voxel::stone().isSolid());
    TERRA_CHECK(Voxel::dirt().isSolid());
    TERRA_CHECK(Voxel::grass().isSolid());
}
