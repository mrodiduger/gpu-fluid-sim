#include "mouse_stirring.h"

#include <cmath>

std::optional<glm::vec2> cursorToSimulationPlane(
        const glm::dvec2 &cursor,
        const glm::uvec2 &windowSize,
        const glm::mat4 &viewProjection) {
    if (windowSize.x == 0 || windowSize.y == 0) return std::nullopt;

    const glm::vec2 ndc {
            static_cast<float>(2.0 * cursor.x / windowSize.x - 1.0),
            static_cast<float>(1.0 - 2.0 * cursor.y / windowSize.y)};
    const glm::mat4 inverseViewProjection = glm::inverse(viewProjection);

    auto unproject = [&](float depth) -> std::optional<glm::vec3> {
        glm::vec4 point = inverseViewProjection * glm::vec4(ndc, depth, 1.0f);
        if (std::abs(point.w) < 1e-6f) return std::nullopt;
        return glm::vec3(point) / point.w;
    };

    const auto nearPoint = unproject(-1.0f);
    const auto farPoint = unproject(1.0f);
    if (!nearPoint || !farPoint) return std::nullopt;

    const glm::vec3 direction = *farPoint - *nearPoint;
    if (std::abs(direction.z) < 1e-6f) return std::nullopt;

    const float distanceToPlane = -nearPoint->z / direction.z;
    if (distanceToPlane < 0.0f) return std::nullopt;

    return glm::vec2(*nearPoint + direction * distanceToPlane);
}

bool mouseStirringEnabled(
        bool is2DScene,
        bool paused,
        bool leftButtonPressed,
        bool shiftPressed,
        bool uiCapturesMouse) {
    return is2DScene &&
           !paused &&
           leftButtonPressed &&
           !shiftPressed &&
           !uiCapturesMouse;
}

void MouseStirringAccumulator::add(const MouseStirringInput &input) {
    if (!input.active) return;

    pending.position = input.position;
    pending.drag += input.drag;
    pending.active = true;
}

MouseStirringInput MouseStirringAccumulator::consume() {
    const MouseStirringInput input = pending;
    pending = {};
    return input;
}

void MouseStirringAccumulator::clear() {
    pending = {};
}

MouseStirringInput MouseStirringTracker::update(
        const std::optional<glm::vec2> &position,
        bool enabled) {
    if (!enabled || !position.has_value()) {
        previousPosition.reset();
        return {};
    }

    MouseStirringInput input;
    input.position = *position;
    if (previousPosition.has_value()) {
        input.drag = *position - *previousPosition;
        input.active = glm::dot(input.drag, input.drag) > 0.0f;
    }
    previousPosition = position;
    return input;
}
