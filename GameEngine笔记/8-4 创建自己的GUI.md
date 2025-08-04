## 8-4 创建自己的GUI

```cpp
class RenderGUISystem : public System {
    public:
       RenderGUISystem() = default;
       void Update(const std::unique_ptr<Registry>& registry) {
            ImGui::NewFrame();
            
            if (ImGui::Begin("Spawn enemies")) {
                ImGui::Text("here is what we spawn new enemies");
                static int enemyXPos = 0;
                static int enemyYPos = 0;
                ImGui::InputInt("X", &enemyXPos);
                ImGui::InputInt("Y", &enemyYPos);
                if (ImGui::Button("Spawn enemy")) {
                    // 创建entity的逻辑
                }
            }
            ImGui::End();

            ImGui::Render();
            ImGuiSDL::Render(ImGui::GetDrawData());
       }
};
```

注意：为什么要用到**static**？

- static 变量（无论是全局、局部还是类的 static 成员）都存储在静态存储区（static storage area），也叫数据段（data segment），而不是栈（stack）或堆（heap）。

- 这块区域在程序启动时分配，程序结束时释放。

- 这两个变量被声明为 static，是因为 ImGui 的控件（如 InputInt）需要一个持续存在的变量来保存输入的值。

- 如果不用 static，每次进入 Update() 函数时变量都会重新初始化为 0，用户输入的值就会丢失。

- 用 static 修饰后，这两个变量在函数多次调用之间只初始化一次，值会被保留，直到程序结束。