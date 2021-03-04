#pragma once
#include "../vkcommon.h"

namespace cruelEngine {
    class Window;
namespace cruelRender {
    class Instance {
    public: 
        Instance(const VkApplicationInfo &_appInfo, const bool _validation,
            const std::vector<const char*> _validationLayers,
            const std::vector<const char*> _instanceExtensions);

        Instance(const Instance&) = delete;

        Instance(Instance &&) = delete;

        Instance operator=(const Instance&) = delete;

        Instance operator=(Instance&&) = delete;

        ~Instance(); 

        void            query_gpus();

        const std::vector<VkPhysicalDevice> &get_gpus() const {return gpus;}

        const VkInstance&       get_handle() const {return handle;}

    private:

        VkInstance      handle;
        void            createInstance();

        const VkApplicationInfo &appInfo;
        const bool      validation;
        const std::vector<const char*>
                        validationLayers;

        const std::vector<const char*>
                        requiredInstanceExtensions;
        std::vector<const char*> getRequiredInstanceExtensions();

    private:
        std::vector<VkPhysicalDevice> gpus;
    };
}
}