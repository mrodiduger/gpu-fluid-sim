#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

struct SimulationAccuracyStats {
    double meanDensity = 0.0;
    double meanSignedDensityError = 0.0;
    double meanPositiveDensityError = 0.0;
    double maxPositiveDensityError = 0.0;
    double rmsSpeed = 0.0;
    size_t nonFinitePositions = 0;
    size_t nonFiniteVelocities = 0;
    size_t nonFiniteDensities = 0;
};

SimulationAccuracyStats calculateSimulationAccuracyStats(
        const std::vector<float> &positions,
        const std::vector<float> &velocities,
        const std::vector<float> &densities,
        uint32_t dimensions,
        uint32_t stride,
        float targetDensity);
