### Day 10 implement Some Systems



#### 1.Movement System



![image-20250626200749971](/Users/qqhang/Library/Application Support/typora-user-images/image-20250626200749971.png)



get all entities that have transform component & …component

update their components



接收deltaTime进行计算





#### 2.RenderSystem

##### 2.1SpriteComponent

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250628113507654.png" alt="image-20250628113507654" style="zoom:50%;" />



##### 2.2 调用RenderSystem：

##### <img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250628113820411.png" alt="image-20250628113820411" style="zoom:50%;" />



##### 2.3 RenderSystem：

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250628113918025.png" alt="image-20250628113918025" style="zoom:50%;" />



疑问：component、entityt的enable和disable



