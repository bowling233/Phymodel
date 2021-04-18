#include <iostream>
#include "Object.h"
#include <glm\glm.hpp>
#include <vector>
//#include <queue>
#include <fstream>
#include <iomanip>

using namespace std;



int main()
{
    vector<Ball> balls;
    for (int i = 0; i != 2; i++)
        balls.push_back(Ball());

    cout << balls;
    return 0;
}