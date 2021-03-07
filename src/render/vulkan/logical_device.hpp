#pragma once
#include "../vkcommon.h"
#include "queue.hpp"
#include "resource_cache.hpp"

namespace cruelEngine {
namespace cruelRender {

class PhysicalDevice;
class CommandPool;
class CommandBuffer;
class FrameBuffer;
class Queue;
class ResourceCache;

class LogicalDevice {
public:
  /*! \brief LogicalDevice is the data prototype of a logical Device.
LogicalDevice creates objects that contain a logical device, and all
it's informations, operations, etc. \param _physicalDevice is the gpu
that the logical device will be linked with. \param _layerNames, are the
validation layers the enabled in the instance. \param
_requiredExtensions are the device extensions that the device should
enable. \param _flags are types of queue that this device should
support.
*/
  LogicalDevice(const PhysicalDevice &_physicalDevice,
                std::vector<const char *> &_layerNames,
                std::vector<const char *> &_requiredExtensions,
                const VkQueueFlags &_flags);

  LogicalDevice(const LogicalDevice &) = delete;

  LogicalDevice(LogicalDevice &&) = delete;

  LogicalDevice operator=(const LogicalDevice &) = delete;

  LogicalDevice operator=(LogicalDevice &&) = delete;

  ~LogicalDevice();

public:
  void createDevice();

  void destroyLogicalDevice();

  VkFramebuffer &createFrameBuffer();

  const VkDevice &get_handle() const { return handle; };

  const PhysicalDevice &get_physicalDevice() const { return physicalDevice; }

  bool is_surface_supported(const VkSurfaceKHR &surface);

  /*! \brief Get a queue from the device by types. */
  Queue &get_queue_by_flags(VkQueueFlags queue_flags, u32 queue_index);

  /*! \brief Get a proper graphics queue from the device. */
  Queue &get_suitable_graphics_queue(u32 queue_index);

  Queue &get_suitable_present_queue(const VkSurfaceKHR &surface,
                                    u32 queue_index);

  std::vector<Queue> &get_queues();

  /*! \brief Get the reference to the commandPool. */
  void get_commanfPool(){};

  /*! \brief Request a buffer with memory from the device. */
  void request_buffer(){};

  // copy_buffer();

  /*! \brief Request a CommandBuffer with memory from the device. */
  CommandBuffer &request_commandBuffer(const VkCommandBufferLevel &_level);

  // request_frameBuffer();

  // request_Fence();

  /*! \brief Wait for the device the finish the task it is running. */
  VkResult wait_idle();

private:
  const PhysicalDevice &physicalDevice;

  VkDevice handle;

  std::vector<const char *> &layerNames;

  std::vector<const char *> &requiredExtensions;

  //! The required queue family flags
  const VkQueueFlags &flags;

  //! All supported device queue
  std::vector<Queue> queues{};

  //! command pool
  std::unique_ptr<CommandPool> commandPool;

  //! RenderCache
  ResourceCache resource_cache;

  void query_queues();
};
} // namespace cruelRender
} // namespace cruelEngine