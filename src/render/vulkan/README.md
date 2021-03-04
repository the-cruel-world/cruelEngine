# Vulkan Backend

  - [x] Device select & creation
  
    - [x] Physical device select
    - [x] Logical device creation & destroy
  
  - [x] Swap Chain

    - [x] Images & Image View

  - [x] Render Pass

  - [x] Graphics PipeLine

  - [x] Frame Buffer



## Vulkan Command Buffer

```C++
void 			begin(); // Start to record drawing command 

void 			end(); // The recording of drawing command ends. 

void 			begin_renderpass(const VkRenderPass &renderPass, const VkFrameBuffer &frameBuffer, const VkExtent2D &extent);

void 			end_renderpass();
```

Let the program to define their own draw command. 

This is an example on how to record a drawing command (example Triangle):

```C++
commandBuffers->begin();

commandBuffers->begin_renderpass(renderPass.get_handle(), frameBuffers->get_handle(), swapChain.get_extent());

vkCmdBindPipeline(commandBuffers->get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine.get_handle()); // There is another function to replace this vk function

vkCmdDraw(commandBuffers->get_handle(), 3, 1, 0, 0);

commandBuffers->end_renderpass();

commandBuffers->end();
```

## PipeLine

PipeLine also needs much customization. So the `PipeLine` will allow you to design your own way.

```C++
void createPipeLine(const VkGraphicsPipelineCreateInfo &pipelineInfo);
```

Compute pipeline will be supported in the future.

## Issues 

- [ ] The later usage of an **instance** should be clear that the instance can be created with the proper instance extensions and validation layers.



## TODO

- [x] use c++ smart pointers

- [ ] Swapchain
  - [ ] Update or Recreate
  - [ ] create swapchain image
  - [ ] swapchain format proorities

- [ ] Device
  - [x] Separate Physical and Logical Devices
  - [ ] Physical Device
    - [ ] list all supported extensions of this physical device
  - [ ] Logical Device
    - [x] create all supported queues by default.
    - [ ] Manage Pipeline and Buffer resources

- [ ] Render Pass & subPass 
  - [ ] It was said that the use of subpass can improve the performance of render heavily.

- [ ] Render Context

- [ ] Render Sessions
  - [ ] Swapchain
  - [ ] Render tasks

- [ ] Render Tasks