#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "terra/core/math.h"
#include "terra/voxel/voxel.h"

namespace terra {

// Fixed-size 3D chunk of voxels. Chunks are the unit of storage, meshing and
// rendering for the voxel world.
class Chunk {
public:
    static constexpr int kChunkSize = 16;
    static constexpr int kChunkHeight = 256;

    Chunk(int chunkX, int chunkZ);

    // Chunk coordinate (in chunk space).
    int chunkX() const { return m_chunkX; }
    int chunkZ() const { return m_chunkZ; }

    // World position of the chunk origin.
    int worldOriginX() const { return m_chunkX * kChunkSize; }
    int worldOriginZ() const { return m_chunkZ * kChunkSize; }

    // Get/set a voxel by local chunk coordinates. Bounds-checked.
    Voxel getVoxel(int x, int y, int z) const;
    void setVoxel(int x, int y, int z, Voxel v);

    // Get/set by linear local index [x + y*kChunkSize + z*kChunkSize*kChunkHeight].
    Voxel getVoxel(size_t index) const;
    void setVoxel(size_t index, Voxel v);

    bool modified() const { return m_modified; }
    void setModified(bool modified) { m_modified = modified; }

    bool meshDirty() const { return m_meshDirty; }
    void setMeshDirty(bool dirty) { m_meshDirty = dirty; }

private:
    int m_chunkX;
    int m_chunkZ;
    bool m_modified;
    bool m_meshDirty;

    // Voxel data stored as [x][y][z] for cache-friendly vertical slices.
    std::array<Voxel, kChunkSize * kChunkHeight * kChunkSize> m_voxels;

    static size_t indexOf(int x, int y, int z);
};

} // namespace terra
