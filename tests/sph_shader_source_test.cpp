#include <cassert>
#include <fstream>
#include <iterator>
#include <string>

std::string readText(const std::string &path) {
    std::ifstream input(path);
    assert(input.is_open());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
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

    const size_t indexLoad =
            traversal.find("uint NEIGHBOUR_INDEX = dequantize_index(lookup);");
    const size_t exactPositionLoad = traversal.find(
            "VEC_T NEIGHBOUR_POSITION = COORDINATES_BUFFER_NAME[NEIGHBOUR_INDEX];");
    assert(indexLoad != std::string::npos);
    assert(exactPositionLoad != std::string::npos);
    assert(indexLoad < exactPositionLoad);
    assert(traversal.find("VEC_T NEIGHBOUR_POSITION = dequantize_position(lookup);") ==
           std::string::npos);

    assert(particle.find(
                   "max(density - constants.targetDensity, 0.0)") !=
           std::string::npos);
    assert(particle.find("float pressureCoefficient =") != std::string::npos);
    assert(particle.find("float neighbourPressureCoefficient =") !=
           std::string::npos);
    assert(particle.find(
                   "pressureCoefficient + neighbourPressureCoefficient") !=
           std::string::npos);
    assert(particle.find("calculateSharedPressure") == std::string::npos);

    assert(density.find("#define GRID_BINDING_COORDINATES -1") !=
           std::string::npos);
    assert(particle.find("#define GRID_BINDING_COORDINATES -1") !=
           std::string::npos);
    assert(background.find("#define GRID_BINDING_COORDINATES -1") !=
           std::string::npos);
}
