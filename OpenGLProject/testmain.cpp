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
    FixedPoint pointone;
    vector<Point> points;
    for(int i = 0;i!=5;i++)
    points.push_back(Point());
    for (auto& i : points)
        cout << i.judge(pointone) << endl;
    points.pop_back();
    for (auto& i : points)
        cout << i.judge(pointone) << endl;

    cout << pointone.loc().x << endl;
    return 0;
}