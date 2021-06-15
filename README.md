# PhyModel

研究性学习：小球碰撞模型的物理分析、算法设计和C++/OpenGL实现

## 环境配置

2.8更新：使用了VS项目路径变量，不再需要拷贝`OpenGLtemplate`文件夹

>从[bilibili](https://www.bilibili.com/video/BV1MJ411u7Bc)学到的\_(:з」∠)\_

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
