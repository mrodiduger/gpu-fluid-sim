#include "visual_scale.h"

#include <cassert>

int main() {
    static_assert(VISUAL_SCALE == 1.5f);
    assert(scaleVisualSize(6.0f) == 9.0f);
    assert(scaleVisualSize(120.0f) == 180.0f);
}
