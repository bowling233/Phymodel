//#define OPENGL_CLOSE
//################
//头文件
//################
//OpenGL
#ifndef OPENGL_CLOSE
#include <GL\glew.h>
#include <GLFW\glfw3.h>
//#include <SOIL2\soil2.h>
#include "Utils.h"
#include "Sphere.h"
#endif
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
//自选用的头文件
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <queue>
#include "Object.h"
#include "Collision.h"
using namespace chrono;
//为freopen()关闭visual studio错误提示
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

//################
//预分配变量
//################
float aspect, rot_v, amt = 0.0f;
glm::vec3 cameraLoc, lookAt;

#ifndef OPENGL_CLOSE
//OpenGL数据
#define numVAOs 1
GLuint vao[numVAOs];
GLuint coordVbo[1], sphereVbo[4], planeVbo[3], containerVbo[3];
//OpenGL着色器和计算
GLuint coordRenderingProgram,
	sphereRenderingProgram,
	planeRenderingProgram,
	containerRenderingProgram;
// 光照
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc,
	posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
float lightPos[3];
glm::vec3 currentLightPos, transformed;
// 窗口、视角、位置、变换
int width, height;
glm::mat4 pMat, mMat, mvMat, lMat, invTrMat, rMat, vMat;
//OpenGL材质和模型
// white light
float globalAmbient[4] = {0.7f, 0.7f, 0.7f, 1.0f};
float lightAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
// gold material
float *matAmb = Utils::goldAmbient();
float *matDif = Utils::goldDiffuse();
float *matSpe = Utils::goldSpecular();
float matShi = Utils::goldShininess();
// 球体模型
Sphere mySphere = Sphere(48);
#endif

//################
//小工具函数
//################
#ifndef OPENGL_CLOSE
float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

glm::mat4 buildRotate(glm::vec3 vectorBefore, glm::vec3 vectorAfter)
{
	glm::vec3 rotationAxis;
	float rotationAngle;
	glm::mat4 rotationMatrix;
	rotationAxis = glm::cross(vectorBefore, vectorAfter);
	rotationAngle = acos(glm::dot(glm::normalize(vectorBefore), glm::normalize(vectorAfter)));
	rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis);
	return rotationMatrix;
}

void installLights(GLuint renderingProgram, glm::mat4 vMatrix)
{
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0)); //currentLightPos不用传递，只需更改
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
}

void window_size_callback(GLFWwindow *win, int newWidth, int newHeight)
{
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

//############
//设置顶点
//############
void setupVert_sphere(vector<shared_ptr<Ball>> &balls)
{
	glUseProgram(sphereRenderingProgram);
	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> nvalues;
	std::vector<float> scales;

	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++)
	{
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	for (auto const &i : balls)
		scales.push_back(i->r());

	glGenBuffers(4, sphereVbo);
	//VBO:0.顶点1.法向量2.缩放3.位置
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[2]);
	glBufferData(GL_ARRAY_BUFFER, scales.size() * sizeof(float), &scales[0], GL_STATIC_DRAW);
}

void setupVert_coord(void)
{
	glUseProgram(coordRenderingProgram);
	float vertCoordPositions[18] = {
		-1000.f, 0.0f, 0.0f, 1000.f, 0.0f, 0.0f,
		0.0f, -1000.0f, 0.0f, 0.0f, 1000.0f, 0.0f,
		0.0f, 0.0f, -1000.0f, 0.0f, 0.0f, 1000.0f};
	glGenBuffers(1, coordVbo);

	glBindBuffer(GL_ARRAY_BUFFER, coordVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertCoordPositions), vertCoordPositions, GL_STATIC_DRAW);
}

