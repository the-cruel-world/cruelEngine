#pragma once
#include <string>
#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(stat)                                                  \
  do {                                                                         \
    VkResult res = (stat);                                                     \
    if (res != VK_SUCCESS) {                                                   \
      fprintf(stderr, "Fatal: vKResult is \"%s\" in %s at line %d\n",          \
              errorString(res).c_str(), __FILE__, __LINE__);                   \
      exit(-1);                                                                \
    }                                                                          \
  } while (0)

std::string errorString(VkResult errorCode);