# PhyModel

研究性学习：小球碰撞模型的物理分析、算法设计和C++/OpenGL实现

## 环境配置

2.8更新：使用了VS项目路径变量，不再需要拷贝`OpenGLtemplate`文件夹

>从[bilibili](https://www.bilibili.com/video/BV1MJ411u7Bc)学到的\_(:з」∠)\_

~~将`OpenGLtemplate`文件夹复制到`C:\`~~

## Todo List

### 当前开发进度

status: simple-event

等待做的：

* 循环全部使用迭代器
* 全部改成智能指针

Day1上午

* 完善event类型
    * event的类图
    * event实现的函数确认全部写完：构造、处理、输出
    * 实际编写一个程序测试一下（暂时不需要用到优先队列。但要测试信息保存和操作，用Vector）
* 着手编写collisionSystem
    * 首先绘制好程序框图，或者是伪代码
    * 按照流程写好程序，测试好相关操作：初始化、移动、检测、记录、处理、删除、再预测
    * 转移到类中进行实现

Day1下午：OpenGL实现。

* 添加光照效果，阴影不用
* 构建平面和立方体，平面使用曲面细分

遗留问题：

* 返回根的问题（通过反向运行判断，但未检验实用性
* 步长的问题，留到事件实现中讨论
* 测试小球和墙的碰撞（如果不行还要修复）

## 编辑约定

以下是几个用于程序源代码文件的标记：

* `tochk`:wait to be checked
* `todo`
* `doing`
* `tomdf`:wait to be modified
* `wrong`
* `Ac`:accomplished
* `temp`

## 依赖项

* [GLFW:Graphics Library Framework](https://github.com/glfw/glfw)
* [GLEW:OpenGL Extension Wrangler Library](http://glew.sourceforge.net/)
* [GLM:OpenGL Mathematics](https://github.com/g-truc/glm)
* [SOIL2:Simple OpenGL Image Library](https://github.com/SpartanJ/soil2)

为缩减项目体积，已经删去Optional部分的库，仅保留必要设施

Optional:

* [GLAD:GL/GLES/EGL/GLX/WGL Loader-Generator](https://github.com/Dav1dde/glad)
* [GLAD help](https://blog.csdn.net/sigmarising/article/details/80470054)
* [freeglut](https://www.transmissionzero.co.uk/software/freeglut-devel/)
* [freeglut help](https://www.2bboy.com/archives/181.html)

~~*GLFW GLEW GLM SOIL2 GLAD freeglut全部都已经配置好*放在OpenGLtemplate中，~~

默认配置为x64，使用OpenGL4.3(GLAD选项).使用GLAD时，请在项目源文件中包含`OpenGLtemplate\glad_src\glad.c`
