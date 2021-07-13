//OpenGL使用的头文件
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
//#include <SOIL2\soil2.h>
//#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
//#include <glm\glm.hpp>
//#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
//#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include "Sphere.h"
//#include "Utils.h"
using namespace std;
using namespace chrono;

//自己选用的头文件
//#include <cmath>
//#include <sstream>
//#include <cstdlib>

//#include <iomanip>
//#include <vector>
//#include <queue>
#include "Object.h"
#include "Collision.h"

int main()
{
	ifstream ifstrm("testdata.txt");


	CollisionSystem system(ifstrm);
	auto start = system_clock::now();
	system.run(1.0f);
	auto end = system_clock::now();
	auto duration = duration_cast<milliseconds>(end - start);
	cout << duration.count();
	return 0;
}