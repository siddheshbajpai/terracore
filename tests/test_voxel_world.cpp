#include "tests/test_runner.h"

#include "terra/voxel/voxel_world.h"
#include "terra/voxel/terrain.h"

using namespace terra;

TERRA_TEST_CASE(voxel_world_get_set_world_coords) {
    VoxelWorld world;
    // A block well inside chunk (0,0).
    world.setVoxel(5, 10, 5, Voxel::stone());
    TERRA_CHECK(world.getVoxel(5, 10, 5).isSolid());
    TERRA_CHECK(world.getVoxel(5, 10, 5).typeId == VOXEL_STONE);
    // Air elsewhere.
    TERRA_CHECK(!world.getVoxel(100, 100, 100).isSolid());
    TERRA_CHECK(!world.getVoxel(5, 100, 5).isSolid()); // above height
}

TERRA_TEST_CASE(voxel_world_chunk_creation) {
    VoxelWorld world;
    world.setVoxel(20, 5, 20, Voxel::grass()); // lands in chunk (1,1)
    Chunk* c = world.getChunk(1, 1);
    TERRA_CHECK(c != nullptr);
    TERRA_CHECK(c->getVoxel(4, 5, 4).isSolid());
}

TERRA_TEST_CASE(voxel_world_negative_coords_chunk) {
    VoxelWorld world;
    world.setVoxel(-5, 5, -5, Voxel::stone());
    // -5 / 16 with C++ truncation toward 0 is 0, but we want floor behavior:
    // our mapping handles negatives so -5 goes to chunk -1 with local 11.
    TERRA_CHECK(world.getVoxel(-5, 5, -5).isSolid());
    Chunk* c = world.getChunk(-1, -1);
    TERRA_CHECK(c != nullptr);
}

TERRA_TEST_CASE(voxel_world_unload_chunk) {
    VoxelWorld world;
    world.setVoxel(5, 5, 5, Voxel::stone());
    Chunk* c = world.getChunk(0, 0);
    TERRA_CHECK(c != nullptr);
    world.unloadChunk(0, 0);
    TERRA_CHECK(world.getChunk(0, 0) == nullptr);
    TERRA_CHECK(!world.getVoxel(5, 5, 5).isSolid());
}

TERRA_TEST_CASE(voxel_world_terrain_generate_chunk) {
    VoxelWorld world;
    Terrain t;
    Chunk* c = world.ensureChunk(0, 0);
    t.generate(*c);
    TERRA_CHECK(c->modified());
    TERRA_CHECK(c->meshDirty());
    bool anySolid = false;
    for (int z = 0; z < Chunk::kChunkSize && !anySolid; ++z) {
        for (int x = 0; x < Chunk::kChunkSize && !anySolid; ++x) {
            for (int y = 0; y < Chunk::kChunkHeight; ++y) {
                if (c->getVoxel(x, y, z).isSolid()) { anySolid = true; break; }
            }
        }
    }
    TERRA_CHECK(anySolid);
}
