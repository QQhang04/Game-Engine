## Day 7.1 ECS

### Folder Structure

#### SRC

​	ECS：core / clients(实际游戏逻辑) `ECS.h`中包含component/entity/system/Registry类定义

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250613214055629.png" alt="image-20250613214055629" style="zoom:50%;" />

​	Components：

​		如transformComponent

​	Systems: 实际每帧运行的逻辑

​		如MovementSystem

<img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-13 21.35.01.png" alt="截屏2025-06-13 21.35.01" style="zoom:50%;" />

在Game.cpp中如何调用？ **// TODO**

![image-20250613213703669](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613213703669.png)



![image-20250613213744748](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613213744748.png)

![image-20250613213851386](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613213851386.png)



### Component Signature —— link the entity to system

system通过bitset数组**signature**知道32个位0/1来知道需要什么component

<img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-13 21.48.20.png" alt="截屏2025-06-13 21.48.20" style="zoom:50%;" />

to identify which component: add `id` property to Component 



