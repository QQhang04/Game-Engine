## Day 11.1 Actual Displaying Texture into Display

`SpriteComponent`:

![image-20250630111348463](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630111348463.png)

`RenderSystem`:

draw PNG file

![image-20250630111013350](/Users/qqhang/Library/Application Support/typora-user-images/image-20250630111013350.png)

**subsection**: srcRect

有时候需要从一张大的texture素材进行裁剪

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250630110250298.png" alt="image-20250630110250298" style="zoom:50%;" />



疑难问题：

##### 1.把texture渲染到实际屏幕中时的流程和各个参数的实际用途

```tex
纹理内存 → 源矩形裁剪 → 目标矩形缩放 → 屏幕像素
   ↓           ↓            ↓           ↓
SDL_Texture  srcRect     destRect    屏幕显示
```
1.1**源矩形**裁剪

const std::string& textureId = "", int w = 0, int h = 0, int srcRectX = 0, int srcRectY = 0

从哪个texture裁剪，裁剪出来的宽度和高度，裁剪的左上角的位置


```cpp
struct SpriteComponent {
    std::string textureId;
    int width, height;
    SDL_Rect srcRect;

    SpriteComponent(const std::string& textureId = "", int w = 0, int h = 0, int srcRectX = 0, int srcRectY = 0) : textureId(textureId), width(w), height(h) {
        srcRect.x = srcRectX;
        srcRect.y = srcRectY;
        srcRect.w = width;
        srcRect.h = height;
        this -> srcRect = srcRect;
    }
};
```



1.2**目标矩形**

tranform组件，copy到哪里，缩放的影响

```cpp
SDL_Rect destRect = {
    static_cast<int>(transform.position.x),                    
  // 屏幕X坐标
    static_cast<int>(transform.position.y),                    
  // 屏幕Y坐标
    static_cast<int>(sprite.width * transform.scale.x),       
  // 渲染宽度
    static_cast<int>(sprite.height * transform.scale.y)       
  // 渲染高度
};
```

1.3**实际渲染**

```cpp
SDL_RenderCopy(renderer, 
    assetStore->GetTexture(sprite.textureId), 
    &srcRect, 
    &destRect);
```



##### 2.unique_ptr的引用

在RenderSystem中：

```cpp
void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            for (auto entity : GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto sprite = entity.GetComponent<SpriteComponent>();

                SDL_Rect srcRect = sprite.srcRect;
                SDL_Rect destRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };
                
                SDL_RenderCopy(renderer, 
                assetStore->GetTexture(sprite.textureId), 
                &srcRect, 
                &destRect);
            }
        }
```

std::unique_ptr<AssetStore>& assetStore是对智能指针的引用，相当于只有房子的钥匙，但是不能转让或者卖出房子。

倘若是td::unique_ptr<AssetStore> assetStore，那么首先，Game类中的assetStore就会指向nullptr，然后，在update函数结束时指针自动析构