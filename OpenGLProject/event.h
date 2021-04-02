#ifndef EVENT_H
#define EVENT_H
#include "Object.h"
#include <vector>
#include <iostream>
class Ball;
class Wall;

class Event//doing
{
    //friend class Event_mgr;

public:
    Event() = default;
    Event(Ball &ball_1, Ball &ball_2, float t)
        : type(0), ball1(ball_1), ball2(ball_2), 
        count1(ball_1.count), count2(ball_2.count), 
        time(t),wall(Wall()){}
    Event(Ball &ball, Wall &wall, float t)
        : type(1), ball1(ball), wall(wall), 
        count1(ball.count), 
        time(t){}
    void handle();

private:
    bool type;
    Ball &ball1, ball2; //小球引用
    const Wall &wall;
    int count1, count2;
    float time;        //事件发生绝对时间

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