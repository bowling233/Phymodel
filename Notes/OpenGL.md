# OpenGL学习笔记

## 重要的资料

[GLSL语法基础](https://www.jianshu.com/p/66b10062bd67)

[超棒的OpenGL教程](https://www.bilibili.com/video/BV1MJ411u7Bc)

## 结构

* `opengl.cpp`:opengl实现代码
* `vertShader.glsl`和`fragShader.glsl`:用于存放顶点着色器和片段着色器代码
* `Utils.cpp`:书本提供的一些重用函数:

```C
string Utils::readShaderFile(const char *filePath)
bool Utils::checkOpenGLError()
void Utils::printShaderLog(GLuint shader)
GLuint Utils::prepareShader(int shaderTYPE, const char *shaderPath)
void Utils::printProgramLog(int prog)
int Utils::finalizeShaderProgram(GLuint sprogram)
GLuint Utils::createShaderProgram(const char *vp, const char *fp)
GLuint Utils::createShaderProgram(const char *vp, const char *gp, const char *fp)
GLuint Utils::createShaderProgram(const char *vp, const char *tCS, const char* tES, const char *fp) 
```

## 代码结构

* OpenGL程序对象包含了一系列编译过的着色器

```cpp
//头文件部分
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "ImportedModel.h"
#include "Utils.h"

//全局定义部分
GLuint vao[nmuVAOs];
GLuint vbo[numVBOs];
GLuint renderingProgram;//程序对象ID

//编写一个类给自己用

void setupVertices(void) {
	float vertexPositions[108] = {
		//顶点数据
	};
	//相当于在init中完成缓冲区的建立和激活
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}


void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");//创建程序对象
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
}

void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);//调用程序对象，将着色器程序载入OpenGL管线阶段（GPU上）
	glPointSize(30.0f);
	glDrawArrays(GL_POINTS, 0, 1);//启动运行管线过程
}


```

## Chapter3:数学知识

变换一览表：

* 视图：指定观察者或相机位置

在应用任何模型变换前，应当先应用视图变换（视图变换移动了工作坐标系）；透视投影中，观察点在原点；正投影中，观察者在z轴正方向无穷远的地方

* 模型：在场景中移动物体；三种基本操作：Translation/Rotation/Scaling顺序很重要
* 模型视图：描述视图和模型变换的二元性
* 投影：改变视景体的大小或重设他们的形状
* 视口：对窗口上最终输出进行缩放：场景二维投影后映射到屏幕物理窗口坐标

重要概念：*齐次坐标*

一些数据类型：

* vec4(GLM/GLSL)
* vec3(GLM/GLSL)
* mat4(GLM/GLSL)

### 矩阵

* 列优先排序
* 单位矩阵：对角线值为1：`glm::mat4 m(1.0f)`
* 矩阵转置：`glm::transpose(mat4)`和`transpose(mat4)`(GLM)
* 矩阵加减法：运算符在GLSL中进行了重载
* 矩阵乘法：一般使用从右向左：点和矩阵相乘得到点：`vec4`和矩阵相乘(GLM/GLSL)
* 由于会将一系列矩阵变换运用于场景中的每一个点，预先计算*相同的矩阵*可以成倍减少计算量
* 逆矩阵：mat4.inverse()(GLM/GLSL)
* 平移矩阵：单位矩阵和第三列的三个参数构成；`glm::translate(x,y,z)`构建平移矩阵;`mat4*vec4`平移点
* 缩放矩阵：改变物体大小或者将点向原点相反方向移动/*切换坐标系*：由单位矩阵和对角线上的缩放因子构成：`glm::scale(x,y,z)`构建缩放矩阵；`mat4*vec4`
* 旋转矩阵；预备知识：围绕任何轴的旋转都可以表示为绕xyz轴旋转的组合，围绕每个轴的旋转角称为*欧拉角*；`glm::rotate(mat4,θ,x,y,z)`构建绕xyz轴旋转θ度的缩放矩阵；`mat4*vec4`；*反向旋转矩阵恰等于转置矩阵*；欧拉角有一些瑕疵，计算旋转时推荐使用*四元数*

#### 理解矩阵

矩阵的16个值表示了空间中的一个特定位置，以及相对视觉坐标系的3个轴上的方向。一个4*4矩阵包含了一个不同坐标系的位置和方向。

矩阵的每一列都代表一个由4个元素组成的向量。前三个是xyz轴上的方向向量，第四列向量包含变换后坐标系原点的xyz值。

### 向量

* 一些操作：`+-`,`normalize(vec3/4)`,`dot(vec3/4,vec3/4)`（点积）,`cross(vec3/4,vec3/4)`（叉积）
* 其他：magnitude,reflection,refraction
* *求解点到平面的最小有符号距离*：求法向量和原点到平面距离（P33）
* *外向法向量*：右手定则，光照

### 空间

* 局部空间(object space)：模型自己的方便空间：分层次地构建复杂的模型
* 世界空间：将对象定位、定向在世界空间使用*模型矩阵*

### 相机

* 合成相机：一个点：

1. 将相机放入世界的某个位置
2. 调整相机的角度：有一套自己的坐标轴U/V/N
3. 定义一个*视体(view volume)*
4. 将视体内的对象投影到投影平面(projection plane)上

需要做的事情：

1. 把相机模拟移动到合适的位置和方向
2. *视图变换*：找出世界物体和期望相机的相对位置（基于U/V/N轴）：a.将点平移，向量为负的期望相机位置 b.将点旋转，角度是负的期望相机的欧拉角：视图变换矩阵`V`，相机平移矩阵`T`，相机旋转矩阵`R`,`V=R*T`先平移后旋转
3. MV矩阵：`MV=V*M`，模型-视图矩阵

### 投影

* 视体(view volume)：需要4个参数：纵横比、视场、投影平面或近/远投影平面
* 视场：可视空间的纵向角度
* 纵横比：远近裁剪平面的宽高比
* 这些元素构成一个视锥(frustum)
* `glm::perspective()`构建一个透视矩阵
* LookAt矩阵：`glm::lookAt()`：需要一个相机位置（眼睛）、目标位置和初始向上向量`Y`来构建：当你把相机放在某处并看向特定的位置时就需要用到它了。第8章生成阴影会用到

### 一些构建矩阵的GLSL函数

主要是记住数学上这些矩阵的组成方式

```GLSL
mat4 buildScale(float x, float y, float z);


mat4 buildTranslate(float x, float y, float z)
{	mat4 trans = mat4(	1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		x, y, z, 1.0 );
	return trans;
}

//  rotation around the X axis
mat4 buildRotateX(float rad)
{	mat4 xrot = mat4(	1.0, 0.0, 0.0, 0.0,
		0.0, cos(rad), -sin(rad), 0.0,
		0.0, sin(rad), cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0 );
	return xrot;
}

//  rotation around the Y axis
mat4 buildRotateY(float rad)
{	mat4 yrot = mat4(	cos(rad), 0.0, sin(rad), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(rad), 0.0, cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0 );
	return yrot;
}

//  rotation around the Z axis
mat4 buildRotateZ(float rad)
{	mat4 zrot = mat4(	cos(rad), sin(rad), 0.0, 0.0,
		-sin(rad), cos(rad), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 );
	return zrot;
}
```

## Chapter4:管理3D数据

### 1.缓冲区和顶点属性

在init()中创建缓冲区，在C++段把顶点数据放入缓冲区，把缓冲区和着色器中声明的顶点属性相关联

* Vertex Buffer Object:顶点缓冲对象，每个顶点都需要一个，用于存储顶点的各类属性信息，在显卡存储空间中
* Vertex Array Object:顶点数组对象，是对很多个VBO的引用，用于组织缓冲区

```C
GLuint vao[nmuVAOs];
GLuint vbo[numVBOs];

glGenVertexArrays(numVAOs,vao);\\创建VAO，返回整数ID保存在数组vao中
glBindVertexArrays(vao[0]);\\标记活跃
glGenBuffers(numVBOs,vbo);
```


init()中

* 创建缓冲区
* 把顶点复制到缓冲区

display()中

* 启用包含顶点数据的缓冲区
* 缓冲区和顶点属性关联
* 启用顶点属性
* 绘制对象：`glDrawArrays()`

顶点着色器中

```GLSL
layout (location = 0) in vec3 position;
```

`layout(location=0)`是layout修饰符，把顶点属性和特定缓冲区关联起来。顶点属性的识别号是0.

绘制一个立方体：

* 将顶点值复制到缓冲区中

`glBindBuffer()`标记活跃

`glBufferData()`将顶点从数组复制到活跃缓冲区

* 将活跃缓冲区和着色器顶点属性关联

`glVertexAttribPointr()`关联顶点属性到缓冲区

`glEnableVertexAttribArray()`启用顶点属性

### 2.统一变量

`uniform`关键字在着色器中声明统一变量

`glm::value_ptr()`返回对矩阵数据的引用

`glUniformMatrix*()`将矩阵值传递给统一变量

glUniform*():数字表示几个数值，数值后缩写表示GL数值类型(GLfloat等)，v表示看作向量传递

有一些常见的：

* i:int:GLint/GLsizei
* f:float:GLfloat/GLclampf
* d:double;GLdouble
* ui:unsigned int;GLuint,GLenum

### 3.顶点属性插值

光栅化过程会线性插值顶点

`gl_Position`顶点着色器中的内置vec4变量，自动输出到光栅着色器

应该指定一个`out vec4 color`顶点属性

### 4.模型-视图和透视矩阵

* 根据所需的摄像机构建视图矩阵
* 对于每个模型：构建模型矩阵，模型和视图结合成MV矩阵，将MV和投影矩阵发送给着色器

### 5.第一个3D程序

* 模型在自己的坐标系中定义

`vMat = glm::translate(glm::mat4(1.0f),glm::vec(-cameraX,..))`构建一个变换矩阵，从单位矩阵开始，以向量的形式指定变换值

```c

#define numVAOs 1
#define numVBOs 2

Utils util = Utils();//书本中本没有这一行，不太懂
float cameraX, cameraY, cameraZ;//位置
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;//渲染程序的ID
GLuint vao[numVAOs];//顶点数组对象等的ID储存在数组中
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc;//display函数中的变量应该事先分配好
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

void setupVertices(void) {
	float vertexPositions[108] = {
		//此处省略一堆顶点，重要的是每个三角形都要声明一遍顶点，分别发送到管线
	};

	glGenVertexArrays(1, vao);//创建、标记活跃、传送数据
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");//载入着色器程序，以后可以不用写这些了耶！（见appendix）
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;//这里给定了立方体在世界中的位置
	setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);//这里没有清除缓冲区的必要？

	glUseProgram(renderingProgram);//启用着色器程序，在GPU上安装GLSL代码，让后续的调用确定着色器的顶点属性和统一变量的位置
	//获取统一变量位置，构建各个矩阵
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	glfwGetFramebufferSize(window, &width, &height);//获得窗口的宽高比
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);//glm库的投影矩阵构建函数

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));//glm许多变换支持“从单位矩阵开始，用向量的形式指定变换的值”的构建方式
	mvMat = vMat * mMat;//先m再v

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));//传统一变量矩阵
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);//缓冲区活跃
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);//关联顶点属性
	glEnableVertexAttribArray(0);//启用顶点属性

	glEnable(GL_DEPTH_TEST);//开启深度测试
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 4 - program 1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
```

```GLSL
//vert shader
#version 430

layout (location=0) in vec3 position;//因为这里有layout，所以可以通过glVertexAttribPointer和glEnableVertexAttribArray调用0来引用这个变量

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);//注意这个vec4的构建，在vec3的后面加上1
} //矩阵应用于顶点，放入内置输出gl_Position通过管线又光栅着色器插值
//fragment shader
#version 430

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{	color = vec4(1.0, 0.0, 0.0, 1.0);
}//修改以后这里变成varyingcolor，接收顶点着色器影响的颜色。其中在顶点着色器中应当使用out修饰符，片段着色器使用in修饰符
```

### 6.一个对象 多个副本

这样一个方法：在display函数中，把构建mv矩阵并绘制立方体的代码放到循环中重复执行

更大的方法：实例化：改成glDrawArrays()→glDrawArraysInstanced()绘制一堆副本

顶点着色器可以访问内置变量gl_InstanceID，指向当前正在处理对象的第几个实例

这样做需要把构建不同模型和视图矩阵的计算移动到顶点着色器中，因为每个立方体需要重新计算旋转

```GLSL
#version 430

layout (location=0) in vec3 position;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;//这个是时间因子，可以改成我们的ID因子

out vec4 varyingColor;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{	float x = gl_InstanceID + tf;
	float a = sin(203.0 * x/8000.0) * 403.0;	//when 100000 instances
	float b = cos(301.0 * x/4001.0) * 401.0;
	float c = sin(400.0 * x/6003.0) * 405.0;

	
	mat4 localRotX = buildRotateX(1.75*x);
	mat4 localRotY = buildRotateY(1.75*x);
	mat4 localRotZ = buildRotateZ(1.75*x);
	mat4 localTrans = buildTranslate(a,b,c);

	mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
	mat4 mv_matrix = v_matrix * newM_matrix;
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	varyingColor = vec4(position,1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

```

向着色器中传递数组，使用glUniform1fv(ID,个数,数组名)

经光栅着色器插值后的像素位置（称为片段）发送到片段着色器，设置输出像素的颜色，为每个像素单独调用

约定在程序员定义的插值顶点属性变量名称中包含varying

构建旋转矩阵的方式

```C
glm::rotate(单位矩阵,θ,旋转轴vec3)
```

### 7.同场景的不同模型

不会用到，省略

### 8.矩阵堆栈

不会用到，省略。但是很有意思

### 9.Z冲突

### 10.图元

### 11.性能

* 减少动态内存分配，努力在display中实现最高的效率

* 预先计算透视矩阵

* 背面剔除

  三角形缠绕顺序：逆时针判定向前

  glEnable(GL_CULL_FACE)背面剔除

## Chapter5:纹理

### 1.加载纹理图像

Utils::loadTexture(“name”);

## Chapter6:3D模型

### 1.程序构建

做一个球

* 选择水平切片精度
* 切片圆周细分。在这个模型中，每个切片有相同数量顶点
* 顶点分组为三角形：遍历顶点，每步构建两个三角形
* 为每个顶点指定纹理坐标
* 








## Appendix

### 一些略去的代码

```C
//Opengl部分
//使用Utils.c中代替

GLuint renderingProgram(){

//1.着色器源代码部分
	constchar *vShaderSouce=
}
...
//2.创建着色器和程序对象部分
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);//创建着色器，返货整数ID作为引用序号
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vfprogram = glCreateProgram();//创建程序对象
//3.载入和编译着色器
	glShaderSource(vShader, 1, &vshaderSource, NULL);//载入着色器GLSL代码
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	glCompileShader(vShader);//编译着色器
	glCompileShader(fShader);
//4.载入和链接程序对象
	glAttachShader(vfprogram, vShader);//着色器加入程序对象
	glAttachShader(vfprogram, fShader);
	glLinkProgram(vfprogram);//请求GLSL确保兼容

	return vfprogram;
}
```

一个显示帧数的[函数](https://www.cnblogs.com/tjulym/p/5055020.html)

```C#include <time.h>
double CalFrequency()
{
     static int count;
     static double save;
     static clock_t last, current;
     double timegap;

     ++count;
     if( count <= 50 )
         return save;
     count = 0;
     last = current;
     current = clock();
     timegap = (current-last)/(double)CLK_TCK;
     save = 50.0/timegap;
     return save;
}
```

关于[帧速率控制](https://www.zhihu.com/question/63302479/answer/207636037)：

在渲染循环中计算deltatime，后在物体运动时总是用速度乘以deltatime进行运动，可以实现在不同的帧速率下达到相同的运行速度。deltatime计算可通过以下方式得到：float deltatime =0.0f;

```
float lastframe = 0.0f;
...
renderloop()
{
    float currectframe = glfwGetTime();
    deltatime = currectframe -lastframe;
    lastframe = currectframe;
...
}
```

