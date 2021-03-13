#include "logical_device.hpp"

#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "frame_buffer.hpp"
#include "physical_device.hpp"
#include "queue.hpp"

namespace cruelEngine {
namespace cruelRender {
LogicalDevice::LogicalDevice(const PhysicalDevice &_physicalDevice,
                             std::vector<const char *> &_layerNames,
                             std::vector<const char *> &_requiredExtensions,
                             const VkQueueFlags &_flags)
    : physicalDevice(_physicalDevice), layerNames(_layerNames),
      requiredExtensions(_requiredExtensions),
      flags(_flags), resource_cache{*this} {
  // clock_t start_time = clock();
  createDevice();
  // std::cout << "[logical] device created. time: " << (clock() - start_time) *
  // 1e-5  << "ms" << std::endl; start_time = clock();
  query_queues();
  // std::cout << "[logical] queue pool created. time: " << (clock() -
  // start_time) * 1e-5  << "ms" << std::endl;
  commandPool = std::make_unique<CommandPool>(*this);
}

LogicalDevice::~LogicalDevice() {
  commandPool.reset();
  // std::cout << "command Pool deleted" << std::endl;
  queues.clear();
  // std::cout << "command Pool deleted" << std::endl;
  if (handle != VK_NULL_HANDLE)
    vkDestroyDevice(handle, nullptr);
  std::cout << "Logical device destroied" << std::endl;
}

void LogicalDevice::createDevice() {
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.get_handle(),
                                           &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
      physicalDevice.get_handle(), &queueFamilyCount, queueFamilies.data());

  std::vector<std::vector<float>> queuePriorities;
  queuePriorities.resize(queueFamilyCount);
  for (size_t family_index = 0; family_index < queueFamilyCount;
       family_index++) {
    queuePriorities[family_index].resize(queueFamilies[family_index].queueCount,
                                         1.0f);
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = family_index;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriorities[family_index][0];

    queueCreateInfos.push_back(queueInfo);
  }

  VkDeviceCreateInfo deviceCreateInfo = {};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.queueCreateInfoCount =
      static_cast<u32>(queueCreateInfos.size());
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
  deviceCreateInfo.pEnabledFeatures = &physicalDevice.getRequiredFeatures();

  deviceCreateInfo.enabledExtensionCount =
      static_cast<u32>(requiredExtensions.size());
  deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

  if (layerNames.size()) {
    deviceCreateInfo.enabledLayerCount = static_cast<u32>(layerNames.size());
    deviceCreateInfo.ppEnabledLayerNames = layerNames.data();
  } else
    deviceCreateInfo.enabledLayerCount = 0;

  VK_CHECK_RESULT(vkCreateDevice(physicalDevice.get_handle(), &deviceCreateInfo,
                                 nullptr, &handle));
}

// VkFramebuffer &LogicalDevice::createFrameBuffer() {
//   VkFramebuffer framebuffer = VK_NULL_HANDLE;
//   return framebuffer;
// }

void LogicalDevice::query_queues() {
  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.get_handle(),
                                           &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
      physicalDevice.get_handle(), &queueFamilyCount, queueFamilies.data());

  for (size_t family_index = 0; family_index < queueFamilyCount;
       family_index++) {
    std::cout << "Queue Family [" << family_index
              << "]: " << queueFamilies[family_index].queueCount << std::endl;
    if (queueFamilies[family_index].queueCount > 0) {
      for (size_t queue_index = 0;
           queue_index < queueFamilies[family_index].queueCount &&
           queue_index < 1;
           queue_index++) {
        queues.emplace_back(*this, family_index, queue_index,
                            queueFamilies[family_index]);
      }
    }
  }
}

Queue &LogicalDevice::get_queue_by_flags(VkQueueFlags queue_flags,
                                         u32 queue_index) {
  for (auto &queue : queues) {
    if (queue.get_properties().queueFlags & queue_flags &&
        queue.get_index() == queue_index)
      return queue;
  }
  for (auto &queue : queues) {
    if (queue.get_properties().queueFlags & queue_flags)
      return queue;
  }
  throw std::runtime_error("No suitable queue found.");
}

bool LogicalDevice::is_surface_supported(const VkSurfaceKHR &surface) {
  for (auto &queue : queues) {
    if (queue.can_present(surface) == VK_TRUE)
      return true;
  }
  return false;
}

Queue &LogicalDevice::get_suitable_graphics_queue(u32 queue_index) {
  return get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, queue_index);
}

Queue &LogicalDevice::get_suitable_present_queue(const VkSurfaceKHR &surface,
                                                 u32 queue_index) {
  for (auto &queue : queues) {
    if (queue.can_present(surface) == VK_TRUE &&
        queue.get_index() == queue_index)
      return queue;
  }
  for (auto &queue : queues) {
    if (queue.can_present(surface) == VK_TRUE)
      return queue;
  }
  throw std::runtime_error("No suitable present queue found");
}

CommandBuffer &
LogicalDevice::request_commandBuffer(const VkCommandBufferLevel &_level) {
  return commandPool->request_commandbuffer(_level);
}
} // namespace cruelRender
} // namespace cruelEngine