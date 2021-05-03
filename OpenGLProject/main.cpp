//Program from chapter
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
#include <cstdlib>
using namespace std;

//关掉vs错误提示
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#define OUTPUT std::cout

#include <iomanip>
#include <vector>
#include <queue>
#include "Object.h"
#include "Collision.h"

#define numVAOs 1

float cameraX, cameraY, cameraZ;

GLuint skyboxRenderingProgram, coordRenderingProgram, sphereRenderingProgram, planeRenderingProgram;
GLuint vao[numVAOs];
GLuint skyboxVbo[2], coordVbo[1], sphereVbo[5], planeVbo[3];
GLuint skyboxTexture, sphereTexture, planeTexture;
float rotAmt = 0.0f; //tochk

//allocation
GLuint mvLoc, projLoc, vLoc ,sLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, lMat, sMat;
Sphere mySphere = Sphere(48);









//setupvert-----------------------------------------------------------------------------------------------------------
void setupVert_sphere()
{

	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;



	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++)
	{
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}
	
	glGenBuffers(5, sphereVbo);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void setupVert_coord(void)
{
	float vertCoordPositions[18] = {
		-100.f, 0.0f, 0.0f, 100.f, 0.0f, 0.0f,
		0.0f, -100.0f, 0.0f, 0.0f, 100.0f, 0.0f,
		0.0f, 0.0f, -100.0f, 0.0f, 0.0f, 100.0f};
	glGenBuffers(1, coordVbo);

	glBindBuffer(GL_ARRAY_BUFFER, coordVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertCoordPositions), vertCoordPositions, GL_STATIC_DRAW);
}

void setupVert_skybox(void)
{
	float cubeVertexPositions[108] =
		{-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		 -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f};
	glGenBuffers(2, skyboxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);
}








//init-----------------------------------------------------------------------------------------------------------
void init(GLFWwindow *window)
{
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	skyboxRenderingProgram = Utils::createShaderProgram("vs_skybox.glsl", "fs_skybox.glsl");
	coordRenderingProgram = Utils::createShaderProgram("vs_coord.glsl", "fs_coord.glsl");
	sphereRenderingProgram = Utils::createShaderProgram("vs_sphere.glsl", "fs_sphere.glsl");
	//	planeRenderingProgram = Utils::createShaderProgram("vs_plane.glsl", "fs_plane.glsl");
	cameraX = -10.0f;
	cameraY = 3.0f;
	cameraZ = 7.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVert_sphere();
	//setupVert_skybox();
	setupVert_coord();
	sphereTexture = Utils::loadTexture("earth.jpg");
	skyboxTexture = Utils::loadCubeMap("cubeMap"); // expects a folder name
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}












//draw-----------------------------------------------------------------------------------------------------------
void draw_skybox(void)
{
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	// draw cube map

	glUseProgram(skyboxRenderingProgram);

	vLoc = glGetUniformLocation(skyboxRenderingProgram, "v_matrix");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(lMat));

	projLoc = glGetUniformLocation(skyboxRenderingProgram, "p_matrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); // cube is CW, but we are viewing the inside
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
}

void draw_coord(void)
{
	//glLineWidth(5.0f);
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	vMat = lMat;
	glUseProgram(coordRenderingProgram);

	vLoc = glGetUniformLocation(coordRenderingProgram, "v_matrix");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));

	projLoc = glGetUniformLocation(coordRenderingProgram, "p_matrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, coordVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINES, 0, 6);
}



void draw_sphere(vector<shared_ptr<Ball>> &balls)
{

	glUseProgram(sphereRenderingProgram);
	glBindVertexArray(vao[0]);

	/*
	for (int i = 0; i != balls.size(); i++)
	{
		mMat = glm::translate(glm::mat4(1.0f), balls[i]->loc());								   //构建好模型矩阵
		mvLoc = glGetUniformLocation(sphereRenderingProgram, ("m_matrix[" + to_string(i) + "]").c_str()); //获取着色器中的位置
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mMat));							   //传模型矩阵

		sMat = glm::scale(glm::mat4(1.0f), glm::vec3(balls[i]->r()));
		sLoc = glGetUniformLocation(sphereRenderingProgram, ("s_matrix[" + to_string(i) + "]").c_str());
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(sMat));
	}
	*/

//uniform
	vMat = lMat;
	glUniformMatrix4fv(glGetUniformLocation(sphereRenderingProgram, "v_matrix"), 1, GL_FALSE, glm::value_ptr(vMat));
	projLoc = glGetUniformLocation(sphereRenderingProgram, "proj_matrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

//VBO0:vert
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
//VBO1:terxture
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphereTexture);


	std::vector<float> scales;//float
	std::vector<float> models;//vec3
	for (int i = 0;i!=balls.size();i++)
		scales.push_back(balls[i]->r());

	for (int i = 0; i != balls.size(); i++) 
	{
		models.push_back(balls[i]->loc().x);
		models.push_back(balls[i]->loc().y);
		models.push_back(balls[i]->loc().z);
	}

//VBO3:model
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[3]);//传送模型矩阵
	glBufferData(GL_ARRAY_BUFFER, models.size() * 4, &models[0], GL_STATIC_DRAW);//直接刷新缓冲区
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);//配置实例化更新数据

//VBO4:scale
	/*
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[4]);//缩放矩阵
	glBufferData(GL_ARRAY_BUFFER, scales.size() * 4, &scales[0], GL_STATIC_DRAW);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);//配置实例化更新数据
	*/
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, mySphere.getNumIndices(), balls.size());
}


void display(GLFWwindow *window, double currentTime, CollisionSystem&system)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//draw_skybox();
	//draw_sphere(system.balls);
	//draw_sphere(system.fixedBalls);
	draw_coord();
}

void window_size_callback(GLFWwindow *win, int newWidth, int newHeight)
{
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}














int main(void)
{
	//freopen("out.txt", "w", stdout);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow *window = glfwCreateWindow(600, 600, "Phymodel", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	glfwSwapInterval(1);//垂直同步
	glfwSetWindowSizeCallback(window, window_size_callback);

	

	ifstream ifstrm("20ball.txt");
	ofstream ofstrm("out.txt");
	CollisionSystem system(ifstrm);
	lMat = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
	
	init(window);
	
	while (!glfwWindowShouldClose(window))
	{
		//move camera

		system.run(0.01f);

	//display
		display(window, glfwGetTime(), system);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}