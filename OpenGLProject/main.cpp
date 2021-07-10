#include <string>
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include "Sphere.h"
//#include "Utils.h"
using namespace std;

//自己选用的头文件
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <chrono>
//#include <iomanip>
#include <vector>
#include <queue>
#include "Object.h"
#include "Collision.h"
using namespace chrono;
//关闭vs studio错误提示
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)


int main(void)
{
	freopen("time_out.txt", "w", stdout);
//OpenGL初始化
	

	
//创建碰撞系统
	ifstream ifstrm("E:\\Coding\\testdata\\605ball.txt");
	//ofstream ofstrm("out.txt");

	CollisionSystem system(ifstrm);
	auto last = system_clock::now();
	auto current = system_clock::now();
	auto duration = duration_cast<microseconds>(current - last);
	unsigned int count = 0;
	bool flag = true;
	
	system.run(60.0f);

	
	cout << system;

	exit(EXIT_SUCCESS);
}