## Day 13 Collision



do I need BETTER Collison?

根据实际情况分析，平衡performance和precisely

pixel collision/mesh collision/box collision……



#### BoxColliderComponent

width/height/offset



#### Collision System

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250702095616016.png" alt="image-20250702095616016" style="zoom:50%;" />

嵌套for循环检测（每个entitiy都要和其他的所有entity检测）性能很差，相当于这一次检查A是否撞B，下一次会重复检查B是否撞A

![image-20250702102559073](/Users/qqhang/Library/Application Support/typora-user-images/image-20250702102559073.png)

![image-20250702100859512](/Users/qqhang/Library/Application Support/typora-user-images/image-20250702100859512.png)

use **AABB**包围盒检测

核心思想：检测x轴和y轴是否都同时有覆盖

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250702101825758.png" alt="image-20250702101825758" style="zoom:50%;" />

![image-20250702102928979](/Users/qqhang/Library/Application Support/typora-user-images/image-20250702102928979.png)



#### Render collision System(to debug)

press D to show debug

###### Game

bool isDeubg: changed by ProcessInput()

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250702114922647.png" alt="image-20250702114922647" style="zoom:50%;" />

###### RenderColliderSystem

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250702115030135.png" alt="image-20250702115030135" style="zoom:50%;" />