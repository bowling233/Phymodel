#include "Collision.h"

std::ostream &operator<<(std::ostream &os, const Event &event)
{
    os << time <<std::endl;
    return os;
}
/*
std::istream operator>>(std::istream &is, CollisionSystem &system)
{
    {
        char identifier;
        int num;
        while (is >> identifier) //iden
        {

            if (!(is >> num)) //num
            {
                std::cerr << "num err" << std::endl;
                std::cerr << is.eof() << is.bad() << is.fail() << is.good() << std::endl;
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i != num; i)
            {
                switch (identifier)
                {
                case B:
                {
                    system.balls.push_back(Ball(is));
                    break;
                }
                case W:
                {
                    system.walls.push_back(Wall(is));
                    break;
                }
                case F:
                {
                    system.fixedBalls.push_back(FixedBall(is));
                    break;
                }
                default:
                    break;
                }
            }
        }
        std::cout << "System read in over::::::::::::::::" << std::endl;
    }
}

void CollisionSystem::run(float t)
{
    //todo
}

void CollisionSystem::reverse()
{
    for(auto&i:balls)
        i.reverse();
}
*/