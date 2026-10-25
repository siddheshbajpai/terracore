#include "terra/voxel/terrain.h"

#include <cstdint>

namespace terra {

Terrain::Terrain() : m_seed(1337u) {}

void Terrain::setSeed(uint32_t seed) { m_seed = seed; }

float Terrain::noise(int x, int z) const {
    // Simple 2D hash -> value in [0,1). Deterministic given seed.
    uint32_t h = static_cast<uint32_t>(x) * 0x45D9F3BFu +
                 static_cast<uint32_t>(z) * 0x9E3779B1u + m_seed;
    h ^= h >> 16;
    h *= 0x85EBCA6Bu;
    h ^= h >> 13;
    h *= 0xC2B2AE35u;
    h ^= h >> 16;
    return (h & 0x7FFFFFFFu) / (float)0x7FFFFFFFu;
}

int Terrain::heightAt(int x, int z) const {
    // Combine a few noise octaves for a rolling terrain.
    float h = 0.0f;
    float amp = 1.0f;
    float freq = 1.0f;
    float maxAmp = 0.0f;
    for (int o = 0; o < 4; ++o) {
        int ix = (int)(x * freq);
        int iz = (int)(z * freq);
        h += noise(ix, iz) * amp;
        maxAmp += amp;
        amp *= 0.5f;
        freq *= 2.0f;
    }
    h /= maxAmp;
    // Sea level around 10-30 blocks, mountains up to ~64.
    return (int)(h * 54.0f) + 8;
}

void Terrain::generate(Chunk& chunk) const {
    int ox = chunk.worldOriginX();
    int oz = chunk.worldOriginZ();

    for (int z = 0; z < Chunk::kChunkSize; ++z) {
        for (int x = 0; x < Chunk::kChunkSize; ++x) {
            int wx = ox + x;
            int wz = oz + z;
            int h = heightAt(wx, wz);

            for (int y = 0; y < Chunk::kChunkHeight; ++y) {
                Voxel v;
                if (y < h) {
                    // Surface grass, below dirt, deep stone.
                    if (y == h - 1) {
                        v = Voxel::grass();
                    } else if (y > h - 5) {
                        v = Voxel::dirt();
                    } else {
                        v = Voxel::stone();
                    }
                } else {
                    v = Voxel::air();
                }
                chunk.setVoxel(x, y, z, v);
            }
        }
    }
    chunk.setModified(true);
    chunk.setMeshDirty(true);
}

} // namespace terra
