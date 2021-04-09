#pragma once

typedef struct Extent3D
{
    float width;
    float height;
    float depth;
} Extent3D;

typedef struct Extent2D
{
    float width;
    float height;
} Extent2D;

typedef enum RenderBackendFlag
{
    RenderBackend_Vulkan = 0,
    RenderBackend_D3D,
} RenderBackendFlag;

typedef enum SceneComponentTypeFlags
{
    SceneComponentType_Mesh,
    SceneComponentType_Material,
    SceneComponentType_Light,
} SceneComponentTypeFlags;

typedef enum SceneMaterialTypeFlags
{

} SceneMaterialTypeFlags;