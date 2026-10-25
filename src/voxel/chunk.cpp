#include "terra/voxel/chunk.h"

#include <algorithm>

namespace terra {

Chunk::Chunk(int chunkX, int chunkZ)
    : m_chunkX(chunkX)
    , m_chunkZ(chunkZ)
    , m_modified(false)
    , m_meshDirty(true) {
    m_voxels.fill(Voxel::air());
}

size_t Chunk::indexOf(int x, int y, int z) {
    return (static_cast<size_t>(x) +
            static_cast<size_t>(y) * kChunkSize +
            static_cast<size_t>(z) * kChunkSize * kChunkHeight);
}

Voxel Chunk::getVoxel(int x, int y, int z) const {
    if (x < 0 || x >= kChunkSize || y < 0 || y >= kChunkHeight ||
        z < 0 || z >= kChunkSize) {
        return Voxel::air();
    }
    return m_voxels[indexOf(x, y, z)];
}

void Chunk::setVoxel(int x, int y, int z, Voxel v) {
    if (x < 0 || x >= kChunkSize || y < 0 || y >= kChunkHeight ||
        z < 0 || z >= kChunkSize) {
        return;
    }
    m_voxels[indexOf(x, y, z)] = v;
    m_modified = true;
    m_meshDirty = true;
}

Voxel Chunk::getVoxel(size_t index) const {
    if (index >= m_voxels.size()) return Voxel::air();
    return m_voxels[index];
}

void Chunk::setVoxel(size_t index, Voxel v) {
    if (index >= m_voxels.size()) return;
    m_voxels[index] = v;
    m_modified = true;
    m_meshDirty = true;
}

} // namespace terra
