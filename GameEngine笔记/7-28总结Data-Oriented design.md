### Data-Oriented design



用于对性能敏感的设计

充分利用**CPU的cache**：

保存频繁使用的内存地址，如L1,2,3,4



**OOP设计**：非常具象化，利于人类理解。但是会导致poor data locality

**DO设计**：利于计算机来理解计算，数据在内存中连续排布，尽量降低cache miss

![image-20250728181308785](/Users/qqhang/Library/Application Support/typora-user-images/image-20250728181308785.png)



重点：data layout，根据数据和字段的需要来排布

#### **ECS如何提高命中率：**

- 10,000 个实体，每个实体有位置（pos）、速度（vel）、血量（hp）。
- 只需更新位置：pos += vel * dt

###### **OOP（AoS）模式：**

- 访问数据布局：pos, vel, hp, pos, vel, hp, ...
- 每次读取一个位置，都同时把无用的 hp 读入 cache，浪费带宽。

###### **ECS（SoA）模式：**

- 访问数据布局：pos[], vel[]
- 直接顺序访问 pos[] 和 vel[]，cache line 利用率接近 100%。

例如“an Entity dosen’t have to be a component”



<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250728182252649.png" alt="image-20250728182252649" style="zoom:50%;" />



#### 目前的设计缺陷：

不是所有的entity都有所有的component

存在data gap

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250728182550080.png" alt="image-20250728182550080" style="zoom:50%;" />



##### 解决办法：Keep data packed

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250728182739510.png" alt="image-20250728182739510" style="zoom:50%;" />

重构Pool：

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250730100613864.png" alt="image-20250730100613864" style="zoom:50%;" />

在add和remove entity的时候都要实时更新

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250730100926540.png" alt="image-20250730100926540" style="zoom:50%;" />

改动：

在IPool中添加虚方法removeComponent（int entityId）

在pool类加入set，get方法，并override虚方法

在ECS中addComponent以及删除entity时调用新的方法