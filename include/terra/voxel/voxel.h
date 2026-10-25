#pragma once

#include <cstdint>
#include <string>

namespace terra {

// A single voxel type / block definition.
struct VoxelType {
    uint8_t id;
    std::string name;
    bool solid;
    bool transparent;

    VoxelType();
};

// Voxel data value stored in a chunk. Packs the type id and a little
// metadata (light, etc.) for the MVP.
struct Voxel {
    uint8_t typeId;
    uint8_t meta;

    static Voxel air();
    static Voxel stone();
    static Voxel grass();
    static Voxel dirt();
    bool isSolid() const;
};

enum : uint8_t {
    VOXEL_AIR = 0,
    VOXEL_STONE = 1,
    VOXEL_DIRT = 2,
    VOXEL_GRASS = 3,
};

} // namespace terra
