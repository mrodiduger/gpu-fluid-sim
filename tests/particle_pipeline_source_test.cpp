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
    const std::string source =
            readText(std::string(GPU_FLUID_SOURCE_DIR) + "/src/particle_physics.cpp");
    const size_t transferRead = source.find("vk::AccessFlagBits::eTransferRead");
    const size_t velocityCopy = source.find("cmd.copyBuffer");

    assert(transferRead != std::string::npos);
    assert(velocityCopy != std::string::npos);
    assert(transferRead < velocityCopy);
}
