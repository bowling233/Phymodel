//OpenGL使用的头文件
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
//#include <SOIL2\soil2.h>
#include <string>
//#include <sstream>
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

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

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
	freopen("result.txt", "w", stdout);
	string txt(".txt");
	string loca("E:\\Coding\\data\\4.1.");
	for(int i = 1;i!=10;i++)
	{
		for (int j = 1; j != 4; j++)
		{
			string s(loca + to_string(i) + '.' + to_string(j) + txt);
			cout << s << endl;
			ifstream ifstrm(s);
			CollisionSystem system(ifstrm);
			auto start = system_clock::now();
			system.run(10.0f);
			auto end = system_clock::now();
			auto duration = duration_cast<milliseconds>(end - start);
			cout << duration.count() << endl;
		}
	}
	
	return 0;
}