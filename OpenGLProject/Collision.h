#ifndef COLLISION_H
#define COLLISION_H
#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>
#include <queue>

class Object; //ac
class MovableObject;
class FixedBall; //ac
class Wall;
class Ball;

class Event;
class CollisionSystem;

class Event
{
    //io
    friend std::ostream operator<<(std::ostream &, const Event &);

public:
    //construct
    Event(const MovableObject &obj1, const Object &obj2, const float t) : object1(obj1), object2(obj2), time(t), count1(obj1.cnt()), count2(objb.cnt()) {}
    ~Event() = default;
    //information
    float t() { return time; }
    //action
    void handle(); //extern

    //operator
    bool operator<(const Evnet &event) { return (this->time) < event.time; }

private:
    Ball &ball;
    Object &object;
    enum event_type
    {
        FB,
        W,
        B,
        U
    } type;
    float time;
    unsigned int countBall, countObject;
};

std::ostream operator<<(std::ostream &os, const Event &event) //todo
{
    os << std::endl;
    return os;
}

#ifdef DEBUG
class CollisionSystem
{
public:
    CollisionSystem(std::istream &is) : CollisionSystem() {}

    //io
    void readIn(std::istream &is)
    {
        char identifier;
        int num;

        if (!is)
        {
            std::cerr << "is open err" << std::endl;
            exit(EXIT_FAILURE);
        }
        while (is >> identifier) //iden
        {
            if (!(is >> num)) //num
            {
                std::cerr << "num err" << std::endl;
                std::cerr << is.eof() << is.bad() << is.fail() << is.good() << std::endl;
                exit(EXIT_FAILURE);
            }

            switch (identifier)
            {
            case A:
            {
                Ball tempBall;
                for (int i = 0; i != num; i++)
                {
                    is >> tempBall;
                    balls.push_back(tempBall);
                }
                break;
            }
                //todo
            default:
                break;
            }
        }

        
        std::cout << "System read in over::::::::::::::::" << std::endl;
    }

private:
    std::vector<Ball> balls;
    std::vector<shared_ptr<Object>> objects;
    std::priority_queue<Event> events;
    float currentTime;
}

#endif

#endif