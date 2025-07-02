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

### 时间轴示例（3帧动画，1倍速）：
```
时间(秒):  0    1    2    3    4    5    6
帧索引:    0 -> 1 -> 2 -> 0 -> 1 -> 2 -> 0
```

### 速度影响示例（3帧动画，2倍速）：
```
时间(秒):  0   0.5   1   1.5   2   2.5   3
帧索引:    0 -> 1 -> 2 -> 0 -> 1 -> 2 -> 0
```

## 关键特性

1. **循环播放** - 使用模运算确保动画无限循环
2. **速度控制** - `animation.speed` 控制播放快慢
3. **时间独立** - 不依赖帧率，基于实际时间计算
4. **平滑过渡** - 即使帧率不稳定，动画速度也保持一致

