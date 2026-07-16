#include <cassert>
#include <fstream>
#include <iterator>
#include <string>

std::string readText(const std::string &path) {
    std::ifstream input(path);
    assert(input.is_open());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

size_t countOccurrences(const std::string &text, const std::string &needle) {
    assert(!needle.empty());

    size_t count = 0;
    size_t position = 0;
    while ((position = text.find(needle, position)) != std::string::npos) {
        count++;
        position += needle.size();
    }
    return count;
}

int main() {
    const std::string physicsSource =
            readText(std::string(GPU_FLUID_SOURCE_DIR) + "/src/particle_physics.cpp");
    const std::string simulationStateSource =
            readText(std::string(GPU_FLUID_SOURCE_DIR) + "/src/simulation_state.cpp");

    assert(countOccurrences(physicsSource, "cmd.dispatch(") == 3);
    assert(countOccurrences(physicsSource, "computeBarrier(cmd)") == 2);
    assert(countOccurrences(physicsSource, "cmd.pipelineBarrier(") == 1);
    assert(countOccurrences(physicsSource, "cmd.copyBuffer(") == 1);
    assert(countOccurrences(physicsSource, "createDeviceLocalBuffer(") == 1);

    const size_t densityPipeline = physicsSource.find(
            "cmd.bindPipeline(vk::PipelineBindPoint::eCompute, densityPipeline)");
    const size_t densityDispatch = physicsSource.find("cmd.dispatch(", densityPipeline);
    const size_t densityBarrier = physicsSource.find("computeBarrier(cmd)", densityDispatch);
    const size_t forcePipeline = physicsSource.find(
            "cmd.bindPipeline(vk::PipelineBindPoint::eCompute, computePipeline)",
            densityBarrier);
    const size_t forceDispatch = physicsSource.find("cmd.dispatch(", forcePipeline);
    const size_t forceBarrier = physicsSource.find("computeBarrier(cmd)", forceDispatch);
    const size_t positionPipeline = physicsSource.find(
            "cmd.bindPipeline(vk::PipelineBindPoint::eCompute, positionUpdatePipeline)",
            forceBarrier);
    const size_t positionDispatch = physicsSource.find("cmd.dispatch(", positionPipeline);
    const size_t transferBarrier = physicsSource.find("cmd.pipelineBarrier(", positionDispatch);
    const size_t velocityCopy = physicsSource.find("cmd.copyBuffer(", transferBarrier);

    assert(densityPipeline != std::string::npos);
    assert(densityDispatch != std::string::npos);
    assert(densityBarrier != std::string::npos);
    assert(forcePipeline != std::string::npos);
    assert(forceDispatch != std::string::npos);
    assert(forceBarrier != std::string::npos);
    assert(positionPipeline != std::string::npos);
    assert(positionDispatch != std::string::npos);
    assert(transferBarrier != std::string::npos);
    assert(velocityCopy != std::string::npos);
    assert(densityPipeline < densityDispatch);
    assert(densityDispatch < densityBarrier);
    assert(densityBarrier < forcePipeline);
    assert(forcePipeline < forceDispatch);
    assert(forceDispatch < forceBarrier);
    assert(forceBarrier < positionPipeline);
    assert(positionPipeline < positionDispatch);
    assert(positionDispatch < transferBarrier);
    assert(transferBarrier < velocityCopy);
    assert(physicsSource.find("cmd.pipelineBarrier(", velocityCopy) == std::string::npos);

    const std::string barrierSource =
            physicsSource.substr(transferBarrier, velocityCopy - transferBarrier);
    const size_t computeStage =
            barrierSource.find("vk::PipelineStageFlagBits::eComputeShader");
    const size_t transferStage =
            barrierSource.find("vk::PipelineStageFlagBits::eTransfer");
    const size_t shaderWrite =
            barrierSource.find("vk::AccessFlagBits::eShaderWrite");
    const size_t transferRead =
            barrierSource.find("vk::AccessFlagBits::eTransferRead");

    assert(computeStage != std::string::npos);
    assert(transferStage != std::string::npos);
    assert(shaderWrite != std::string::npos);
    assert(transferRead != std::string::npos);
    assert(computeStage < transferStage);
    assert(shaderWrite < transferRead);

    const size_t copyEnd = physicsSource.find("writeTimestamp(cmd, PhysicsEnd)", velocityCopy);
    assert(copyEnd != std::string::npos);
    const std::string copySource =
            physicsSource.substr(velocityCopy, copyEnd - velocityCopy);
    const size_t copyBufferSource =
            copySource.find("particleVelocityBufferCopy.buf");
    const size_t copyBufferDestination =
            copySource.find("simulationState.particleVelocityBuffer.buf");

    assert(copyBufferSource != std::string::npos);
    assert(copyBufferDestination != std::string::npos);
    assert(copyBufferSource < copyBufferDestination);

    const size_t runFunction =
            physicsSource.find("vk::CommandBuffer ParticleSimulation::run");
    const size_t readbackCommentStart = physicsSource.find("/*", runFunction);
    const size_t readbackCommentEnd = physicsSource.find("*/", readbackCommentStart);
    const size_t firstReadback =
            physicsSource.find("fillHostWithStagingBuffer(");
    const size_t lastReadback =
            physicsSource.rfind("fillHostWithStagingBuffer(");

    assert(countOccurrences(physicsSource, "fillHostWithStagingBuffer(") == 3);
    assert(runFunction != std::string::npos);
    assert(readbackCommentStart != std::string::npos);
    assert(readbackCommentEnd != std::string::npos);
    assert(firstReadback != std::string::npos);
    assert(lastReadback != std::string::npos);
    assert(readbackCommentStart < firstReadback);
    assert(lastReadback < readbackCommentEnd);

    const size_t coordinateBufferSize =
            simulationStateSource.find("const vk::DeviceSize coordinateBufferSize");
    const size_t sceneTypeSwitch =
            simulationStateSource.find("switch (parameters.type)", coordinateBufferSize);
    assert(coordinateBufferSize != std::string::npos);
    assert(sceneTypeSwitch != std::string::npos);
    const std::string coordinateSizing =
            simulationStateSource.substr(
                    coordinateBufferSize,
                    sceneTypeSwitch - coordinateBufferSize);
    assert(coordinateSizing.find(
                   "particleVectorByteSize(parameters.type, parameters.numParticles)") !=
           std::string::npos);

    const size_t velocityValues =
            simulationStateSource.find("std::vector<float> velocityValues(");
    const size_t densityValues =
            simulationStateSource.find("std::vector<float> densityValues", velocityValues);
    assert(velocityValues != std::string::npos);
    assert(densityValues != std::string::npos);
    const std::string velocityInitialization =
            simulationStateSource.substr(
                    velocityValues,
                    densityValues - velocityValues);
    assert(velocityInitialization.find(
                   "particleVectorScalarCount(parameters.type, parameters.numParticles)") !=
           std::string::npos);
    assert(simulationStateSource.find(
                   "std::vector<float> velocityValues(parameters.numParticles, 0.0f)") ==
           std::string::npos);
}
