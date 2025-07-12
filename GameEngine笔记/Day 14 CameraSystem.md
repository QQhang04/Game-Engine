### CameraSystem

#### ![image-20250706185451033](/Users/qqhang/Library/Application Support/typora-user-images/image-20250706185451033.png)

1.**通过 `SDL_RECT` to represent**

在Game初始化时：

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250706185621786.png" alt="image-20250706185621786" style="zoom:50%;" />

2.**通过`CameraFollowComponent`决定Camera的follow对象**

这个component就像一个tag标记，不需要别的数据



3.**通过`CameraMovementSystem`来移动Camera**

在Game的Update中传入SDL_Rect& Camera



4.重要！通过修改**RendererSystem的渲染逻辑**来实现实际的camera移动

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250706190537882.png" alt="image-20250706190537882" style="zoom:50%;" />

其他润色：

static windowHeight



计算mapWidth，mapHeight

限制实际的camera移动

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250706191251897.png" alt="image-20250706191251897" style="zoom:50%;" />