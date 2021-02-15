# Game Engine Performance

1. "*More computing sins are committed in the name of efficiency (without necessarily achieving it) than for any other single reason - including blind stupidity.*" --- William Wulf
2. "*On the other hand, we cannot ignore efficiency*." --- Jon Bentley

## C++ code optimization

1. [C++ Optimization Strategies and Techniques](https://www.tantalon.com/pete/cppopt/main.htm)
2. [Tips for Optimizing C/C++ Code](https://people.cs.clemson.edu/~dhouse/courses/405/papers/optimize.pdf)
3. 

## Render Performance

1. [Dos and donts](https://developer.nvidia.com/blog/vulkan-dos-donts/)
2. [Performance](https://zeux.io/2020/02/27/writing-an-efficient-vulkan-renderer/)
3. 


## Coding tips
1. To replace the name of a namespace, the easy way:

```bash
grep -rl "realEngine" . | xargs sed -i 's/realEngine/cruelEngine/g'
```