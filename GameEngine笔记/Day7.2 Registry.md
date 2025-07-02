## Day7.2 Registry



#### 管理component

```cpp
std::vector<IPool*> componentPools; // index : component Id
```

设计思路：因为Pool类含有**typename T**，无法直接放入容器，故设计一个父级接口`IPool`让`Pool`继承

Q: 为什么无法直接放入容器？

##### 原因分析：

```cpp
class IPool {
    public:
        virtual ~IPool() {};
};

template<typename T>
class Pool : public IPool {
    // 具体实现
};
```

######  **统一存储不同类型的组件池**
```cpp
std::vector<std::shared_ptr<IPool>> componentPools;
```

这个向量需要存储**不同类型的组件池**：
- `Pool<TransformComponent>`
- `Pool<SpriteComponent>` 
- `Pool<RigidBodyComponent>`

如果直接使用 `std::shared_ptr<Pool>`，编译器会报错，**因为**：
- `Pool<TransformComponent>` 和 `Pool<SpriteComponent>` 是**完全不同的类型**
- 不能存储在同一个 `std::vector<std::shared_ptr<Pool>>` 中

###### **运行时类型转换**
```cpp
// 在AddComponent中：
auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
```

通过基类指针存储，然后在需要时转换为具体的 `Pool<TComponent>` 类型。

这是C++中实现**类型擦除**和**多态容器**的标准做法！



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

#### 3.manage System 

##### 3.1Add/Remove/Has/Get <xxxSystem>System

![image-20250614195930874](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614195930874.png)

impl:

![image-20250614200605643](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614200605643.png)

```cpp
template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
    TSystem* newSystem = new TSystem(std::forward<TArgs>(args)...);
    // systems[std::type_index(typeid(TSystem))] = newSystem;
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}
```

语法上的注意事项：

- 如果你希望“有就覆盖、没有就插入”，用 systems[key] = value;

- 如果你希望“只插入，不覆盖已有的”，用 insert，并可通过返回值判断是否插入成功

- ##### 1. typeid(TSystem) 是什么？

  - typeid(TSystem) 返回一个 std::type_info 对象，代表类型 TSystem 的唯一类型信息。

  - 例如：typeid(int)、typeid(MyClass) 都可以。

  ------

  ##### 2. std::type_index 是什么？

  - std::type_index 是一个包装了 std::type_info 的类，可以用作 map/set 的 key。

  - 因为 std::type_info 不能直接比较大小或哈希，而 std::type_index 支持这些操作。

  ------

  ##### 3. 为什么要用 std::type_index(typeid(TSystem)) 作为 key？

  ###### 目的：用类型本身作为 map 的 key，实现“类型到对象”的映射

  - 在 ECS 框架中，通常会有很多不同的系统类型（如 RenderSystem、PhysicsSystem 等）。

  - 你希望能通过“类型”快速找到对应的系统对象。

  - 直接用类型名（如字符串）不安全也不高效。

  - 用 std::type_index(typeid(TSystem)) 作为 key，可以唯一标识每个系统类型。

  ------

  ##### 4. 使用Resize时注意事项

  - ##### 指针/引用失效

    - 如果 resize 导致 vector 重新分配内存（比如扩大到超出当前capacity），所有指向vector元素的指针和引用都会失效。

    - 缩小时，指向被删除元素的指针/引用也会失效。



```cpp
template <typename TSystem>
TSystem& Registry::GetSystem() const {
    return *(std::static_pointer_cast<TSystem>(systems[std::type_index(typeid(TSystem))]));
}
```

c++中的类型转换：

- dynamic_cast：运行时类型检查，通过查看RTTI来判断是否能正确将父类指针转换到子类指针（多态基类指针/引用），安全但慢，失败时返回nullptr或抛异常。

- reinterpret_cast：极端低级别转换，几乎不做检查，危险。

- const_cast：去掉const/volatile限定。

- static_pointer_cast

  - 只能用于 std::shared_ptr 或 std::unique_ptr。

  - 会返回一个新的智能指针，指向同一块内存，但类型变成你想要的类型。

  - 用法：std::static_pointer_cast<Derived>(basePtr)

  - 内部其实就是对原始指针做 static_cast，然后包一层智能指针。

  static_cast

  - 只能用于原始指针或引用或基本数据类型的转换（但是没有运行时的检查）。

  - 不能直接用于智能指针。

  - 用法：static_cast<Derived*>(basePtr)



##### 3.2Add Entity To Systems

注意：是加到所有的符合条件的System**s**上

![image-20250614200753897](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614200753897.png)



通过&来比较`entity`和`System`的**signature**

![image-20250614201332973](/Users/qqhang/Library/Application Support/typora-user-images/image-20250614201332973.png)