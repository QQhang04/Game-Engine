## DAY7 Architecture

### Game Object

stack：连续的内存(更快)，fixed size，只要超出作用域，栈内存自动回收

heap：no fixed size分配的需要手动释放



###### raw vs smart pointers



#### organize Gameobjects

良好的适配性：引擎load游戏database（代码、资源……）就能直接运行



### Component-based Design

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250613174720051.png" alt="image-20250613174720051" style="zoom:50%;" />

###### translate to code architecture:

![image-20250613175724958](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613175724958.png)

如render： game –》 render所有entity   每个entity –》render所有component



### ECS –––– data-oriented design

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250613202603473.png" alt="image-20250613202603473" style="zoom:50%;" />

entity-component-system

##### Entity: 只是一个id，代表着scene中的objects

##### Components：pure data（ECS中的核心）

- 在内存中连续，更易缓存，处理器访问效率更高

- 被自己organized而不是再被entity管理

传统架构——被entity管理（效率低）

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250613203202135.png" alt="image-20250613203202135" style="zoom:33%;" />

ECS：try to minimize **cache** miss

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250613203229316.png" alt="image-20250613203229316" style="zoom:33%;" />



##### System –––– logic that update the components

![image-20250613204642192](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613204642192.png)

eg.  MoveSystem

translate to code implement

**注意**：每个System都只在意特定的entity，比如move system只在乎有transform和velocity Component的entities

例如下图中system的GetEntities( )方法，就返回哪些有这两个组件的entity

![image-20250613204424118](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613204424118.png)

