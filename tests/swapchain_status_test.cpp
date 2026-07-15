#include "swapchain_status.h"

#include <cassert>

int main() {
    assert(!requiresSwapchainRecreation(vk::Result::eSuccess));
    assert(requiresSwapchainRecreation(vk::Result::eSuboptimalKHR));
    assert(requiresSwapchainRecreation(vk::Result::eErrorOutOfDateKHR));
}
