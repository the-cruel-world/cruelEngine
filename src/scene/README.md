# Sence

The object that includes all materials, objects.

Sence is the core part for this engine. 

A sence will be inited at the beginning. New objects will be loaded 
into sence through methods provided by sence. The sence will be evaluated through 
physics engine. At the end of a loop, the renderer will render a new frame, and present it 
to screen.

~~How an object will be rendered will be defined by the object itself. Because some times, special objects may have their own shader etc. Design a general render pipeline and renderpass might be difficult. ~~

We can classify the render object into several types and define the render tasks correspondingly. Then all the dependencies of render can be removed from scene. 

## Camera

* Fundamentals of Real-Time Camera Design
* smooth near clipping plane

## Scene Graph

## Shape

