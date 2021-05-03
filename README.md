# Cruel engine

Cruel Engine is a game engine. It is in the early development stage.

It will be a performance efficient, easy to use engine.



## Platform 

* Linux
* FreeBSD

## Dependences
1. glfw3
2. vulkan SDK
3. glm
4. imgui
5. tinygltf

Download these dependencies with the following command:

```bash
git submodule init && git submodule update
```

## Install Vulkan Drivers
On archlinux see [this](https://wiki.archlinux.org/index.php/Vulkan#Installation)
```bash
sudo pacman -S vulkan-icd-loader vulkan-headers vulkan-validation-layers vulkan-tools
```
Vulkan Driver
```bash 
sudo pacman -S vulkan-intel #(or lib32-vulkan-intel)
sudo pacman -S nvidia-utils #(or lib32-nvidia-utils)
sudo pacman -S vulkan-radeon
```

## Build
```bash
git clone https://github.com/yiwenxue/cruel.git
cd cruel/
mkdir build
cd build
cmake ../
make 
```

Then you can run some executables like ./editor/editor

## TODO

1. Render System
   1. Basic Render
   2. Render Template
   3. Dynamic loadable render model
2. Memory & Process Management
3. Terrain editor
4. Scene Editor