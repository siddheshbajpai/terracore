#pragma once

#include <cstdint>
#include <vector>

#include "terra/core/math.h"
#include "terra/voxel/chunk.h"

namespace terra {

// Procedural terrain generator producing heightmaps and filling chunks.
// Deterministic given a seed (simple noise for the MVP; replace with a
// proper noise library later).
class Terrain {
public:
    Terrain();

    void setSeed(uint32_t seed);
    uint32_t seed() const { return m_seed; }

    // Height at world (x, z).
    int heightAt(int x, int z) const;

    // Fill a chunk with terrain voxels.
    void generate(Chunk& chunk) const;

private:
    uint32_t m_seed;

    // Simple deterministic pseudo-noise (LCG-based, hash-like).
    float noise(int x, int z) const;
};

} // namespace terra
