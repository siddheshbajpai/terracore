#include "terra/voxel/voxel.h"

namespace terra {

VoxelType::VoxelType()
    : id(0)
    , name("air")
    , solid(false)
    , transparent(true) {}

Voxel Voxel::air() { return Voxel{VOXEL_AIR, 0}; }
Voxel Voxel::stone() { return Voxel{VOXEL_STONE, 0}; }
Voxel Voxel::dirt() { return Voxel{VOXEL_DIRT, 0}; }
Voxel Voxel::grass() { return Voxel{VOXEL_GRASS, 0}; }

bool Voxel::isSolid() const { return typeId != VOXEL_AIR; }

} // namespace terra
