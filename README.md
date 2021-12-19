# PhyModel

研究性学习：计算机模拟刚性球体系统的物理分析、算法设计和OpenGL可视化

![](https://img.shields.io/badge/license-GPL-blue?style=for-the-badge)

**请注意**：本项目仓库源码遵循GPL许可协议。

使用方法请参阅[用户手册](Manual.md)

## 环境配置

本项目使用Visual Studio 2022创建，`OpenGLtemplate`中包含了程序构建所需的库，因此无需配置环境变量。

>从[bilibili](https://www.bilibili.com/video/BV1MJ411u7Bc)学到的\_(:з」∠)\_

本项目默认使用`x64`配置编译运行，未配置过`x86`选项，也未在32位系统上进行测试。

项目测试环境：
* Windows11 x64 22000.348 i5-8250U
* Windows10 x64 19043.928 i5-10500U
* Windows10 x64 

## 依赖项

* [GLFW:Graphics Library Framework](https://github.com/glfw/glfw)
* [GLEW:OpenGL Extension Wrangler Library](http://glew.sourceforge.net/)
* [GLM:OpenGL Mathematics](https://github.com/g-truc/glm)
* [SOIL2:Simple OpenGL Image Library](https://github.com/SpartanJ/soil2)

为缩减项目体积，已经删去Optional部分的库

Optional:

* [GLAD:GL/GLES/EGL/GLX/WGL Loader-Generator](https://github.com/Dav1dde/glad)
* [GLAD help](https://blog.csdn.net/sigmarising/article/details/80470054)
* [freeglut](https://www.transmissionzero.co.uk/software/freeglut-devel/)
* [freeglut help](https://www.2bboy.com/archives/181.html)

默认配置为x64，使用OpenGL4.3(GLAD选项).使用GLAD时，请在项目源文件中包含`OpenGLtemplate\glad_src\glad.c`
