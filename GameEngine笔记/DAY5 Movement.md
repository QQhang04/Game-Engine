# DAY5 Movement

```cpp
#include <glm/glm.hpp> // math library
```

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250611195103052.png" alt="image-20250611195103052" style="zoom:50%;" />

![image-20250611195220493](/Users/qqhang/Library/Application Support/typora-user-images/image-20250611195220493.png)

- static_cast：

- 在编译时进行类型检查

- 只能用于相关类型之间的转换

- 如果转换不安全，编译器会报错





### Fixing Time Step

##### Move, Simulate Physics, Collision => 保证不同速度处理器运行结果正常

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250611200755632.png" alt="image-20250611200755632" style="zoom:50%;" />

SDL_GetTicks( ): 从sdl init到现在经过了多久

如果太快，就等着while阻塞

![image-20250611201217267](/Users/qqhang/Library/Application Support/typora-user-images/image-20250611201217267.png)

缺点： cpu占用率99% while一直在运作

##### delay funtion： SDL_Delay

能让处理器运行别的进程，等待指定时间后再回来

![image-20250611203514335](/Users/qqhang/Library/Application Support/typora-user-images/image-20250611203514335.png)



### Delta Time

—— 让运动统一

在上面的方法中，Update中的移动于每帧发生

60帧和120帧的移动速度不同！

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250611203855718.png" alt="image-20250611203855718" style="zoom:50%;" />

关联帧 错！–》 关联时间 对！

**一帧**动多少 –》**一秒**动多少

![image-20250611204001132](/Users/qqhang/Library/Application Support/typora-user-images/image-20250611204001132.png)

![image-20250611204758589](/Users/qqhang/Library/Application Support/typora-user-images/image-20250611204758589.png)

##### 最终实现

![image-20250611205117660](/Users/qqhang/Library/Application Support/typora-user-images/image-20250611205117660.png)