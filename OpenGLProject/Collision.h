#ifndef COLLISION_H
#define COLLISION_H
#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>

class Event;
class CollisionSystem;

class Event
{
    //io
    friend std::ostream operator<<(std::ostream &, const Event &);

public:
    //construct
    Event() : {}
    Event(const MovableObject&obj1, const Object &obj2, const float t) : object1(obj1), object2(obj2), time(t), count1(obj1.cnt()), count2(objb.cnt()) {}
    ~Event() = default;
    //information
    float t() { return time; }
    //action
    void handle(); //extern

    //operator
    bool operator<(const Evnet &event) { return (this->time) < event.time; }

private:
    Ball &object1;
    
    float time;
    unsigned int count1, count2;
};

std::ostream operator<<(std::ostream &os, const Event &event) //todo
{
    os << std::endl;
}

class CollisionSystem
{
public:
    CollisionSystem(std::istream &is) : CollisionSystem()
    {
        int numBalls, numWalls;
        if (!is)
        {
            cerr << "open is err" << endl;
            exit(EXIT_FAILURE);
        }
        if (!(is >> numBalls))
        {
            cerr << "num err" << endl;
            cerr << is.eof() << is.bad() << is.fail() << is.good() << endl;
            exit(EXIT_FAILURE);
        }
        //read in balls
        for (int i = 0; i != numBalls; i++)
            this->balls.push_back(Ball(is));
        if (!(is >> numWalls))
        {
            cerr << "num err" << endl;
            cerr << is.eof() << is.bad() << is.fail() << is.good() << endl;
            exit(EXIT_FAILURE);
        }
        //read in walls
        for (int i = 0; i != numWalls; i++)

            this->walls.push_back(Wall(is));
        std::cout << "System created::::::::::::::::" << std::endl;
    }

private:
    std::vector<Ball> balls;
    std::vector<Wall> walls;
}

#endif