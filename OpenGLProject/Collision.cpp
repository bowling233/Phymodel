#include "Collision.h"
#include "Object.h"
#include <iomanip>
#include <memory>

bool Event::handle() const //notice
{
    std::cout << "::::::::::::::::::::::::::::::Event handle: " << *this << std::endl;
    if ((time >= 0) && (ball->cnt() == countBall) && (object->cnt() == countObject))
    {
        std::cout << "event check: "<< ball->cnt()<<" "<<object->cnt() << std::endl;
        ball->bounce(*object);
        return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const Event &event)
{
    os << std::setprecision(3) << std::fixed;
    os << std::setw(8) << event.time
       << " | " << std::setw(4) << event.ball->num()
       << " | " << std::setw(3) << event.countBall
       << " | " << std::setw(10);
    switch (event.object->type())
    {
    case Object_type::FIXEDBALL:
        os << "fixedball";
        break;
    case Object_type::BALL:
        os << "ball";
        break;
    case Object_type::WALL:
        os << "wall";
        break;
    }
    os << " | " << std::setw(3) << event.object->num()
       << " | " << std::setw(3) << event.countObject
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::priority_queue<Event, std::vector<Event>> eventqueue) //time consuming
{
    os << "---------------Events-------------------------" << std::endl;
    os << "   time  | Ball | cnt | Obj_type  | Obj | cnt"
       //"   1.000 |    1 |   3 | fixedball |   1 |   3"
       << std::endl;
    while (!eventqueue.empty())
    {
        os << eventqueue.top() << std::endl;
        eventqueue.pop();
    }
    return os;
}
/*
std::ostream & operator>>(std::ostream &os,CollisionSystem &system)
{

return os;
}
std::istream& operator>>(std::istream &is, CollisionSystem &system)
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