#include "tests/test_runner.h"

#include "terra/voxel/terrain.h"

using namespace terra;

TERRA_TEST_CASE(terrain_height_in_range) {
    Terrain t;
    for (int x = -50; x <= 50; x += 7) {
        for (int z = -50; z <= 50; z += 7) {
            int h = t.heightAt(x, z);
            TERRA_CHECK(h >= 0);
            TERRA_CHECK(h < 200);
        }
    }
}

TERRA_TEST_CASE(terrain_deterministic) {
    Terrain a;
    Terrain b;
    a.setSeed(42u);
    b.setSeed(42u);
    for (int x = -30; x <= 30; ++x) {
        for (int z = -30; z <= 30; ++z) {
            TERRA_CHECK(a.heightAt(x, z) == b.heightAt(x, z));
        }
    }
}

TERRA_TEST_CASE(terrain_different_seed_different) {
    Terrain a;
    Terrain b;
    a.setSeed(1u);
    b.setSeed(2u);
    bool different = false;
    for (int x = -20; x <= 20 && !different; ++x) {
        for (int z = -20; z <= 20 && !different; ++z) {
            if (a.heightAt(x, z) != b.heightAt(x, z)) different = true;
        }
    }
    TERRA_CHECK(different);
}

TERRA_TEST_CASE(terrain_generate_fills_chunk) {
    Terrain t;
    Chunk c(0, 0);
    t.generate(c);
    // Surface should be solid, sky above should be air.
    bool foundSolid = false;
    bool foundAir = false;
    for (int z = 0; z < Chunk::kChunkSize; ++z) {
        for (int x = 0; x < Chunk::kChunkSize; ++x) {
            int h = t.heightAt(x, z);
            if (c.getVoxel(x, h > 0 ? h - 1 : 0, z).isSolid()) foundSolid = true;
            if (!c.getVoxel(x, Chunk::kChunkHeight - 1, z).isSolid()) {
                foundAir = true;
            }
        }
    }
    TERRA_CHECK(foundSolid);
    TERRA_CHECK(foundAir);
    TERRA_CHECK(c.modified());
    TERRA_CHECK(c.meshDirty());
}
