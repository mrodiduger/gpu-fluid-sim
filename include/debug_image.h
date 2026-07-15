#pragma once

#include "initialization.h"
#include <functional>
#include <string>

class DebugImage {
    vk::DescriptorImageInfo descriptorInfo;
    vk::DeviceMemory imageMemory;
    std::string name;

    void create();
    void release();

public:
    explicit DebugImage(std::string name);
    ~DebugImage();
    vk::Image image;
    vk::ImageView view;

    void resize();
    void clear(vk::CommandBuffer cmd, std::array<float, 4> color);
    vk::DescriptorSetLayoutBinding getLayout(uint32_t binding);
    vk::WriteDescriptorSet getWrite(vk::DescriptorSet set, uint32_t binding);
};
