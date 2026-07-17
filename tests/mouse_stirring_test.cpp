#include "mouse_stirring.h"

#include <cassert>
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>

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
