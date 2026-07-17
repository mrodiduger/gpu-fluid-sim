#include <cassert>
#include <cctype>
#include <fstream>
#include <iterator>
#include <string>

std::string readText(const std::string &path) {
    std::ifstream input(path);
    assert(input.is_open());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::string removeWhitespace(const std::string &text) {
    std::string normalized;
    normalized.reserve(text.size());
    for (char character : text) {
        if (!std::isspace(static_cast<unsigned char>(character))) {
            normalized += character;
        }
    }
    return normalized;
}

int main() {
    const std::string root = GPU_FLUID_SOURCE_DIR;
    const std::string traversal =
            readText(root + "/shaders/spatial_lookup.traversal.glsl");
    const std::string particle =
            readText(root + "/shaders/particle_simulation.comp");
    const std::string density =
            readText(root + "/shaders/density_update.comp");
    const std::string background =
            readText(root + "/shaders/background2d.frag");

    const size_t classRejection = traversal.find("if (pClass != nClass)");
    const size_t foundClass =
            traversal.find("foundClass = true;", classRejection);
    const size_t indexLoad =
            traversal.find("uint NEIGHBOUR_INDEX = dequantize_index(lookup);",
                           foundClass);
    const size_t exactPositionLoad = traversal.find(
            "VEC_T NEIGHBOUR_POSITION = COORDINATES_BUFFER_NAME[NEIGHBOUR_INDEX];",
            indexLoad);
    assert(classRejection != std::string::npos);
    assert(foundClass != std::string::npos);
    assert(indexLoad != std::string::npos);
    assert(exactPositionLoad != std::string::npos);
    assert(classRejection < foundClass);
    assert(foundClass < indexLoad);
    assert(indexLoad < exactPositionLoad);
    assert(traversal.find("VEC_T NEIGHBOUR_POSITION = dequantize_position(lookup);") ==
           std::string::npos);

    const std::string normalizedParticle = removeWhitespace(particle);
    assert(particle.find(
                   "max(density - constants.targetDensity, 0.0)") !=
           std::string::npos);
    assert(particle.find("float pressureCoefficient =") != std::string::npos);
    assert(particle.find("float neighbourPressureCoefficient =") !=
           std::string::npos);
    assert(particle.find(
                   "pressureCoefficient + neighbourPressureCoefficient") !=
           std::string::npos);
    assert(normalizedParticle.find(
                   "floatpressureCoefficient="
                   "pressure*inverseDensity*inverseDensity;") !=
           std::string::npos);
    assert(normalizedParticle.find(
                   "floatneighbourPressureCoefficient="
                   "neighbourPressure*inverseNeighbourDensity*"
                   "inverseNeighbourDensity;") !=
           std::string::npos);
    assert(normalizedParticle.find(
                   "pressureAcceleration-=particleMass*"
                   "(pressureCoefficient+neighbourPressureCoefficient)*"
                   "direction*kernelDerivative;") !=
           std::string::npos);
    assert(normalizedParticle.find(
                   "#ifdefDEF_2DconstfloatmouseForceStrength=1000.0;") !=
           std::string::npos);
    assert(normalizedParticle.find(
                   "floatmouseRadius=2.0*constants.spatialRadius;") !=
           std::string::npos);
    assert(normalizedParticle.find(
                   "floatmouseInfluence=1.0-smoothstep(0.0,mouseRadius,mouseDistance);") !=
           std::string::npos);
    assert(normalizedParticle.find(
                   "velocity+=mouseDrag*mouseForceStrength*mouseInfluence*constants.deltaTime;") !=
           std::string::npos);
    assert(particle.find("calculateSharedPressure") == std::string::npos);

    assert(density.find("#define GRID_BINDING_COORDINATES -1") !=
           std::string::npos);
    assert(particle.find("#define GRID_BINDING_COORDINATES -1") !=
           std::string::npos);
    assert(background.find("#define GRID_BINDING_COORDINATES -1") !=
           std::string::npos);
}
