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

```cpp
void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);
    Logger::Log("Texture added to asset store: " + assetId);
}
```

`emplace` 是 C++11 引入的一个 STL 容器方法，它的作用是**直接在容器内部构造对象**，而不是先构造对象再拷贝到容器中。

1. `emplace` vs `insert` 的区别

使用 `insert`（传统方式）：

```cpp
// 先构造一个 pair 对象
std::pair<std::string, SDL_Texture*> temp_pair(assetId, texture);
// 然后拷贝到 map 中
textures.insert(temp_pair);
```

使用 `emplace`（现代方式）：

```cpp
// 直接在 map 内部构造 pair 对象
textures.emplace(assetId, texture);
```

2. 性能优势

`emplace` 的主要优势是**避免不必要的拷贝构造**



###### 3.SDL渲染png图片的流程



1. 计算机图形学角度

1.1 图像数据的层次结构

```
文件系统 → 原始像素数据 → GPU纹理内存
   ↓           ↓              ↓
  PNG/JPG   SDL_Surface   SDL_Texture
```

**为什么需要 SDL_Surface？**

- **像素格式统一**：不同图像文件格式（PNG、JPG、BMP）有不同的像素排列方式
- **内存布局标准化**：SDL_Surface 提供统一的 RGBA 格式
- **跨平台兼容**：不同操作系统的图像库有不同的内存布局

1.2 GPU 纹理的特殊要求

```cpp
SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
```
这一步的关键意义：
- **GPU 内存优化**：纹理需要存储在 GPU 的专用内存中
- **硬件加速**：GPU 可以直接访问这些纹理进行渲染
- **内存对齐**：GPU 对内存对齐有特殊要求
- **压缩格式**：GPU 可能使用特殊的压缩格式（如 DXT）



2. 设计模式角度

你问得很好！让我重新分析一下，实际上这里**并不是**典型的抽象工厂模式。我之前的表述不够准确。

让我重新解释这里实际使用的设计模式：

##### 1. **简单工厂模式（Simple Factory）**

```cpp
// 简单工厂模式
SDL_Surface* surface = IMG_Load(filePath.c_str());  // 根据文件扩展名创建不同的 Surface
```

##### 2. 实际代码中使用的是**策略模式（Strategy Pattern）**

```cpp
// SDL_image 内部使用策略模式处理不同格式
class ImageLoadStrategy {
public:
    virtual SDL_Surface* Load(const std::string& filePath) = 0;
};

class PNGLoadStrategy : public ImageLoadStrategy {
public:
    SDL_Surface* Load(const std::string& filePath) override {
        // PNG 特定的加载逻辑
        return LoadPNG(filePath);
    }
};

class JPEGLoadStrategy : public ImageLoadStrategy {
public:
    SDL_Surface* Load(const std::string& filePath) override {
        // JPEG 特定的加载逻辑
        return LoadJPEG(filePath);
    }
};

// 策略选择器
class ImageLoader {
private:
    std::map<std::string, std::unique_ptr<ImageLoadStrategy>> strategies;
    
public:
    ImageLoader() {
        strategies["png"] = std::make_unique<PNGLoadStrategy>();
        strategies["jpg"] = std::make_unique<JPEGLoadStrategy>();
        strategies["jpeg"] = std::make_unique<JPEGLoadStrategy>();
        // ...
    }
    
    SDL_Surface* Load(const std::string& filePath) {
        std::string extension = GetFileExtension(filePath);
        auto it = strategies.find(extension);
        if (it != strategies.end()) {
            return it->second->Load(filePath);
        }
        throw std::runtime_error("Unsupported image format");
    }
};
```

4.1 简单工厂模式

```cpp
SDL_Surface* surface = IMG_Load(filePath.c_str());
```
- **工厂方法**：`IMG_Load` 根据文件扩展名创建不同的 Surface 对象
- **产品**：`SDL_Surface` 是统一的产品接口
- **具体产品**：不同格式的 Surface 实现

4.2 适配器模式

```cpp
SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
```
- **适配器**：`SDL_CreateTextureFromSurface` 将 Surface 适配为 Texture
- **源接口**：`SDL_Surface`
- **目标接口**：`SDL_Texture`

4.3 建造者模式（Builder Pattern）

```cpp
// 整个纹理创建过程可以看作建造者模式
class TextureBuilder {
private:
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    
public:
    TextureBuilder& LoadFromFile(const std::string& filePath) {
        surface = IMG_Load(filePath.c_str());
        return *this;
    }
    
    TextureBuilder& ConvertToTexture(SDL_Renderer* renderer) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        return *this;
    }
    
    TextureBuilder& CleanupSurface() {
        SDL_FreeSurface(surface);
        surface = nullptr;
        return *this;
    }
    
    SDL_Texture* Build() {
        return texture;
    }
};

// 使用方式
SDL_Texture* texture = TextureBuilder()
    .LoadFromFile("image.png")
    .ConvertToTexture(renderer)
    .CleanupSurface()
    .Build();
```

