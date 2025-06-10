# DAY1 Complie

### 1. 命令基本组成

```bash
g++ Main.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o Main
```
1. `-I/opt/homebrew/include`: 
   - `-I`选项用于指定头文件搜索路径
   - 当代码中使用`#include <SDL2/SDL.h>`时，编译器会在这个目录下查找头文件
   - 如果不指定这个路径，编译器只会在系统默认的位置查找，就会导致之前的"file not found"错误

2. `-L/opt/homebrew/lib`: 
   - `-L`选项用于指定库文件的搜索路径
   - 告诉链接器在哪里查找需要的库文件

3. `-lSDL2`: 
   - `-l`选项用于指定需要链接的库
   - 这里指定链接SDL2库
   - 链接器会查找名为`libSDL2.so`（动态库）或`libSDL2.a`（静态库）的文件

4. `-o Main`: 
   - `-o`选项指定输出文件的名称
   - 这里会生成名为`Main`的可执行文件



## 头文件和库文件的作用不一样，编译过程分两步：

### 1. 编译阶段（编译器用头文件）

- 编译器在编译 `.cpp` 文件时，需要知道函数、类型、宏等的声明。
- 这些声明都在 **头文件（.h）** 里。
- 用 `-I` 告诉编译器去哪里找这些头文件，比如 `#include <SDL2/SDL.h>` 。
- 如果不指定，编译器只会在默认的系统路径（如 `/usr/include`）找，找不到就报错。

------

### 2. 链接阶段（链接器用库文件）

- 编译器把源码编译成目标文件（.o）后，链接器负责把目标文件和库文件链接成可执行文件。

- 具体函数的实现是在 **库文件**（静态库 `.a` 或动态库 `.dylib`）里。

- 用 `-L` 告诉链接器去哪里找这些库文件。

- 用 `-l` 指定链接哪个库，比如 `-lSDL2`。

- 如果不指定，链接器找不到函数实现，会报未定义引用错误。

  

### 2. 静态库与动态库

#### 静态库（Static Library）
- 文件扩展名通常是`.a`（Unix/Linux）或`.lib`（Windows）
- 在编译时，库中的所有代码都会被复制到最终的可执行文件中
- 优点：
  - 执行时不需要依赖外部库
  - 运行速度较快
  - 部署简单
- 缺点：
  - 生成的可执行文件体积较大
  - 内存占用较大
  - 更新库时需要重新编译整个程序

#### 动态库（Dynamic Library）
- 文件扩展名通常是`.so`（Unix/Linux）、`.dylib`（macOS）或`.dll`（Windows）
- 在程序运行时才会被加载到内存中
- 优点：
  - 可执行文件体积小
  - 多个程序可以共享同一个库，节省内存
  - 库可以独立更新，不需要重新编译程序
- 缺点：
  - 运行时需要确保库文件存在
  - 可能存在版本兼容性问题
  - 部署时需要同时部署依赖的动态库







## 编译过程： 

##### preproccessor:

#include<>  #define #define 常量 #ifdef

##### compiler：

编译器只管函数调用情况 不管库函数内部是否正确

##### linker：

找到实际的implement，链接 `-l`





### 使用MakeFile:

```makefile
build:
	g++ -Wall -std=c++17 src/*.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o gameEngine;
	
run:
	./gameEngine -nocollision -debug
	
clean:
	rm gameEngine
```

```makefile
make build
make run

```

```makefile
#include<> --global:
#include <SDL2/...>

#include"" --directory:
如glm、sol、imguing头文件在libs文件夹下, 要指示编译器去找
	-I"./libs/"增加一个新的查找头文件的路径
```

更新之后：

```makefile
build:
	g++ -Wall -std=c++17 src/*.cpp -I"./libs/" -I/opt/homebrew/include  -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -o gameEngine;
	
run:
	./gameEngine -nocollision -debug
	
clean:
	rm gameEngine
```



#### 为什么SDL\Lua 与 GLM\IMGUI…不同？

##### <img src="/Users/qqhang/Library/Application Support/typora-user-images/截屏2025-06-05 18.08.35.png" alt="截屏2025-06-05 18.08.35" style="zoom:33%;" />

##### sdl、lua专门下载，为预编译的binary版本，并非源码

link时不会再compile，直接访问库实现的binary

在libs中lua、sdl只有头文件，故需要指定实现的文件

##### glm…为专门的代码文件（.h/.cpp）







