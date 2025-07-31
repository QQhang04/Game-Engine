## 7-31 Font

1. Adding Font to **AssetStore**

2. Add **TextLabelComponent**

3. **![截屏2025-07-31 10.37.33](/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-07-31 10.37.33.png)**

4. **RenderTextSystem**

   ![image-20250731102812750](/Users/qqhang/Library/Application Support/typora-user-images/image-20250731102812750.png)

流程是：

用字体库（SDL_ttf）把文字渲染成一张图片（Surface）。

再把这张图片转换成显卡能用的纹理（Texture）。

最后用 SDL 渲染 Texture 到屏幕。



4. ##### 在Initialize中Init TTF