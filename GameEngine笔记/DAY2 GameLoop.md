# DAY2 GameLoop

<img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-05 18.19.49.png" alt="截屏2025-06-05 18.19.49" style="zoom:50%;" />

```c++
while(true) {
  game -> processInput();
  game -> update();
  game -> render();
}
```



#### Troubles

##### 1.快慢不同的机器 -> 稳定的运行机制

```cpp
SetUp(); // Initialize
Update(); 
Draw();
```

