## Day 7.1 ECS

### Folder Structure

#### SRC

​	ECS：core / clients(实际游戏逻辑) `ECS.h`中包含component/entity/system/Registry类定义

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250613214055629.png" alt="image-20250613214055629" style="zoom:50%;" />

​	Components：

​		如transformComponent

​	Systems: 实际每帧运行的逻辑

​		如MovementSystem

<img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-13 21.35.01.png" alt="截屏2025-06-13 21.35.01" style="zoom:50%;" />

在Game.cpp中如何调用？ **// TODO**

![image-20250613213703669](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613213703669.png)



![image-20250613213744748](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613213744748.png)

![image-20250613213851386](/Users/qqhang/Library/Application Support/typora-user-images/image-20250613213851386.png)



### Component Signature —— link the entity to system

system通过bitset数组**signature**知道32个位0/1来知道需要什么component

<img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-13 21.48.20.png" alt="截屏2025-06-13 21.48.20" style="zoom:50%;" />



to identify which component: add `id` property to Component 

##### 对应关系: 1个id 对应一个 component type

```cpp
struct IComponent {
    protected:
        static int nextId;
};

template<typename T>
class Component : public IComponent {
    static int GetId() {
        static auto id = nextId++;
        return id;
    }
};
```



###### 关键点：静态局部变量只初始化一次

因为静态局部变量存储在数据段，虽然作用域在函数内，但是生命周期是整个程序运行时，所以这张写法保证了个id 对应一个 component type；再此调用该方法则直接返回之前初始化过的id



删除方法：

`remove_if`将符合条件的放到容器末尾并且返回新逻辑的**结束迭代器**

```cpp
void System::RemoveEntityFromSystem(Entity entity) {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {return entity == other;}), 
        entities.end()
    );
}
```



### Registry —— Entity Manager

功能：协调整个ECS

![image-20250614163606090](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614163606090.png)

- how to store components? –> **component pool**

```cpp
vector<Pool*> componentPools;
// index <--> component type id
```



##### the  POOL

each pool index <–> the entity id

![image-20250614163057320](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614163057320.png)



![image-20250614163735807](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614163735807.png)





