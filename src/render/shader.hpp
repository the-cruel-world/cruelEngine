#pragma once
#include "../common.h"
#include "../types.h"

#include "../debug/debug.hpp"


namespace realEngine {

    std::vector<char> readFile(const std::string& filename);

    VkShaderModule createShaderModule(const VkDevice logicalDevice, const std::vector<char>& code);

    class Shader {
    public:
        Shader (const VkDevice &_device, const std::string& _filename)
            : device (_device)
        {
            const std::vector<char> &buffer = readFile(_filename);
            shader =  createShaderModule(device, buffer);
            //std::cout << "shader " + _filename + " loaded!" << std::endl;
        }
        virtual ~Shader (){
            //std::cout << "shader released!" << std::endl;
            vkDestroyShaderModule(device, shader, nullptr);
        }

        VkShaderModule      shader;
    private: 
        const VkDevice      &device;
    };
}