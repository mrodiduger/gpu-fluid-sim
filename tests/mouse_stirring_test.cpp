#include "mouse_stirring.h"

#include <cassert>
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

bool approximatelyEqual(float left, float right, float tolerance = 1e-5f) {
    return std::abs(left - right) <= tolerance;
}

void assertVector(const glm::vec2 &actual, const glm::vec2 &expected) {
    assert(approximatelyEqual(actual.x, expected.x));
    assert(approximatelyEqual(actual.y, expected.y));
}

int main() {
    const glm::mat4 viewProjection =
            glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);

    const auto bottomLeft = cursorToSimulationPlane(
            {0.0, 100.0}, {100, 100}, viewProjection);
    const auto center = cursorToSimulationPlane(
            {50.0, 50.0}, {100, 100}, viewProjection);
    const auto topRight = cursorToSimulationPlane(
            {100.0, 0.0}, {100, 100}, viewProjection);

    assert(bottomLeft.has_value());
    assert(center.has_value());
    assert(topRight.has_value());
    assertVector(*bottomLeft, {0.0f, 0.0f});
    assertVector(*center, {0.5f, 0.5f});
    assertVector(*topRight, {1.0f, 1.0f});
    assert(!cursorToSimulationPlane(
                    {0.0, 0.0}, {0, 100}, viewProjection)
                    .has_value());

    const glm::mat4 perspective =
            glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 10.0f);
    const glm::vec3 target {0.4f, 0.6f, 0.0f};
    const glm::mat4 perspectiveViewProjection =
            perspective * glm::lookAt(
                                  glm::vec3 {target.x, target.y, 0.15f},
                                  target,
                                  glm::vec3 {0.0f, 1.0f, 0.0f});
    const auto perspectiveCenter = cursorToSimulationPlane(
            {50.0, 50.0}, {100, 100}, perspectiveViewProjection);
    assert(perspectiveCenter.has_value());
    assertVector(*perspectiveCenter, glm::vec2(target));

    const glm::mat4 parallelViewProjection =
            perspective * glm::lookAt(
                                  glm::vec3 {0.0f, 0.0f, 0.15f},
                                  glm::vec3 {1.0f, 0.0f, 0.15f},
                                  glm::vec3 {0.0f, 0.0f, 1.0f});
    assert(!cursorToSimulationPlane(
                    {50.0, 50.0}, {100, 100}, parallelViewProjection)
                    .has_value());

    const glm::mat4 lookingAwayViewProjection =
            perspective * glm::lookAt(
                                  glm::vec3 {target.x, target.y, 0.15f},
                                  glm::vec3 {target.x, target.y, 1.15f},
                                  glm::vec3 {0.0f, 1.0f, 0.0f});
    assert(!cursorToSimulationPlane(
                    {50.0, 50.0}, {100, 100}, lookingAwayViewProjection)
                    .has_value());

    assert(mouseStirringEnabled(true, false, true, false, false));
    assert(!mouseStirringEnabled(false, false, true, false, false));
    assert(!mouseStirringEnabled(true, true, true, false, false));
    assert(!mouseStirringEnabled(true, false, false, false, false));
    assert(!mouseStirringEnabled(true, false, true, true, false));
    assert(!mouseStirringEnabled(true, false, true, false, true));

    MouseStirringTracker tracker;
    const auto first = tracker.update(glm::vec2 {0.2f, 0.3f}, true);
    assert(!first.active);
    assertVector(first.position, {0.2f, 0.3f});
    assertVector(first.drag, {0.0f, 0.0f});

    const auto dragged = tracker.update(glm::vec2 {0.25f, 0.28f}, true);
    assert(dragged.active);
    assertVector(dragged.position, {0.25f, 0.28f});
    assertVector(dragged.drag, {0.05f, -0.02f});

    const auto blocked = tracker.update(glm::vec2 {0.8f, 0.8f}, false);
    assert(!blocked.active);

    const auto restarted = tracker.update(glm::vec2 {0.8f, 0.8f}, true);
    assert(!restarted.active);
    assertVector(restarted.drag, {0.0f, 0.0f});
}
