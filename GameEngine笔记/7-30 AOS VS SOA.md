### Array of Structs VS Struct of Arrays

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250730115152252.png" alt="image-20250730115152252" style="zoom:50%;" />



**如果频繁拿去component中的某一个字段，那么SoA效率更高**

Eg. Transform Component

**但是如果要同时拿到所有字段，AoS效率更高**

Eg.<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250730115532542.png" alt="image-20250730115532542" style="zoom: 33%;" />

很多系统只对这个component特定的字段感兴趣

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250730115744730.png" alt="image-20250730115744730" style="zoom:50%;" />





#### Valgrind

#### Popular ECS Library

- entt
- 