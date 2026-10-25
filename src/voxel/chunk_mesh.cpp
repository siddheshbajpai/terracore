#include "terra/voxel/chunk_mesh.h"

#include <algorithm>

namespace terra {

namespace {

// Six cube face normals (x, y, z).
const int kFaceNormals[6][3] = {
    {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1},
};

// UV offset per face for simple texture mapping (atlas-friendly later).
const float kFaceU[6] = {0, 0, 0, 0, 0, 0};
const float kFaceV[6] = {0, 0, 0, 0, 0, 0};

// Append a quad (2 triangles) to vertex/index buffers.
void addQuad(std::vector<float>& verts, std::vector<uint32_t>& indices,
             const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3,
             const vec3& normal, float u, float v) {
    uint32_t base = (uint32_t)(verts.size() / 8); // 8 floats per vertex
    // positions + normals + uv
    auto emit = [&](const vec3& p) {
        verts.push_back(p.x);
        verts.push_back(p.y);
        verts.push_back(p.z);
        verts.push_back(normal.x);
        verts.push_back(normal.y);
        verts.push_back(normal.z);
        verts.push_back(u);
        verts.push_back(v);
    };
    emit(p0);
    emit(p1);
    emit(p2);
    emit(p3);
    indices.push_back(base);
    indices.push_back(base + 1);
    indices.push_back(base + 2);
    indices.push_back(base);
    indices.push_back(base + 2);
    indices.push_back(base + 3);
}

} // namespace

ChunkMesh ChunkMesher::mesh(const Chunk& chunk) const {
    ChunkMesh out;
    out.chunkX = chunk.chunkX();
    out.chunkZ = chunk.chunkZ();
    out.valid = false;

    const int S = Chunk::kChunkSize;
    const int H = Chunk::kChunkHeight;

    // For each voxel, emit faces only where neighbor is air (or out of
    // chunk bounds treated via the world - here we just cull internal faces).
    for (int y = 0; y < H; ++y) {
        for (int z = 0; z < S; ++z) {
            for (int x = 0; x < S; ++x) {
                Voxel v = chunk.getVoxel(x, y, z);
                if (!v.isSolid()) continue;

                vec3 p(x, y, z);

                for (int f = 0; f < 6; ++f) {
                    int nx = x + kFaceNormals[f][0];
                    int ny = y + kFaceNormals[f][1];
                    int nz = z + kFaceNormals[f][2];

                    Voxel neighbor = chunk.getVoxel(nx, ny, nz);
                    if (neighbor.isSolid()) continue;

                    // Emit face on the side facing the neighbor.
                    vec3 n(kFaceNormals[f][0], kFaceNormals[f][1],
                           kFaceNormals[f][2]);

                    // Build quad corners based on normal axis.
                    vec3 a, b, c, d;
                    if (n.x != 0) {
                        // Face in YZ plane
                        float ox = n.x > 0 ? 1.0f : 0.0f;
                        a = vec3(ox, y, z);
                        b = vec3(ox, y, z + 1);
                        c = vec3(ox, y + 1, z + 1);
                        d = vec3(ox, y + 1, z);
                    } else if (n.y != 0) {
                        // Face in XZ plane
                        float oy = n.y > 0 ? 1.0f : 0.0f;
                        a = vec3(x, oy, z);
                        b = vec3(x + 1, oy, z);
                        c = vec3(x + 1, oy, z + 1);
                        d = vec3(x, oy, z + 1);
                    } else {
                        // Face in XY plane
                        float oz = n.z > 0 ? 1.0f : 0.0f;
                        a = vec3(x, y, oz);
                        b = vec3(x + 1, y, oz);
                        c = vec3(x + 1, y + 1, oz);
                        d = vec3(x, y + 1, oz);
                    }

                    // Order corners so normal faces outward correctly.
                    addQuad(out.vertices, out.indices, a, b, c, d, n,
                            kFaceU[f], kFaceV[f]);
                }
            }
        }
    }

    out.valid = true;
    return out;
}

} // namespace terra
