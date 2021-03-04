#pragma once
#include "../vkcommon.h"


namespace cruelEngine {
namespace cruelRender {

    class LogicalDevice;

    class CommandBuffer;
    
    uint32_t    findMemoryType(const VkPhysicalDevice &device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void        copyBuffer(const VkDevice &device, const VkQueue &queue, const VkCommandPool &cmdPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    
    void        createBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory); 

    class Buffer {
    public:
        Buffer(LogicalDevice &device);

        Buffer(LogicalDevice &device, VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags memoryProperties);

        Buffer(const Buffer &) = delete;

        Buffer(Buffer &&);

        Buffer &operator=(const Buffer &) = delete;

        Buffer &operator=(Buffer &&) = delete;
        
        ~Buffer();

        void            createBuffer(VkDeviceSize _bufferSize, VkBufferUsageFlags _bufferUsage, VkMemoryPropertyFlags _memoryProperties);
        
        void            destroyBuffer();
        
        void            load (void *data);
        
        void            load_stage (void *data, const VkQueue &queue, const VkCommandPool &cmdPool);

        const VkBuffer&     get_handle() const {return handle;}
        
        const VkDeviceMemory& get_handleMem() const {return memory;}
        
        const LogicalDevice& get_device() const {return device;} 
    private:
        LogicalDevice               &device;

        VkDeviceSize                bufferSize = 0;
        
        VkBuffer                    handle = VK_NULL_HANDLE;
        
        VkDeviceMemory              memory = VK_NULL_HANDLE;
    };
    class UniformBuffer {

    };
}
}