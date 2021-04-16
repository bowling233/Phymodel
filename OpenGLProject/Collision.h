#ifndef COLLISION_H
#define COLLISION_H
#include "Object.h"
#include <vector>
#include <iostream>

class Object;
class FixedObject;
class FixedPoint;
class FixedBall;
class Wall;
class Point;
class Ball;
class Segment;
class Cuboid;
class Polygon;

class Event;
class Event_mgr;
class CollisionSystem;

class Event
{
public:
    //construct
    Event() : {}
    Event(const Object &obja, const Object &objb, const float t) : obj1(obja), obj2(objb), time(t), count1(obja.cnt()), count2(objb.cnt()) {}
    ~Event() :
    {
        obj1.~Object();
        obj2.~Object();
    }
    //information
    float t() { return time; }
    //action
    void handle(){
        
    }

private:
    Object obj1, obj2;
    float time;
    unsigned int count1, count2;
};

class Event_mgr
{
};

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

/*
class Ball;
class Wall;
class Event;
class CollisionSystem;

class Event //doing
{
    //friend class Event_mgr;

public:
    Event() = default;
    Event(Ball &ball_1, Ball &ball_2, float t)//球事件
        : type(0), ball1(ball_1), ball2(ball_2),
          count1(ball_1.count), count2(ball_2.count),
          time(t), wall(Wall()) {}
    Event(Ball &ball, Wall &wall, float t)//墙事件
        : type(1), ball1(ball), wall(wall),
          count1(ball.count),
          time(t) {}
    void handle();

private:
    bool type;
    Ball &ball1, ball2; //小球引用
    const Wall &wall;
    int count1, count2;
    float time; //事件发生绝对时间
};

/*todo
class Event_mgr
{ //事件队列维护类

public:
    using BallIndex = std::vector<Ball>::size_type;//元素下标

    
private:
    std::vector<Event> eventPQ; //默认情况下Event_mgr包含一个空的事件vector
};
*/

#endif