# Render

The render should support both Vulcan and OpenGL as backed.
So a proper render API is required.



## Renderer Design Pattern

## Render API

* Render Context

  It's the context of a Vulcan instance for rendering usage. It's a necessary part for an application to render something on GPU.  It will create an Vulcan instance, enumerate  all GPU devices, and pick the proper once if possible. Of course, you can also pick the best GPU manually. 

* Render Session

* Render Tasks

* GUI Overlay

* Resource Cache

## Light & Shadow

* 