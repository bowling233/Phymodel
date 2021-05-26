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
    std::normal_distribution<double> n(0, 2.664195); //均值, 方差
    for (int i = 0; i != 32; i++)
        for (int j = 0; j != 32; j++)
            for (int k = 0; k != 32; k++)
            {
                ofstrm << i * 5 << ' '
                     << j * 5 << ' '
                     << k * 5 << ' '
                     << n(e) << ' '
                     << n(e) << ' '
                     << n(e) << ' '
                     << 1 << ' '
                     << 1
                     << endl;
            }
    return 0;
}