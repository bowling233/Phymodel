#include "Collision.h"
#include "Object.h"
#include <iomanip>
#include <memory>
int sumbounce = 0;
int sumexam = 0;

void CollisionSystem::run(float t)
{
    for (float targetTime = currentTime + t; currentTime < targetTime;)
    {
        for (auto i = balls.cbegin(); i != balls.cend(); i++)
            for (auto j = i + 1; j != balls.cend(); j++) {
                sumexam++;
                if ((**i).examine(**j))
                {
                    (**i).bounce(**j); sumbounce++;
                }
            }
        move(DELTATIME);
    }
}

void CollisionSystem::reverse()
{
    for (auto &i : balls)
        i->rev();
}

void CollisionSystem::move(float t)
{
    for (auto &i : balls)
        i->move(t);
    currentTime += t;
}

void CollisionSystem::init()
{
    for (auto i = balls.cbegin(); i != balls.cend(); i++)
    {
        for (auto j = i + 1; j != balls.cend(); j++)
            if ((**i).examine(**j))
            {
                std::cerr << "init error" << std::endl;
                exit(EXIT_FAILURE);
            }
    }
}

std::istream &operator>>(std::istream &is, CollisionSystem &system)
{
    char identifier;
    int num;
    while (is >> identifier) //iden
    {

        if (!(is >> num)) //num
        {
            std::cerr << "input err" << std::endl;
            //std::cerr << is.eof() << is.bad() << is.fail() << is.good() << std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i != num; i++)
        {
            switch (identifier)
            {
            case 'B':
            {
                system.balls.push_back(std::make_shared<Ball>(is));
                break;
            }
            case 'W':
            {
                system.walls.push_back(std::make_shared<Wall>(is));
                break;
            }
            case 'F':
            {
                system.fixedBalls.push_back(std::make_shared<FixedBall>(is));
                break;
            }
            }
        }
    }
    std::clog << "System read in over::::::::::::::::" << std::endl;
    system.init();
    return is;
}

std::ostream &operator<<(std::ostream &os, CollisionSystem &system)
{
    os << system.balls << system.fixedBalls << system.walls;
    return os;
}