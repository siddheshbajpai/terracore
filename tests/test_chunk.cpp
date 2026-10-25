#include "tests/test_runner.h"

#include "terra/voxel/chunk.h"

using namespace terra;

TERRA_TEST_CASE(chunk_default_all_air) {
    Chunk c(0, 0);
    TERRA_CHECK(c.chunkX() == 0);
    TERRA_CHECK(c.chunkZ() == 0);
    TERRA_CHECK(c.worldOriginX() == 0);
    TERRA_CHECK(c.worldOriginZ() == 0);
    for (int y = 0; y < Chunk::kChunkHeight; ++y) {
        for (int z = 0; z < Chunk::kChunkSize; ++z) {
            for (int x = 0; x < Chunk::kChunkSize; ++x) {
                TERRA_CHECK(!c.getVoxel(x, y, z).isSolid());
            }
        }
    }
}

TERRA_TEST_CASE(chunk_set_get_voxel) {
    Chunk c(1, 2);
    c.setVoxel(5, 10, 7, Voxel::stone());
    TERRA_CHECK(c.getVoxel(5, 10, 7).isSolid());
    TERRA_CHECK(c.getVoxel(5, 10, 7).typeId == VOXEL_STONE);
    // Out of bounds returns air
    TERRA_CHECK(!c.getVoxel(-1, 0, 0).isSolid());
    TERRA_CHECK(!c.getVoxel(Chunk::kChunkSize, 0, 0).isSolid());
    TERRA_CHECK(!c.getVoxel(0, Chunk::kChunkHeight, 0).isSolid());
}

TERRA_TEST_CASE(chunk_modified_flag) {
    Chunk c(0, 0);
    TERRA_CHECK(!c.modified());
    c.setVoxel(0, 0, 0, Voxel::stone());
    TERRA_CHECK(c.modified());
    TERRA_CHECK(c.meshDirty());
}

TERRA_TEST_CASE(chunk_linear_index) {
    Chunk c(0, 0);
    c.setVoxel(1, 2, 3, Voxel::dirt());
    size_t idx = 1 + 2 * Chunk::kChunkSize +
                 3 * Chunk::kChunkSize * Chunk::kChunkHeight;
    TERRA_CHECK(c.getVoxel(idx).typeId == VOXEL_DIRT);
}

TERRA_TEST_CASE(chunk_negative_world_origin) {
    Chunk c(-1, -1);
    TERRA_CHECK(c.worldOriginX() == -16);
    TERRA_CHECK(c.worldOriginZ() == -16);
}
