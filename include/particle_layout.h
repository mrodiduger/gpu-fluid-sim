#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "helper.h"

constexpr uint32_t particleSpatialDimensions(SceneType type) {
    return type == SceneType::SPH_BOX_2D ? 2U : 3U;
}

constexpr uint32_t particleVectorComponents(SceneType type) {
    return type == SceneType::SPH_BOX_2D ? 2U : 4U;
}

constexpr size_t particleVectorScalarCount(SceneType type, uint32_t numParticles) {
    return static_cast<size_t>(particleVectorComponents(type)) * numParticles;
}

constexpr size_t particleVectorByteSize(SceneType type, uint32_t numParticles) {
    return particleVectorScalarCount(type, numParticles) * sizeof(float);
}
