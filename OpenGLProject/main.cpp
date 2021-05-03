#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
using namespace std;

#include <cmath>
#include <sstream>
#include <cstdlib>
#include <chrono>
//#include <iomanip>
#include <vector>
#include <queue>
#include "Object.h"
#include "Collision_time.h"
using namespace chrono;
//#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable : 4996)

#define numVAOs 1

float cameraX, cameraY, cameraZ;
GLuint skyboxRenderingProgram,
	coordRenderingProgram,
	sphereRenderingProgram,
	planeRenderingProgram;
GLuint vao[numVAOs];
GLuint skyboxVbo[2], coordVbo[1], sphereVbo[5], planeVbo[3];
GLuint skyboxTexture, sphereTexture, planeTexture;
float amt = 0.0f; //tochk

Sphere mySphere = Sphere(48);
glm::vec3 lightLoc = glm::vec3(5.0f, 2.0f, 2.0f);

//allocation
GLuint mvLoc, projLoc, vLoc, sLoc, nLoc;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc,
	posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, lMat, invTrMat, rMat;
glm::vec3 currentLightPos, transformed;
float lightPos[3];

// white light
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material
float* matAmb = Utils::goldAmbient();
float* matDif = Utils::goldDiffuse();
float* matSpe = Utils::goldSpecular();
float matShi = Utils::goldShininess();

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }


void installLights(GLuint renderingProgram,glm::mat4 vMatrix) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
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



//setupvert-----------------------------------------------------------------------------------------------------------
void setupVert_sphere(vector<shared_ptr<Ball>>& balls)
{//VBO:0.顶点1.法向量2.缩放3.位置
	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> nvalues;
	std::vector<float> scales;

	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	for (auto const & i:balls)
		scales.push_back(i->r());

	glGenBuffers(4, sphereVbo);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[2]);
	glBufferData(GL_ARRAY_BUFFER, scales.size() * sizeof(float), &scales[0], GL_STATIC_DRAW);
	glVertexAttribDivisor(2, 1);
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
void init(GLFWwindow *window,CollisionSystem& system)
{
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	//skyboxRenderingProgram = Utils::createShaderProgram("vs_skybox.glsl", "fs_skybox.glsl");
	coordRenderingProgram = Utils::createShaderProgram("vs_coord.glsl", "fs_coord.glsl");
	sphereRenderingProgram = Utils::createShaderProgram("vs_sphere.glsl", "fs_sphere.glsl");
	//	planeRenderingProgram = Utils::createShaderProgram("vs_plane.glsl", "fs_plane.glsl");
	cerr << "init passed" << endl;
	cameraX = -5.0f;
	cameraY = -5.0f;
	cameraZ = 10.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	lMat = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(7.5f, 7.5f, 7.5f), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

	setupVert_sphere(system.b());
	//setupVert_skybox();
	setupVert_coord();
	//sphereTexture = Utils::loadTexture("earth.jpg");
	//skyboxTexture = Utils::loadCubeMap("cubeMap"); // expects a folder name
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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

//uniform
	vMat = lMat;
	glUniformMatrix4fv(glGetUniformLocation(sphereRenderingProgram, "v_matrix"), 1, GL_FALSE, glm::value_ptr(vMat));
	projLoc = glGetUniformLocation(sphereRenderingProgram, "proj_matrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	currentLightPos = glm::vec3(lightLoc.x, lightLoc.y, lightLoc.z);
	amt += 0.5f;
	rMat = glm::rotate(glm::mat4(1.0f), toRadians(amt), glm::vec3(0.0f, 0.0f, 1.0f));
	currentLightPos = glm::vec3(rMat * glm::vec4(currentLightPos, 1.0f));

	installLights(sphereRenderingProgram,vMat);

//VBO0:vert
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
//VBO1:norm
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	std::vector<glm::vec3> models;//vec3
	for (auto const&i:balls) 
		models.push_back(i->loc());

//VBO3:model
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[3]);//传送模型位置
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::vec3), &models[0], GL_DYNAMIC_DRAW);//直接刷新缓冲区
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);//配置实例化更新数据

//VBO2:scale
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo[2]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

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
	draw_sphere(system.balls);
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

	

	ifstream ifstrm("testdata\\605ball.txt");
	ofstream ofstrm("out.txt");

	CollisionSystem system(ifstrm);
	auto last = system_clock::now();
	auto current = system_clock::now();
	auto duration = duration_cast<microseconds>(current - last);
	unsigned int count = 0;
	init(window,system);
	
	while (!glfwWindowShouldClose(window))
	{
		
		if (count++ == 30) {
			last = current;
			current = system_clock::now();
			duration = duration_cast<microseconds>(current - last);
			cout << "fps::"
				<< 30.0/(double(duration.count()) * microseconds::period::num / microseconds::period::den)
				<< endl
				<< "bounce persecond::" 
				<< sumbounce / (double(duration.count()) * microseconds::period::num / microseconds::period::den) 
				<< endl
				<< "exam persecond::"
				<< sumexam / (double(duration.count()) * microseconds::period::num / microseconds::period::den)
				<< endl;
			count = 0;
			sumbounce = 0;

		}
		
		system.run(1.0f/60.0f);

	//display
		display(window, glfwGetTime(), system);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
		
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}