#include <iostream>
#include <fstream>
#include <random>

using namespace std;

#define NUM 16//边个数
#define DISTANCE 0.5//间距
#define MASS 1//质量
#define RADIUS 0.5//半径
#define MEAN 0//均值
#define DELTA 1//方差

int main()
{
    ofstream ofstrm("testdata.txt");
    std::default_random_engine e;                        //引擎
    std::normal_distribution<double> n(MEAN,DELTA);
    e.seed(time(0));
    double origin = -(RADIUS + DISTANCE / 2.0) * (NUM - 1);
    double distance = 2 * RADIUS + DISTANCE;
    ofstrm << "C 1" << endl
           << 0 << '\t' << 0 << '\t' << 0 << endl
           << 1.5 * (NUM + 1) << '\t' << 1.5 * (NUM + 1) << '\t' << 1.5 * (NUM + 1) << endl;
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