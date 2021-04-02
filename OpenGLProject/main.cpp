//Program from chapter
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
using namespace std;

//关掉vs错误提示
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include <iomanip>
#include <vector>
#include <queue>
#include "Object.h"

#define numVAOs 1

float cameraX, cameraY, cameraZ;

GLuint skyboxRenderingProgram, coordRenderingProgram, sphereRenderingProgram, planeRenderingProgram;
GLuint vao[numVAOs];
GLuint skyboxVbo[2], coordVbo[1], sphereVbo[3], planeVbo[3];
GLuint skyboxTexture, sphereTexture, planeTexture;
float rotAmt = 0.0f; //tochk

// variable allocation for display
GLuint mvLoc, projLoc, vLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

Sphere mySphere = Sphere(48);

//start:setupvert
void setupVert_sphere(void)
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

	glGenBuffers(3, sphereVbo);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void setupVert_coord(void) //todo
{
}
void setupVert_skybox(void) //doing
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

void init(GLFWwindow *window)
{
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	skyboxRenderingProgram = Utils::createShaderProgram("vs_skybox.glsl", "fs_skybox.glsl");
	//	coordRenderingProgram = Utils::createShaderProgram("vs_coord.glsl", "fs_coord.glsl");
	sphereRenderingProgram = Utils::createShaderProgram("vs_sphere.glsl", "fs_sphere.glsl");
	//	planeRenderingProgram = Utils::createShaderProgram("vs_plane.glsl", "fs_plane.glsl");
	cameraX = 5.0f;
	cameraY = 0.0f;
	cameraZ = 25.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVert_sphere();
	setupVert_skybox();
	sphereTexture = Utils::loadTexture("earth.jpg");
	skyboxTexture = Utils::loadCubeMap("cubeMap"); // expects a folder name
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	cout << "init accomplished" << endl;
}

//draw

void draw_skybox(void)
{
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	// draw cube map

	glUseProgram(skyboxRenderingProgram);

	vLoc = glGetUniformLocation(skyboxRenderingProgram, "v_matrix");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));

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
	/*
	float vertCoordPositions[18] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, coordVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertCoordPositions), vertCoordPositions, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2,2);
	glDrawArrays(GL_LINES, 4, 2);
	*/
}
//todo

void draw_sphere(vector<Ball> &balls)
{
	glUseProgram(sphereRenderingProgram);
	for (const auto &iterator : balls)
	{

		//		if (iterator.counter() == 0)
		//			continue;

		mvLoc = glGetUniformLocation(sphereRenderingProgram, "mv_matrix");
		projLoc = glGetUniformLocation(sphereRenderingProgram, "proj_matrix");

		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		mMat = glm::translate(glm::mat4(1.0f), iterator.location()); //marker++++++++++++++++++++++++++++

		mvMat = vMat * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sphereTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
	}
}

void display(GLFWwindow *window, double currentTime, vector<Ball> &balls)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	draw_skybox();
	draw_sphere(balls);
}

void window_size_callback(GLFWwindow *win, int newWidth, int newHeight)
{
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void)
{
	cerr << "complier OK" << endl;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow *window = glfwCreateWindow(600, 600, "Phymodel", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	//priority_queue<Event> events;
	ifstream ifstrm("in.txt");
	ofstream ofstrm("out.txt");
	vector<Ball> balls;
	int numBalls;
	int round = 0;
	//check
	if (!ifstrm)
	{
		cerr << "open ifstrm err" << endl;
		exit(EXIT_FAILURE);
	}
	if (!(ifstrm >> numBalls))
	{
		cerr << "num err" << endl;
		cerr << ifstrm.eof() << ifstrm.bad() << ifstrm.fail() << ifstrm.good() << endl;
		exit(EXIT_FAILURE);
	}
	//read in
	for (int i = 0; i != numBalls; i++)
		balls.push_back(Ball(ifstrm));
	vecprint(cout, balls);

	//while------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		cout << "I'm here1" << endl;
		//print
		vecprint(cout, balls);
		//refresh
		for (auto &i : balls)
		{
			i.move(0.01f);
		}
		//examine
		for (int i = 0; i != numBalls; i++)
		{
			for (int j = i + 1; j != numBalls; j++)
			{
				float t = balls[i].timeToCollision(balls[j]);
				cout << "++++++++++++++++++++t:" << t << endl;
				if (t > 0 && t < 0.1f)
				{
					cout << "bounced::::::::::::::::::::::::::::" << endl;
					balls[i].handleCollision(balls[j]);
				}
			}
		}
		//display
		display(window, glfwGetTime(), balls);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}