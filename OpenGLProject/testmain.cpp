#include <iostream>
#include "Ball.h"
//#include "Event.h"
#include <glm\glm.hpp>
#include <vector>
//#include <queue>
#include <fstream>
#include <iomanip>


using namespace std;

int main()
{
    
        vector<Ball>
        balls;
    int numBalls=5;

    ifstream ifstrm("in.txt");
    if (!ifstrm)
    {
        cerr << "open ifstrm err" << endl;
        exit(EXIT_FAILURE);
    }
    if (!(ifstrm >> numBalls)) //发现问题：UFT8输入文件无法被读取，输出文件却是uft8的。查询得知可能是宽字符流的问题
    {
        cerr << "num err" << endl;   
        cerr << ifstrm.eof() << ifstrm.bad() << ifstrm.fail() << ifstrm.good() << endl;
        exit( EXIT_FAILURE);   
    }
    for (int i = 0; i != numBalls; i++)
    {
        balls.push_back(Ball(ifstrm));
    }
    
   while(1)
    {
        vecprint(cout, balls);
        for (auto &indi : balls)
            indi.move(0.01f);
        cout<<"t"<<balls[0].toBall(balls[1])<<endl;
    }
    return 0;
}