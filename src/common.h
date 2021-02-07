#ifndef __real_engine_common__
#define __real_engine_common__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <numeric>
#include <ctime>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <set>
#include <unistd.h>
#include <random>
#include <algorithm>
#include <sys/stat.h>

// headers of vulkan
#include <vulkan/vulkan.h>

#ifdef __VULKAN__
#define GLFW_INCLUDE_VULKAN
#endif 

// headers of GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <numeric>
#include <array>
#include <vector>

#endif