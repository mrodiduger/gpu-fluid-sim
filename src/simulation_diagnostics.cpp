#include "simulation_diagnostics.h"

#include <algorithm>
#include <cmath>

SimulationAccuracyStats calculateSimulationAccuracyStats(
        const std::vector<float> &positions,
        const std::vector<float> &velocities,
        const std::vector<float> &densities,
        uint32_t dimensions,
        uint32_t stride,
        float targetDensity) {
    SimulationAccuracyStats stats;
    size_t finiteDensityCount = 0;
    size_t finiteVelocityCount = 0;
    double densitySum = 0.0;
    double positiveDensityErrorSum = 0.0;
    double speedSquaredSum = 0.0;

    for (size_t particleIndex = 0; particleIndex < densities.size(); particleIndex++) {
        const float density = densities[particleIndex];
        if (std::isfinite(density)) {
            const double densityError = static_cast<double>(density) - targetDensity;
            const double positiveDensityError = std::max(densityError, 0.0);
            densitySum += density;
            positiveDensityErrorSum += positiveDensityError;
            stats.maxPositiveDensityError =
                    std::max(stats.maxPositiveDensityError, positiveDensityError / targetDensity);
            finiteDensityCount++;
        } else {
            stats.nonFiniteDensities++;
        }

        bool finiteVelocity = true;
        double speedSquared = 0.0;
        for (uint32_t component = 0; component < dimensions; component++) {
            const size_t offset = particleIndex * stride + component;
            if (!std::isfinite(positions[offset])) {
                stats.nonFinitePositions++;
            }
            if (!std::isfinite(velocities[offset])) {
                stats.nonFiniteVelocities++;
                finiteVelocity = false;
            } else {
                speedSquared += static_cast<double>(velocities[offset]) * velocities[offset];
            }
        }

        if (finiteVelocity) {
            speedSquaredSum += speedSquared;
            finiteVelocityCount++;
        }
    }

    if (finiteDensityCount > 0) {
        stats.meanDensity = densitySum / finiteDensityCount;
        stats.meanSignedDensityError = (stats.meanDensity - targetDensity) / targetDensity;
        stats.meanPositiveDensityError =
                positiveDensityErrorSum / finiteDensityCount / targetDensity;
    }
    if (finiteVelocityCount > 0) {
        stats.rmsSpeed = std::sqrt(speedSquaredSum / finiteVelocityCount);
    }

    return stats;
}
