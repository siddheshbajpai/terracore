#pragma once

#include <cstdint>
#include <vector>

#include "terra/voxel/chunk.h"

namespace terra {

// A GPU-ready mesh produced from a chunk's voxel data.
struct ChunkMesh {
    std::vector<float> vertices;   // interleaved: x,y,z, nx,ny,nz, u,v
    std::vector<uint32_t> indices;
    int chunkX;
    int chunkZ;
    bool valid;
};

// Builds a ChunkMesh from a Chunk using a greedy / face-culling approach for
// the MVP. Only visible faces between solid and air voxels are emitted.
class ChunkMesher {
public:
    ChunkMesh mesh(const Chunk& chunk) const;
};

} // namespace terra
