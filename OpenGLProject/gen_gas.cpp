#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <cmath>

using namespace std;
#define LINE 16//边排列个数
#define NUM 1024//小球个数
#define DISTANCE 1.549733e-09//小球间距
#define MASS 1.0//小球质量
#define RADIUS 0.5//小球半径
#define MEAN 0//均值
#define DELTA 2.664195e+02//方差

int main()
{
    ofstream ofstrm("testdata.txt");
    std::default_random_engine e;                        //引擎
    std::normal_distribution<double> n(MEAN,DELTA); 
    for (int i = 0; i != LINE; i++)
        for (int j = 0; j != LINE; j++)
            for (int k = 0; k != LINE; k++)
            {
                ofstrm << i * DISTANCE << ' '
                     << j * DISTANCE << ' '
                     << k * DISTANCE << ' '
                     << n(e) << ' '
                     << n(e) << ' '
                     << n(e) << ' '
                     << MASS << ' '
                     << RADIUS
                     << endl;
            }
    return 0;
}