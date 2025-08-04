## Dear ImGui

跨平台

![image-20250802161450014](/Users/qqhang/Library/Application Support/typora-user-images/image-20250802161450014.png)

Retained mode GUI 是一种图形用户界面（GUI）编程模式，与 immediate mode GUI（即时模式 GUI）相对。根据你提供的图里的内容，我来详细解释：

---

### **Retained Mode GUI 的特点**

1. **声明式（Declarative）**

   * 你只需声明界面要包含哪些元素（按钮、输入框、矩形、线条等），而不是每一帧都重新绘制。
   * 系统会维护这些 UI 元素的状态和结构。

2. **场景（Scene）由应用程序构建**

   * 应用会构建一个完整的 UI 场景（scene graph）。
   * 例如：创建按钮、输入框，并指定它们的属性和布局。

3. **图形库维护场景层次模型**

   * 图形库会将整个 UI 的结构存储在内存中（通常是对象模型或 DOM）。
   * 这意味着 UI 的状态和层级关系是持久存在的，而不是即时生成的。

4. **通过查询对象模型进行交互**

   * 当需要获取或更新 UI 中某个控件的值时，直接访问对象模型的方法，例如：

     ```cpp
     input.GetText();
     input.SetText("Hello");
     ```

5. **基于事件驱动（Events）**

   * 用户与 UI 的交互（点击、输入、拖拽）通过事件回调来处理。
   * 例如：

     ```cpp
     buttonSave.AddClickHandler(OnSaveClicked);
     ```

6. **渲染由系统处理**

   * 渲染流程通常是：

     * 场景在内存中更新 → 系统将其转换成绘图指令 → 渲染一帧。
   * 程序员无需每帧手动重绘整个 UI。

---

### **和 Immediate Mode GUI 的对比**

* **Retained Mode：**

  * UI 状态由系统保存。
  * 使用事件来响应交互。
  * 适合复杂 UI，常见于桌面应用（Qt、WPF、HTML DOM）。
* **Immediate Mode：**

  * 每帧重新绘制整个 UI，应用直接调用draw指令
  * UI 状态由程序逻辑维护。
  * 没有events，直接逻辑上执行相关的函数
  * 更灵活，占用内存更少，适合游戏或需要频繁刷新界面的场景（Dear ImGui）。



#### 如何整合到cpp项目中：

1. 头文件：前两个为官方头文件，第三个为自己下载的demo头文件

![image-20250804111218333](/Users/qqhang/Library/Application Support/typora-user-images/image-20250804111218333.png)

2. 在Initialize中初始化imgui

```cpp
// Initialize the ImGui context
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);
```

3. 在主循环的handleInput中将sdl以及io事件传递给Imgui

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250804111502845.png" alt="image-20250804111502845" style="zoom:50%;" />