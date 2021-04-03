# PhyModel

研究性学习：小球碰撞模型的物理分析、算法设计和C++/OpenGL实现

## 环境配置

2.8更新：使用了VS项目路径变量，不再需要拷贝`OpenGLtemplate`文件夹

>从[bilibili](https://www.bilibili.com/video/BV1MJ411u7Bc)学到的\_(:з」∠)\_

~~将`OpenGLtemplate`文件夹复制到`C:\`~~

## Todo List

现在等待做的

1. 把小球类的碰撞处理做好
2. 测试小球和墙的碰撞（如果不行还要修复）
3. 实现OpenGL墙的绘制
4. 把Event更改为Collision
5. 构建一个CollisionSystem类


关于OpenGL墙的绘制的基本思路：

1. 由位置、法向量确定平面的方程
2. 按精度取值解方程得到三角形顶点坐标
3. 索引数组、构造法向量、纹理坐标等（完成Plane类的数据填充）
4. 在OpenGL中调用绘制（编写着色器）

- [ ] 写论文
    - [ ] API_wait
    - [ ] 程序框图_doing
    - [ ] 算法分析
    - [ ] 性能测试
    - [ ] 模型应用
- [ ] 实现
    - [ ] 按API进行实现
        - [ ] Ball
        - [ ] Event
        - [ ] Event_mgr

## 编辑约定

以下是几个用于程序源代码文件的标记：

* `tochk`:wait to be checked
* `todo`
* `doing`
* `tomdf`:wait to be modified
* `wrong`
* `Ac`:accomplished
* `temp`

## 文件结构

* Ball.h+Ball.cpp:小球类Ball和墙类Wall
    * `void calBtB(Ball & ball1, Ball & ball2)`
* Event.h+Event.cpp:事件类Event和事件队列类Event_mgr
    * `void push(Event & event)`
* Utils.h+Utils.cpp:OpenGL实用工具
* Sphere.h+Sphere.cpp:OpenGL小球类Sphere
* testgen.cpp+test.txt:测试数据生成
* main.cpp:主程序

## 引用仓库

* [GLFW:Graphics Library Framework](https://github.com/glfw/glfw)
* [GLEW:OpenGL Extension Wrangler Library](http://glew.sourceforge.net/)
* [GLM:OpenGL Mathematics](https://github.com/g-truc/glm)
* [SOIL2:Simple OpenGL Image Library](https://github.com/SpartanJ/soil2)

Optional:

* [GLAD:GL/GLES/EGL/GLX/WGL Loader-Generator](https://github.com/Dav1dde/glad)
* [GLAD help](https://blog.csdn.net/sigmarising/article/details/80470054)
* [freeglut](https://www.transmissionzero.co.uk/software/freeglut-devel/)
* [freeglut help](https://www.2bboy.com/archives/181.html)

*GLFW GLEW GLM SOIL2 GLAD freeglut全部都已经配置好*放在OpenGLtemplate中，默认配置为x64，使用OpenGL4.3(GLAD选项).使用GLAD时，请在项目源文件中包含`OpenGLtemplate\glad_src\glad.c`
