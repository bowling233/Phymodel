#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <cmath>

using namespace std;

int main()
{
    ofstream ofstrm("testdata.txt");
    std::default_random_engine e;                        //引擎
    std::normal_distribution<double> n(0, 2.664195e+02); //均值, 方差
    for (int i = 0; i != 16; i++)
        for (int j = 0; j != 16; j++)
            for (int k = 0; k != 16; k++)
            {
                ofstrm << i * 1.549733e-09 << ' '
                     << j * 1.549733e-09 << ' '
                     << k * 1.549733e-09 << ' '
                     << n(e) << ' '
                     << n(e) << ' '
                     << n(e) << ' '
                     << 5.313393e-26 << ' '
                     << 0.346e-9
                     << endl;
            }
    return 0;
}