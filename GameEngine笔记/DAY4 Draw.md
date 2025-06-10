## DAY4 Draw



#### FullScreen

##### Fake, still a window

![截屏2025-06-10 15.34.40](/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-10 15.34.40.png)

##### to change a video mode

![image-20250610153743227](/Users/qqhang/Library/Application Support/typora-user-images/image-20250610153743227.png)



##### Fake Full Screen（定死window的width和height） VS Real（根据分辨率来）

像素风：fake 不论分辨率大小，呈现出的范围都是一致的

real：分辨率越大，呈现的范围越大





#### SDL_Rect: (x, y, h, w)

#### <img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-10 18.46.46.png" alt="截屏2025-06-10 18.46.46" style="zoom:50%;" />

![image-20250610190711792](/Users/qqhang/Library/Application Support/typora-user-images/image-20250610190711792.png)



#### Double Buffer Render

![image-20250610192142604](/Users/qqhang/Library/Application Support/typora-user-images/image-20250610192142604.png)

SDL works in double buffers

<img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-10 19.22.11.png" alt="截屏2025-06-10 19.22.11" style="zoom:50%;" />

 SDL 的双缓冲（Double Buffering）机制：

1. **两个缓冲区**
   - 前缓冲区（Front Buffer）：当前显示的内容
   - 后缓冲区（Back Buffer）：正在绘制的内容

2. **工作流程**
```cpp
// 1. 在后缓冲区绘制
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 设置颜色
SDL_RenderClear(renderer);                         // 清空后缓冲区
// ... 绘制游戏内容 ...

// 2. 交换缓冲区
SDL_RenderPresent(renderer);  // 将后缓冲区内容显示到屏幕
```

##### 为什么需要双缓冲？

1. **避免画面撕裂（Screen Tearing）**
   - 单缓冲时：绘制过程中屏幕会显示未完成的画面
   - 双缓冲时：只在绘制完成后才显示，保证画面完整

2. **提高渲染效率**
   - 单缓冲：绘制和显示同时进行，可能造成冲突
   - 双缓冲：绘制和显示分离，更高效

3. **实现平滑动画**
   - 单缓冲：动画可能不流畅
   - 双缓冲：保证每一帧都是完整的

##### 实际例子
假设要绘制一个移动的物体：

```cpp
// 单缓冲（不推荐）
void Render() {
    // 直接绘制到屏幕
    DrawBackground();    // 用户可能看到未完成的背景
    DrawCharacter();     // 用户可能看到未完成的角色
    DrawUI();           // 用户可能看到未完成的UI
}

// 双缓冲（推荐）
void Render() {
    // 1. 在后缓冲区绘制
    SDL_RenderClear(renderer);    // 清空后缓冲区
    DrawBackground();             // 绘制背景
    DrawCharacter();              // 绘制角色
    DrawUI();                     // 绘制UI
    
    // 2. 完成后一次性显示
    SDL_RenderPresent(renderer);  // 交换缓冲区
}
```



#### Render PNG

![image-20250610201625753](/Users/qqhang/Library/Application Support/typora-user-images/image-20250610201625753.png)

RenderCopy: 第三个参数NULL: 用png全部的像素; 第四个参数: 目标位置及大小

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250610202525487.png" alt="image-20250610202525487" style="zoom:50%;" />

<img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-10 20.17.33.png" alt="截屏2025-06-10 20.17.33" style="zoom:50%;" />

###### 1. 为什么需要 Surface 到 Texture 的转换？
1. **Surface 特点**
   - 存储在系统内存中
   - 包含完整的像素数据
   - 适合图像加载和修改
   - 但渲染速度较慢

2. **Texture 特点**
   - 存储在显卡内存中
   - 经过优化，渲染速度快
   - 适合频繁绘制
   - 但创建和修改成本高

3. **转换过程**
   - Surface 用于加载和初始处理图像
   - Texture 用于实际渲染
   - 这是 SDL 的设计模式

###### 2. 为什么需要 Free 和 Destroy？
```cpp
SDL_FreeSurface(surface);
SDL_DestroyTexture(texture);
```
原因：
1. **内存管理**
   - 防止内存泄漏
   - 释放系统资源
   - 特别是 Texture 占用显卡内存
2. **最佳实践**
   - 每次渲染后释放 Texture
   - 避免重复创建 Texture
###### 3. SDL_RenderCopy 的作用
```cpp
SDL_RenderCopy(renderer, texture, NULL, &dstRect);
```
参数说明：
1. `renderer`: 渲染器
2. `texture`: 要绘制的纹理
3. `NULL`: 源矩形（NULL 表示使用整个纹理）
4. `dstRect`: 目标矩形（在屏幕上的位置和大小）
