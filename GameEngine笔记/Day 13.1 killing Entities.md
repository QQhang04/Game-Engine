## killing Entities

at `Registry`:

##### deque<int> freeIds; // 已经删除的可以复用的entity-id

deque：双向数组(2维)，头尾插入删除效率高

区分:list:双向链表



增加entity时，先看freeIds是否有东西，有的话直接用里面的

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250702202714288.png" alt="image-20250702202714288" style="zoom:50%;" />



##### void KillEntity(Entity entity)

add entity to entities to be killed



at Registry/Update()

调用**RemoveEntityFromSystems**删除掉所有tobekilled集合里的entity

![image-20250702202251802](/Users/qqhang/Library/Application Support/typora-user-images/image-20250702202251802.png)



##### ps. <Templates>会显著增加编译时间



###### 补档：为什么要用到集合缓冲删除或者添加entity？

1.防止system中的迭代器失效（for auto entity:entities）

2.保证一帧内所有system面对的entity是一致的