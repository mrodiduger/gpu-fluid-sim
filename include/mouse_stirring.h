#pragma once

#include <glm/glm.hpp>
#include <optional>

struct MouseStirringInput {
    glm::vec2 position {0.0f};
    glm::vec2 drag {0.0f};
    bool active = false;
};

std::optional<glm::vec2> cursorToSimulationPlane(
        const glm::dvec2 &cursor,
        const glm::uvec2 &windowSize,
        const glm::mat4 &viewProjection);

bool mouseStirringEnabled(
        bool is2DScene,
        bool paused,
        bool leftButtonPressed,
        bool shiftPressed,
        bool uiCapturesMouse);

class MouseStirringTracker {
public:
    MouseStirringInput update(
            const std::optional<glm::vec2> &position,
            bool enabled);

private:
    std::optional<glm::vec2> previousPosition;
};
