#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "terra/core/math.h"
#include "terra/voxel/chunk.h"

namespace terra {

// A chunk coordinate key for the sparse voxel world.
struct ChunkCoord {
    int x;
    int z;

    bool operator==(const ChunkCoord& o) const { return x == o.x && z == o.z; }
};

struct ChunkCoordHash {
    size_t operator()(const ChunkCoord& c) const {
        return (static_cast<size_t>(c.x) * 0x9E3779B1u) ^
               (static_cast<size_t>(c.z) * 0xFB599123u);
    }
};

// Sparse voxel world: owns loaded chunks and coordinates meshing/terrain.
class VoxelWorld {
public:
    VoxelWorld();
    ~VoxelWorld();

    // Get or create the chunk at (chunkX, chunkZ).
    Chunk* getChunk(int chunkX, int chunkZ);
    const Chunk* getChunk(int chunkX, int chunkZ) const;

    // Ensure a chunk exists (load/generate if missing).
    Chunk* ensureChunk(int chunkX, int chunkZ);

    // Remove a chunk from the world.
    void unloadChunk(int chunkX, int chunkZ);

    // Get/set a voxel by world coordinates.
    Voxel getVoxel(int wx, int wy, int wz) const;
    void setVoxel(int wx, int wy, int wz, Voxel v);

    // All chunks currently loaded.
    const std::vector<Chunk*>& chunks() const { return m_chunks; }

private:
    std::unordered_map<ChunkCoord, Chunk*, ChunkCoordHash> m_chunkMap;
    std::vector<Chunk*> m_chunks;
};

} // namespace terra
