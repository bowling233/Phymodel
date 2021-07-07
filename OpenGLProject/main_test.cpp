//OpenGL使用的头文件
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
//#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
//#include <glm\glm.hpp>
//#include <glm\gtc\type_ptr.hpp>			// glm::value_ptr
//#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include "Sphere.h"
//#include "Utils.h"
using namespace std;

//自己选用的头文件
#include <cmath>
//#include <sstream>
//#include <cstdlib>
//#include <chrono>
//#include <iomanip>
//#include <vector>
//#include <queue>
#include "Object.h"
#include "Collision.h"

int main()
{
	ifstream ifstrm("\testdata\test_for_ball.txt");
	std::vector<std::shared_ptr<Ball>> balls;
	balls.push_back(std::make_shared<Ball>(ifstrm));
	cout << balls;
	return 0;
}