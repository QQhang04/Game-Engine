## TileMap



![image-20250630194605590](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630194605590.png)

before create entities, we need to load the tilemap textures from jungle.png

![image-20250630195107887](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630195107887.png)

use jungle.map

![image-20250630194907126](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630194907126.png)

![image-20250630195021534](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630195021534.png)



##### IMPL:

![image-20250630211809476](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630211809476.png)



疑难：为什么在这里使用 `col * tileSize, row * tileSize` 的顺序。

### 坐标系统的理解

`SDL_RenderCopyEx` 使用的是 **屏幕空间的笛卡尔坐标系**，但它的**y 轴是向下的**，这是大多数图形库（包括 SDL）采用的惯例之一。x

#### 1. **纹理坐标系统 (UV坐标)**
在 `SpriteComponent` 中：
```cpp
tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, col * tileSize, row * tileSize);
```

这里的参数是 `(textureId, width, height, srcX, srcY)`，其中 `srcX` 和 `srcY` 是纹理中的源坐标。

**纹理坐标系统：**
- **X轴 (col)**: 从左到右
- **Y轴 (row)**: 从上到下

```
纹理坐标系：
(0,0) -------- (1,0)
  |              |
  |              |
  |              |
(0,1) -------- (1,1)
```

#### 2. **世界坐标系统**
在 `TransformComponent` 中：
```cpp
tile.AddComponent<TransformComponent>(glm::vec2(j * tileSize * tileScale, i * tileSize * tileScale), glm::vec2(tileScale, tileScale), 0.0);
```

这里的参数是 `(position, scale, rotation)`，其中 `position` 是 `(x, y)`。

**世界坐标系统：**
- **X轴 (j)**: 从左到右
- **Y轴 (i)**: 从上到下

```
世界坐标系：
(0,0) -------- (width,0)
  |              |
  |              |
  |              |
(0,height) ---- (width,height)
```



### Order Of Tile

depending on the layers

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250630213459857.png" alt="image-20250630213459857" style="zoom:50%;" />

![image-20250630213638287](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630213638287.png)

SpriteComponent:

![image-20250630213840787](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630213840787.png)



// todo: use layer instead of z-index

sort all the entities of our system by z-index before rendering性能太差

采用什么排序策略以及什么时机进行排序需要根据游戏类型来进行优化

![image-20250630214350244](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630214350244.png)



![image-20250630220038575](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630220038575.png)