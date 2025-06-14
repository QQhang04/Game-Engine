## Day7.2 Registry



#### 管理component

```cpp
std::vector<IPool*> componentPools; // index : component Id
```

设计思路：因为Pool类含有**typename T**，无法直接放入容器，故设计一个父级接口`IPool`让`Pool`继承

![image-20250614171807314](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614171807314.png)



#### 管理每个entity的components拥有情况

```cpp
vector<Signature> emtityComponentSignatures; // index : eneity id
```



#### 管理active systems

```cpp
unordered_map<std::type_index, System*> systems;
```

`std::type_index` 是 C++ 标准库（位于 `<typeindex>` 头文件中）提供的一个类，主要用于**在运行时高效地表示和比较类型信息**。它是对 `std::type_info` 的轻量级封装，使其可以被用作关联容器（如 `std::unordered_map`、`std::map`）的键（key）

*// TODO:*

​        *// create a new entity*

​        *// kill entity*

​        *// add a component to entity*

​        *// remove a component from entity*

​        *// does a entity has a component*

​        *// add system*

​        *// remove system*

​        *// has system*

​        *// get system*



#### 1.create/remove Entity

如果在一帧中间突然创建/删除一个entity，可能会让其他system来不及反应

用两个集合entitiesToBeAdded/Killed来缓存，在帧末尾才会实际的create/kill

![image-20250614180510038](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614180510038.png)



#### 2.manage Components link to Entity

Add/Remove/Check Component that uses template:

![image-20250614182008171](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614182008171.png)

![image-20250614181817487](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614181817487.png)

prototypes:![image-20250614182655667](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614182655667.png)



##### 2.1 Add Component to a Entity

![image-20250614184107000](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614184107000.png)

我的疑惑：这里为什么Pool这个数组是Component类型呢？

思考原因：每个entity id都对应着自己独有的Component，即使类型一致也是不同的对象

```cpp
    T newComponent(std::forward<TArgs>(args)...); // TODO ???
    componentPool -> Set(entityId, newComponent);
```

注意：若要将对象用到`set`等排序容器上，必须重载<运算符

##### 2.2Remove/Has Component

![image-20250614190009232](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614190009232.png)



注意：template func其实不是方法，只是一个`placeholder`，等待编译来fill。即**模板实例化**

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250614190834850.png" alt="image-20250614190834850" style="zoom:33%;" />

故这个示例中会模板实例化中很多不同类，每个类都有自己独有的static GetId方法