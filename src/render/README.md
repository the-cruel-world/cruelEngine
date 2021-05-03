# Render

---

The render should support both Vulcan and (OK, just Vulkan) as backed.
So a proper render API is required.



Currently, the renderAPI is sooooooooooooooooooooooooooooooooooooooo close to the KronosGroup's framework.

## Renderer Design Pattern

## Render API

* Render Context

  ---

  It's the context of a Vulcan instance for rendering usage. It's a necessary part for an application to render something on GPU.  It will create an Vulcan instance, enumerate  all GPU devices, and pick the proper once if possible. Of course, you can also pick the best GPU manually. 

* Render Session

  ---

  1. Render Passes should be on the higher level to support the multi-pass rendering
  2. There should be some mechanism to manage multiple render passes.
  3. Multi-subpasses supports are handled by RenderPipeline, a pipeline which is different from the low-level shader pipeline.
  4. RenderFrames are connected with swapchain images (by index).

* RenderSubpass

  ---

  RenderSubpass is a collection of informations to create a renderpass and the render process for this renderpass. It was to say, all draw command (gui not included) will be handled by RenderSubpass. 

  1. RenderSubpass takes input attachments as input to draw.
  2. RenderSubpass draw to specific attachments (in render targets).
  3. RenderSubpass records all draw commands.

* Render Pipeline

  ---

  RenderPipeline is a collection of subpasses for a rendering process. The multi-pass render are handled by RenderPipeline. It takes the informations of all subpasses as a input. On drawing, it requests a render pass from ResourceCache, and use this renderpass to record draw commands. 

  1. It contains all information of subpasses.
  2. It can invoke renderpasses from ResourceCache with subpassInfos (It will not maintain renderpass by it self).
  3. The order of subpasses are important.

* Render Frame

  ---

  Render Frame is a set of images, imageviews, fences and semaphores. It's a collection of objects that needed for render images.
  1. It should support render to texture (image)
  2. It should support render to swapchain images.
  3. The RenderFrame contains a RenderTarget, which contains many images and imageViews.
  4. The RenderFrame doesn't contains any framebuffers, instead, it only request framebuffers from resource cache when necessary.

* RenderTarget

  ---

  RenderTarget is a set of images, imageViews. It's a collection of images that needed for render passes.

  1. It contains images and it's imageViews (An image view can be used by many images).
  2. RenderTarget should contains the properties of images as attachments.
  3. The order of imageViews are important for render subpasses.

* Render Tasks --- deprecated
  
  ---

  A batch of rasks to render the objects in the scene.
  
* Resource Cache

  ---

  1. Resources in Cache should have the `life time` and the `last access time`. This can help to manage vRam.
  2. To reuse the resources, resources should be marked with a special mark: hash tag. All resources are put in a hash table, and the hash tag are calculated based on their creation parameters.

* GUI Overlay

  ---

  Regular GUI will be drawn after the RenderPipeline, but for 3D Gui, it should be able to put it into the scene (curse ray tracing is required). 

