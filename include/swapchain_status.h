#pragma once

#include <vulkan/vulkan.hpp>

constexpr bool requiresSwapchainRecreation(vk::Result result) {
    return result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR;
}
