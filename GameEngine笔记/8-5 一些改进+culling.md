## Easy Scripting

- kill all the entites that outside the **map**(not the cam)

at MovementSystem

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250805094609750.png" alt="image-20250805094609750" style="zoom:50%;" />



- 给移动的物体增加碰撞体,碰到之后反转方向移动

  ![image-20250805100026941](/Users/qqhang/Library/Application Support/typora-user-images/image-20250805100026941.png)

- player限制在map中

- **culling** cam之外的物体 

  在RenderSystem中, update前提前剔除

  ![image-20250805114743422](/Users/qqhang/Library/Application Support/typora-user-images/image-20250805114743422.png)

  改进: 注意要剔除判断时还要考虑到sprite width