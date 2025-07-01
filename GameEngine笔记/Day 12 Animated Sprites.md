## Animated Sprites

![image-20250701195146689](/Users/qqhang/Library/Application Support/typora-user-images/image-20250701195146689.png)

##### Animation Component

![image-20250701195605865](/Users/qqhang/Library/Application Support/typora-user-images/image-20250701195605865.png)



timeScale



##### Animation System

![截屏2025-07-01 20.31.42](/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-07-01 20.31.42.png)

cal current frame:

```cpp
(花掉的事件ms * speed（默认为1） / 1000)s % texture切片的数量
```