void setupVert_container(std::vector<std::shared_ptr<Container>> const &containers)
{
	glUseProgram(containerRenderingProgram);
	vector<float> locations;
	vector<float> scale;
	float containerVertexPositions[108] = {
		-0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f};

	for (auto const &i : containers)
	{
		locations.push_back(i->loc().x);
		locations.push_back(i->loc().y);
		locations.push_back(i->loc().z);
		scale.push_back(i->scale().x);
		scale.push_back(i->scale().y);
		scale.push_back(i->scale().z);
	}

	glGenBuffers(3, containerVbo);
	glBindBuffer(GL_ARRAY_BUFFER, containerVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(containerVertexPositions), containerVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, containerVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, locations.size() * sizeof(float), &locations[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, containerVbo[2]);
	glBufferData(GL_ARRAY_BUFFER, scale.size() * sizeof(float), &scale[0], GL_STATIC_DRAW);
}

//############
//OpenGL初始化
//############
void init(GLFWwindow *window, CollisionSystem &system)
{
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	coordRenderingProgram = Utils::createShaderProgram("vs_coord.glsl", "fs_coord.glsl");
	sphereRenderingProgram = Utils::createShaderProgram("vs_sphere.glsl", "fs_sphere.glsl");
	planeRenderingProgram = Utils::createShaderProgram("vs_plane.glsl", "tcs_plane.glsl", "tes_plane.glsl", "fs_plane.glsl");
	containerRenderingProgram = Utils::createShaderProgram("vs_container.glsl", "tcs_plane.glsl", "tes_plane.glsl", "fs_plane.glsl");

	//构建视口矩阵
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	lMat = glm::lookAt(cameraLoc, lookAt, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

	//设置模型顶点
	setupVert_coord();
	setupVert_sphere(system.b());
	//if(!system.w().empty())
	//	setupVert_plane(system.w());
	if(!system.c().empty())
		setupVert_container(system.c());

	cout << "log:OpenGL初始化成功" << endl;
}

//############
//OpenGL绘制
//############
void draw_coord(void)
{
	glUseProgram(coordRenderingProgram);
	glLineWidth(5.0f);

	glUniformMatrix4fv(glGetUniformLocation(coordRenderingProgram, "v_matrix"), 1, GL_FALSE, glm::value_ptr(lMat));
	glUniformMatrix4fv(glGetUniformLocation(coordRenderingProgram, "p_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, coordVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINES, 0, 6);
}

void draw_sphere(vector<shared_ptr<Ball>> const &balls)
{
	glUseProgram(sphereRenderingProgram);

	//light
	currentLightPos = cameraLoc;
	//currentLightPos = glm::vec3(rMat * glm::vec4(currentLightPos, 1.0f));
	installLights(sphereRenderingProgram, lMat);

	//uniform
	glUniformMatrix4fv(glGetUniformLocation(sphereRenderingProgram, "v_matrix"), 1, GL_FALSE, glm::value_ptr(lMat));
	glUniformMatrix4fv(glGetUniformLocation(sphereRenderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));

	//VBO0:vert
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//VBO1:norm
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 0);

	//VBO2:scale
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[2]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1); //第一个参数是location地址，第二个参数表示几个实例化更新一次

	//VBO3:model
	std::vector<glm::vec3> models; //vec3
	for (auto const &i : balls)
		models.push_back(i->loc());
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[3]);												   //传送模型位置
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::vec3), &models[0], GL_DYNAMIC_DRAW); //直接刷新缓冲区
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1); //配置实例化更新数据

	//draw
	glEnable(GL_CULL_FACE); //背面消除，加快速度
	glFrontFace(GL_CCW);
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, mySphere.getNumIndices(), balls.size());
	glDisable(GL_CULL_FACE);
}

