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

## 坐标系统的理解

### 1. **纹理坐标系统 (UV坐标)**
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

### 2. **世界坐标系统**
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

## 为什么是这个顺序？

`SDL_RenderCopyEx` 使用的是 **屏幕空间的笛卡尔坐标系**，但它的**y 轴是向下的**，这是大多数图形库（包括 SDL）采用的惯例之一。

### **纹理坐标 (col, row)**
```cpp
int row = tileId / 10, col = tileId % 10;
// col * tileSize, row * tileSize
```

- `col` 决定在纹理中的水平位置（X坐标）
- `row` 决定在纹理中的垂直位置（Y坐标）

这是因为瓦片ID的计算方式：
- `tileId / 10` 得到行号（向下递增）
- `tileId % 10` 得到列号（向右递增）

### **世界坐标 (j, i)**
```cpp
glm::vec2(j * tileSize * tileScale, i * tileSize * tileScale)
```

- `j` 是列索引，对应世界坐标的X轴
- `i` 是行索引，对应世界坐标的Y轴

## 示例说明

假设有一个 10x10 的瓦片图，每个瓦片 32x32 像素：

```
瓦片ID分布：
 0  1  2  3  4  5  6  7  8  9
10 11 12 13 14 15 16 17 18 19
20 21 22 23 24 25 26 27 28 29
...
```

对于瓦片ID = 23：
- `row = 23 / 10 = 2` (第2行，从0开始)
- `col = 23 % 10 = 3` (第3列，从0开始)

**纹理坐标：**
- `srcX = col * tileSize = 3 * 32 = 96`
- `srcY = row * tileSize = 2 * 32 = 64`

**世界坐标：**
- `x = j * tileSize * tileScale = 3 * 32 * 2 = 192`
- `y = i * tileSize * tileScale = 2 * 32 * 2 = 128`

这样就能正确地从纹理中提取对应的瓦片，并将其放置在正确的位置上。

**总结：** 这个顺序确保了瓦片在纹理中的位置和在世界中的位置保持一致，使得地图能够正确渲染。