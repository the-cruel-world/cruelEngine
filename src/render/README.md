# Render

The render should support both Vulcan and OpenGL as backed.
So a proper render API is required.



## Renderer Design Pattern

## Render API

* Render Context

  It's the context of a Vulcan instance for rendering usage. It's a necessary part for an application to render something on GPU.  It will create an Vulcan instance, enumerate  all GPU devices, and pick the proper once if possible. Of course, you can also pick the best GPU manually. 

* Render Session

  1. Render Passes should be on the higher level to support the multi-pass rendering
  2. There should be some mechanism to manage multiple render passes.

* Render Frame

  Render Frame is a set of images, imageviews and framebuffers. It's a collection of objects that needed for render images.
  1. It should support render to texture (image)
  2. It should support render to swapchain images.
  3. The render Frame should include many images and image views sas framebuffer supports multiple imageviews.

* Render Tasks
  
  a batch of rasks to render the objects in the scene.

* GUI Overlay

* Resource Cache

## Light & Shadow

* 