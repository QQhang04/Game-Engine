## Managing Assets

texture, audio …….



<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250630091213134.png" alt="image-20250630091213134" style="zoom:50%;" />

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250630091334180.png" alt="image-20250630091334180" style="zoom:50%;" />

the source png might be the same



##### load png assets not directly through source, using AssetStore instead.

id –––– real asset 

享元模式 

![image-20250630092209190](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630092209190.png)

 



In Game.h:

![image-20250630093714815](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630093714815.png)

In Game.cpp// `Setup()`

![image-20250630093959500](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630093959500.png)

Impl:

![image-20250630095105572](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630095105572.png)

其中出现的问题：

###### 1.对SpriteComponent改变构造方法的参数后，编译无法通过

增加textureId属性

![image-20250630102404924](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630102404924.png)

**原因**：

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250630102640372.png" alt="image-20250630102640372" style="zoom: 50%;" />

在`Pool`中的`resize`方法中，由于对vector的容器resize会直接调用容器对象的默认构造方法，故需要完善SpriteComponent的默认构造，或者增加默认参数

**改进之后**：

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250630103425659.png" alt="image-20250630103425659" style="zoom:50%;" />

###### 2.emplace到底是干什么的



3.

RenderSystem:

draw PNG file