#include "resource_cache.hpp"

#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
ResourceCache::ResourceCache(LogicalDevice &device) : device{device}
{}

ResourceCache::~ResourceCache()
{}
} // namespace cruelRender
} // namespace cruelEngine