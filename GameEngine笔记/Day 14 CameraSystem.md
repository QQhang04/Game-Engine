### CameraSystem

#### ![image-20250706185451033](/Users/qqhang/Library/Application Support/typora-user-images/image-20250706185451033.png)

1.**通过 `SDL_RECT` to represent** a camera

在Game初始化时：

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250706185621786.png" alt="image-20250706185621786" style="zoom:50%;" />

2.**通过`CameraFollowComponent`决定Camera的follow对象**

这个component就像一个tag标记，不需要别的数据



3.**通过`CameraMovementSystem`来移动Camera**

在Game的Update中传入SDL_Rect& Camera

通过这个system根据标记了CameraFollowComponent的entity的transform来改变camera的Rect位置



4.重要！通过修改**RendererSystem的渲染逻辑**来实现实际的camera移动

camera向右移动  所有entity向左移动

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250706190537882.png" alt="image-20250706190537882" style="zoom:50%;" />

**注意**：HUD，UI等entity不需要跟随摄像机移动， 可以在SpriteComponent上增加一个isFixed的组件，特殊判断即可

**总而言之：在Rendersystem中把物体的世界坐标转换成相对相机的相对坐标**



其他润色：

static windowHeight，windowHeight，mapWidth，mapHeight

需要再cpp中的类外初始化



容易混淆 **static**：

static成员变量要在类外初始化

### 文件作用域和 static 的区别

你说的“静态类型的作用域是文件”，其实是指**加了 static 关键字的全局变量或函数**，它们的作用域才是文件级别（即只在当前 .cpp 文件可见）。

而类内的 static 成员变量，static 只是表示“属于类”，不是“文件私有”。

它们的作用域依然是类，只要你能访问到类声明，就能访问到它。



计算mapWidth，mapHeight

限制实际的camera移动

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250706191251897.png" alt="image-20250706191251897" style="zoom:50%;" />

 