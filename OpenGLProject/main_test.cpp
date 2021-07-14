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

extern int sumbounce;

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
	//freopen("result.txt", "w", stdout);

	string txt(".txt");
	string data("E:\\Coding\\data\\4.1.");
	for(int t = 1;t!=30;t+=5)
		for(int i = 4;i!=5;i++)
			for (int j = 1; j != 4; j++)
		{
			string s(data + to_string(i) + '.' + to_string(j) + txt);
			cout << s << endl;
			string out(to_string(t) + '.' + to_string(j) + txt);
			ifstream ifstrm(s);
			ofstream ofstrm(out);
			CollisionSystem system(ifstrm);
			system.run(t);
			system.reverse();
			system.run(t);
			ofstrm << system;
			ofstrm << sumbounce << endl;
			sumbounce = 0;
		}
	
	
	return 0;
}