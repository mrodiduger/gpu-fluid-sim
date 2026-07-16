#include "simulation_diagnostics.h"

#include <cassert>
#include <cmath>
#include <limits>

bool approximatelyEqual(double left, double right, double tolerance = 1e-9) {
    return std::abs(left - right) <= tolerance;
}

int main() {
    const std::vector<float> positions {
            0.0f, 0.0f,
            1.0f, 1.0f,
            2.0f, 2.0f};
    const std::vector<float> velocities {
            3.0f, 4.0f,
            0.0f, 0.0f,
            0.0f, 0.0f};
    const std::vector<float> densities {90.0f, 110.0f, 130.0f};

    const auto stats = calculateSimulationAccuracyStats(
            positions, velocities, densities, 2, 2, 100.0f);

    assert(approximatelyEqual(stats.meanDensity, 110.0));
    assert(approximatelyEqual(stats.meanSignedDensityError, 0.1));
    assert(approximatelyEqual(stats.meanPositiveDensityError, 4.0 / 30.0));
    assert(approximatelyEqual(stats.maxPositiveDensityError, 0.3));
    assert(approximatelyEqual(stats.rmsSpeed, std::sqrt(25.0 / 3.0)));
    assert(stats.nonFinitePositions == 0);
    assert(stats.nonFiniteVelocities == 0);
    assert(stats.nonFiniteDensities == 0);

    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float infinity = std::numeric_limits<float>::infinity();
    const auto nonFiniteStats = calculateSimulationAccuracyStats(
            {0.0f, nan, infinity, 0.0f},
            {0.0f, 0.0f, 0.0f, infinity},
            {100.0f, nan},
            2,
            2,
            100.0f);

    assert(approximatelyEqual(nonFiniteStats.meanDensity, 100.0));
    assert(approximatelyEqual(nonFiniteStats.rmsSpeed, 0.0));
    assert(nonFiniteStats.nonFinitePositions == 2);
    assert(nonFiniteStats.nonFiniteVelocities == 1);
    assert(nonFiniteStats.nonFiniteDensities == 1);
}
