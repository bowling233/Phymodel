#include <iostream>
#include <fstream>
#include <random>

using namespace std;

#define NUM 298//边个数
#define DISTANCE 3.353473e-9//间距
#define MASS 5.3e-26//质量
#define RADIUS 0.173e-9//半径
#define MEAN 0//均值
#define DELTA 2.667499e+02//方差

int main()
{
    ofstream ofstrm("testdata.txt");
    std::default_random_engine e;                        //引擎
    std::normal_distribution<double> n(MEAN,DELTA); 
    for (int i = 0; i != NUM; i++)
        for (int j = 0; j != NUM; j++)
            for (int k = 0; k != NUM; k++)
            {
                ofstrm 
                     << i * DISTANCE + DISTANCE / 2.0 << '\t'
                     << j * DISTANCE + DISTANCE / 2.0 << '\t'
                     << k * DISTANCE + DISTANCE / 2.0 << '\t'
                     << n(e) << '\t'
                     << n(e) << '\t'
                     << n(e) << '\t'
                     << MASS << '\t'
                     << RADIUS
                     << endl;
            }
    return 0;
}