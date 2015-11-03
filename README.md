# Fariy2D
Simple 2D Game Engine powered by OpenGL and Lua



0 参考了ejoy2d，cocos2dx，love2d等，文字绘制功能照搬的ejoyed，场景管理基本照搬的cocos2dx

1 用到的第三方库包括lua，tcmalloc，stb_image，glew等

2 程序最大的优点是简单明了，最大的缺点是绘制图形时没有合并batch，绘一张图提交一次。(不过可以修改lua来合并batch)

3 代码比较规范除了某个头文件使用了using namespace std;

4 Bin下的文件使用前请杀毒。Fairy2d.exe启动时自动加载main.lua的代码，修改main-tower.lua以及main-bomb.lua为main.lua再运行Fairy2d.exe来启动不同的demo，demo仅起功能演示作用，不完善。Dbgview.exe是为了调试用的，错误信息通过OutputDebugString输出的，可以用Dbgview.exe看到。。。

5 做这个最初的想法是用C++提供最基本的功能，用lua来实现游戏框架以及游戏业务，通过lua库来扩展游戏功能

6 Bin的说明：Resource放的素材，Bomb，Tower，Tank，Snake放的demo的业务逻辑。GL是OpenGL的图形操作的封装。DS是基本的数据结构。Net是luaSocket。 Tower和Tank下的scene.lua封装的是简单游戏框架。