#include "terra/voxel/voxel_world.h"

#include <algorithm>

namespace terra {

VoxelWorld::VoxelWorld() {}

VoxelWorld::~VoxelWorld() {
    for (auto* c : m_chunks) delete c;
    m_chunks.clear();
    m_chunkMap.clear();
}

Chunk* VoxelWorld::getChunk(int chunkX, int chunkZ) {
    ChunkCoord key{chunkX, chunkZ};
    auto it = m_chunkMap.find(key);
    return it != m_chunkMap.end() ? it->second : nullptr;
}

const Chunk* VoxelWorld::getChunk(int chunkX, int chunkZ) const {
    ChunkCoord key{chunkX, chunkZ};
    auto it = m_chunkMap.find(key);
    return it != m_chunkMap.end() ? it->second : nullptr;
}

Chunk* VoxelWorld::ensureChunk(int chunkX, int chunkZ) {
    Chunk* existing = getChunk(chunkX, chunkZ);
    if (existing) return existing;

    Chunk* c = new Chunk(chunkX, chunkZ);
    ChunkCoord key{chunkX, chunkZ};
    m_chunkMap[key] = c;
    m_chunks.push_back(c);
    return c;
}

void VoxelWorld::unloadChunk(int chunkX, int chunkZ) {
    ChunkCoord key{chunkX, chunkZ};
    auto it = m_chunkMap.find(key);
    if (it == m_chunkMap.end()) return;
    Chunk* c = it->second;
    m_chunks.erase(std::remove(m_chunks.begin(), m_chunks.end(), c),
                   m_chunks.end());
    m_chunkMap.erase(it);
    delete c;
}

Voxel VoxelWorld::getVoxel(int wx, int wy, int wz) const {
    if (wy < 0 || wy >= Chunk::kChunkHeight) return Voxel::air();
    int cx = wx >= 0 ? wx / Chunk::kChunkSize
                     : (wx - Chunk::kChunkSize + 1) / Chunk::kChunkSize;
    int cz = wz >= 0 ? wz / Chunk::kChunkSize
                     : (wz - Chunk::kChunkSize + 1) / Chunk::kChunkSize;
    int lx = wx - cx * Chunk::kChunkSize;
    int lz = wz - cz * Chunk::kChunkSize;

    const Chunk* c = getChunk(cx, cz);
    return c ? c->getVoxel(lx, wy, lz) : Voxel::air();
}

void VoxelWorld::setVoxel(int wx, int wy, int wz, Voxel v) {
    if (wy < 0 || wy >= Chunk::kChunkHeight) return;
    int cx = wx >= 0 ? wx / Chunk::kChunkSize
                     : (wx - Chunk::kChunkSize + 1) / Chunk::kChunkSize;
    int cz = wz >= 0 ? wz / Chunk::kChunkSize
                     : (wz - Chunk::kChunkSize + 1) / Chunk::kChunkSize;
    int lx = wx - cx * Chunk::kChunkSize;
    int lz = wz - cz * Chunk::kChunkSize;

    Chunk* c = ensureChunk(cx, cz);
    c->setVoxel(lx, wy, lz, v);
}

} // namespace terra
