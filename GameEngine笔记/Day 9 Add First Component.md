## Day 9 Add First Component



// todo: Add some components to that entity

![image-20250616101204402](/Users/qqhang/Library/Application Support/typora-user-images/image-20250616101204402.png)

```cpp
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
```



增加RigidBodyComponent

vec2 velocity



**注意！！！**包含模版的方法必须在头文件中包含完整定义，因为在链接前编译器就需要正确找到并实例化



**Q疑惑**：transformcomponent和Component类不需要有任何关系吗？

```cpp
registry->AddComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
```

```cpp
#ifndef TRANSFORM_COMPONENTS_H
#define TRANSFORM_COMPONENTS_H

#include <glm/glm.hpp>

struct TransformComponent {
    public:
        glm::vec2 position;
        glm::vec2 scale;
        double rotation;

        TransformComponent(glm::vec2 position = glm::vec2(0.0, 0.0), glm::vec2 scale = glm::vec2(1.0, 1.0), double rotation = 0.0) : position(position), scale(scale), rotation(rotation) {}
};
#endif
```

```cpp
struct IComponent {
    protected:
        static int nextId;
};

template<typename TComponent>
class Component : public IComponent {
    public:
        static int GetId() {
            static auto id = nextId++;
            return id;
        }
};
```

**A**：耶斯，不需要，在`AddComponent`中，有这行代码

```cpp
const auto componentId = Component<TComponent>::GetId();
```

从编译器的角度详细解释这行代码的处理过程：

- 模板实例化阶段：

```cpp
template<typename TComponent>
class Component : public IComponent {
    public:
        static int GetId() {
            static auto id = nextId++;
            return id;
        }
};
```
- 当编译器看到 `Component<TComponent>` 时，它会为每个不同的 `TComponent` 类型创建一个独立的类
- 比如当 `TComponent` 是 `TransformComponent` 时，编译器会生成类似这样的代码：
```cpp
class Component_TransformComponent : public IComponent {
    public:
        static int GetId() {
            static auto id = nextId++;
            return id;
        }
};
```

