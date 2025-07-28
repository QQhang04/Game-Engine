## DAY 15 Tags & Groups

应用：

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250724201736179.png" alt="image-20250724201736179" style="zoom:50%;" />

给Entity中加入tag、group，分别添加bool方法判断

给Registry中加入一个字典，跟踪：

空间换时间

```cpp
map<string, int> entityPerTag;
map<int, string> tagPerEntity;

map<string, set<Entity>> entitiesPerGroup;
map<int, string> groupPerEntity;
```



在ECS中：

##### 使用emplace的好处：

区别：

- emplace：直接在 map 中构造 std::pair<int, std::string>

- [  ] =：先查找 key，如果不存在则默认构造一个空字符串，然后赋值

```cpp
// 使用 emplace - 高效
tagPerEntity.emplace(entity.GetId(), tag);

// 如果使用 []= - 低效
tagPerEntity[entity.GetId()] = tag;
```



<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250724204112163.png" alt="image-20250724204112163" style="zoom:50%;" />

Tag：

![image-20250724204232865](/Users/qqhang/Library/Application Support/typora-user-images/image-20250724204232865.png)

Group：

![image-20250724204316888](/Users/qqhang/Library/Application Support/typora-user-images/image-20250724204316888.png)