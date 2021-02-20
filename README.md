# Cruel engine

## Components of a game engine
1. Entry point
2. Application Layer
3. Window Layer
4. Render
5. Render API
6. Debugging Support
7. Scripting Language
8. Memory Management
9. Entity component system (ECS)
10. Physics
11. File I/O Visual File System
12. Content creation

## Dependences
1. glfw3
2. vulkan SDK
3. glm
4. imgui

## Install dependences 
On archlinux see [this](https://wiki.archlinux.org/index.php/Vulkan#Installation)
```bash
sudo pacman -S glfw-x11 vulkan-icd-loader vulkan-headers vulkan-validation-layers vulkan-tools
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

## Todo

1. Render System
   1. Basic Render
   2. Render Template
   3. Dynamic loadable render model
2. Memory & Process Management
3. Application Template