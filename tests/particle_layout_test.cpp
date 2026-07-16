#include "particle_layout.h"

#include <cassert>

int main() {
    static_assert(particleSpatialDimensions(SceneType::SPH_BOX_2D) == 2);
    static_assert(particleSpatialDimensions(SceneType::SPH_BOX_3D) == 3);
    static_assert(particleVectorComponents(SceneType::SPH_BOX_2D) == 2);
    static_assert(particleVectorComponents(SceneType::SPH_BOX_3D) == 4);

    assert(particleVectorScalarCount(SceneType::SPH_BOX_2D, 3) == 6);
    assert(particleVectorScalarCount(SceneType::SPH_BOX_3D, 3) == 12);
    assert(particleVectorByteSize(SceneType::SPH_BOX_2D, 3) == 6 * sizeof(float));
    assert(particleVectorByteSize(SceneType::SPH_BOX_3D, 3) == 12 * sizeof(float));
}
