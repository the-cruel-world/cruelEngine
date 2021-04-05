#pragma once
#include <string>
#include <vulkan/vulkan.h>

#define C_DEFAULT "\033[0m"
#define C_RED "\033[0;31m"
#define C_GREEN "\033[0;32m"
#define C_YELLOW "\033[0;33m"
#define C_BLUE "\033[0;34m"
#define C_MAGENTA "\033[0;35m"

#define C_BOLDRED "\033[1;31m"
#define C_BOLDGREEN "\033[1;32m"
#define C_BOLDYELLOW "\033[1;33m"
#define C_BOLDBLUE "\033[1;34m"
#define C_BOLDMAGENTA "\033[1;35m"

#ifndef __FILENAME__
#    define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define VK_CHECK_RESULT(stat)                                               \
    do                                                                      \
    {                                                                       \
        VkResult res = (stat);                                              \
        if (res != VK_SUCCESS)                                              \
        {                                                                   \
            fprintf(stderr, "Fatal: vKResult is \"%s\" in %s at line %d\n", \
                    errorString(res).c_str(), __FILE__, __LINE__);          \
            exit(-1);                                                       \
        }                                                                   \
    } while (0)

std::string errorString(VkResult errorCode);

#ifdef RENDER_DEBUG
#    define RENDER_LOG(fmt, ...)                                                                \
        do                                                                                      \
        {                                                                                       \
            fprintf(stderr, C_BOLDMAGENTA "%s:%d:%s(): " C_DEFAULT fmt, __FILENAME__, __LINE__, \
                    __func__, __VA_ARGS__);                                                     \
        } while (0)
#else
#    define RENDER_LOG(fmt, ...) \
        do                       \
        {                        \
        } while (0)
#endif