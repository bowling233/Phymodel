#include <iostream>
#include <fstream>
#include <random>

using namespace std;

#define NUM 89//边个数
#define DISTANCE 3.353473e-9//间距
#define MASS 5.3e-26//质量
#define RADIUS 0.173e-9//半径
#define MEAN 0.0//均值
#define DELTA 2.667499e2//方差

int main()
{
    ofstream ofstrm("testdata.txt");
    std::default_random_engine e;                        //引擎
    std::normal_distribution<double> n(MEAN,DELTA);
    e.seed(time(0));
    double origin = -(DISTANCE* (NUM - 1))/2.0;
    double distance = DISTANCE;
    ofstrm << "C 1" << endl
           << 0 << '\t' << 0 << '\t' << 0 << endl
           << 0.3e-6 << '\t' << 0.3e-6 << '\t' << 0.3e-6 << endl;
    ofstrm << "B " << NUM * NUM * NUM << endl;
    for (int i = 0; i != NUM; i++)
        for (int j = 0; j != NUM; j++)
            for (int k = 0; k != NUM; k++)
            {
                ofstrm 
                     << origin + i * distance << '\t'
                     << origin + j * distance << '\t'
                     << origin + k * distance << '\t'
                     << n(e) << '\t'
                     << n(e) << '\t'
                     << n(e) << '\t'
                     << MASS << '\t'
                     << RADIUS
                     << endl;
            }
    return 0;
}