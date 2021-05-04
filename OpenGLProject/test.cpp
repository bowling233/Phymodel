#include <iostream>
#include "Object.h"
#include <glm\glm.hpp>
#include <vector>
//#include <queue>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include "Object.h"
#include "Collision.h"
#include <cstdio>

using namespace std;
#define _CRT_SECURE_NO_WARNINGS

int main()
{
    ifstream ifstrm("in.txt");
    ofstream ofstrm("out.txt");

    vector<shared_ptr<Ball>> balls;
    vector<shared_ptr<Wall>> walls;
    vector<shared_ptr<FixedBall>> fixedBalls;
    int round = 0;

    //check
    if (!ifstrm)
    {
        cerr << "open ifstrm err" << endl;
        exit(EXIT_FAILURE);
    }

    char identifier;


    int num;
    while (ifstrm >> identifier) //iden
    {

        if (!(ifstrm >> num)) //num
        {
            std::cerr << "num err" << std::endl;
            std::cerr << ifstrm.eof() << ifstrm.bad() << ifstrm.fail() << ifstrm.good() << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i != num; i++)
        {
            switch (identifier)
            {
            case 'B':
            {
                balls.push_back(make_shared<Ball>(ifstrm));
                break;
            }
            case 'W':
            {
                //walls.push_back(make_shared<Wall>(ifstrm));
                break;
            }
            case 'F':
            {
                fixedBalls.push_back(make_shared<FixedBall>(ifstrm));
                break;
            }
            default:
                break;
            }
        }
    }

    ofstrm << balls;
    ofstrm << fixedBalls;
    //ofstrm << fixedBalls;

    priority_queue<Event, vector<Event>> events;

    float t;
    for (auto i = balls.begin(); i != balls.end(); i++)
    {
        for (auto j = i + 1; j != balls.end(); j++)
        {
            //ball to ball
            t = (**i).predict(**j);
            ofstrm << "++++++++++++++++++++t:" << t << endl;
            if (t > 0 )
                events.push(Event(*i, *j, t));
        }
        t = (**i).predict(*fixedBalls[0]);
        if (t > 0 )
            events.push(Event(*i, fixedBalls[0], t));
    }

    ofstrm << "num event:" << events.size() << endl;

    ofstrm << events;

    float currentTime = 0;
    float deltaTime = 0.1;
    for (; currentTime < 20; currentTime += deltaTime)
    {
        ofstrm << "::::::::::::::::::::::::currentTime:" << currentTime<<endl;
        for (auto& i : balls)
            i->move(deltaTime);
        ofstrm << balls;
        while((!events.empty())&&(currentTime >= events.top().t()))
        {
            ofstrm << "!!!!!!!!!!!!!!!!!!!!!!!!!eventHandle" << endl;
            ofstrm << events.top() << endl;
            events.top().handle();
            events.pop();
        }
        ofstrm << events;
        ofstrm << endl << endl;
    }

    return 0;
}