void draw_container(vector<shared_ptr<Container>> const &containers)
{
	glUseProgram(containerRenderingProgram);
	glLineWidth(1.0f);

	//uniform
	glUniformMatrix4fv(glGetUniformLocation(containerRenderingProgram, "v_matrix"), 1, GL_FALSE, glm::value_ptr(lMat));
	glUniformMatrix4fv(glGetUniformLocation(containerRenderingProgram, "p_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));

	//VBO1:model
	glBindBuffer(GL_ARRAY_BUFFER, containerVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//VBO:location
	glBindBuffer(GL_ARRAY_BUFFER, containerVbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	//VBO3:scale
	glBindBuffer(GL_ARRAY_BUFFER, containerVbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArraysInstanced(GL_PATCHES, 0, 108, containers.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display(GLFWwindow *window, double currentTime, CollisionSystem &system)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	amt += rot_v; //配置旋转
	rMat = glm::rotate(glm::mat4(1.0f), toRadians(amt), glm::vec3(0.0f, 0.0f, 1.0f));
	lMat = glm::lookAt(glm::vec3(rMat * glm::vec4(cameraLoc, 1.0f)), lookAt, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

	draw_coord();
	draw_sphere(system.b());
	if (!system.c().empty())
		draw_container(system.c());
	//cout << "OpenGL render success" << endl;//<debug>
}
#endif

//############
//主程序
//############
int main(void)
{
	//OpenGL初始化
#ifndef OPENGL_CLOSE
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow *window = glfwCreateWindow(600, 600, "Phymodel", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);
	glfwSwapInterval(1); //垂直同步
	glfwSetWindowSizeCallback(window, window_size_callback);
#endif

	//输入输出
	//cout << "请输入您的数据文件名（其它路径记得转义）：";
	//string file;
	//cin >> file;
	//freopen("event_out.txt", "w", stdout);
	ifstream ifstrm("E:\\Coding\\data\\4.1.3.2.txt");

	//创建碰撞系统
	//ifstrm >> cameraLoc >> lookAt >> rot_v;
	cameraLoc = glm::vec3(10, 10, 5);
	lookAt = glm::vec3(0.0f);
	rot_v = 0.05f;
	CollisionSystem system(ifstrm);
	ofstream ofstrm("out.txt");
	//cout << system;

#ifndef OPENGL_CLOSE
	init(window, system); //提供system的相关信息为OpenGL绘制预先存储数据
#endif
	//*帧率
	auto last = system_clock::now();
	auto current = system_clock::now();
	auto duration = duration_cast<microseconds>(current - last);
	unsigned int count = 0;//*/

	int k = 0,m=1;
	bool flag = false;
	system.run(11.0);
	system.reverse();
	system.run(11.0);
	//程序主循环
#ifndef OPENGL_CLOSE
	while (!glfwWindowShouldClose(window))
#endif
#ifdef OPENGL_CLOSE
	while(1)
#endif
	{
		/*帧率
		if (count++ == 30) {
			last = current;
			current = system_clock::now();
			duration = duration_cast<microseconds>(current - last);
			cout << "fps::"
				<< 30.0/(double(duration.count()) * microseconds::period::num / microseconds::period::den)
				<< endl
				<< "每秒碰撞::"
				<< sumbounce / (double(duration.count()) * microseconds::period::num / microseconds::period::den) 
				//<< endl
				//<< "exam persecond::"
				//<< sumexam / (double(duration.count()) * microseconds::period::num / microseconds::period::den)
				<< endl;
			count = 0;
			sumbounce = 0;
			cout << "系统动能：" << system.ek() << endl;
			//cout << "事件队列长度：" << system.e().size() << endl;
			cout << "当前系统时间：" << system.time() << std::endl;//<debug>
			//sumexam = 0;
		}//*/

		/*if (!(k<0))
		system.run(1.0f/15.0);
		k += m;
		if (k == 150)
		{
			m = -1;
			system.reverse();
		}*/


		//if (system.time() > 30)
			//break;

		//显示
		#ifndef OPENGL_CLOSE
		display(window, glfwGetTime(), system);
		glfwSwapBuffers(window);
		glfwPollEvents();
		#endif
	}
	getchar();
	ofstrm << system;
#ifndef OPENGL_CLOSE
	glfwDestroyWindow(window);
	glfwTerminate();
#endif
	exit(EXIT_SUCCESS);
}