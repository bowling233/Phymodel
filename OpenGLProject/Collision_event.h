#ifndef COLLISION_H
#define COLLISION_H

#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
//#include <GLFW\glfw3.h>
//constexpr auto DELTATIME = 0.1f;事件驱动不需要deltaTime
extern int sumbounce;
extern int sumexam;

//预先声明所有用到的外部类
class Object; 
class FixedBall; 
class Wall;
class Ball;

class Event;
class CollisionSystem;

class Event
{
    //io
    friend std::ostream &operator<<(std::ostream &, const Event &);

public:
    //construct
    Event(std::shared_ptr<Ball> b, std::shared_ptr<Object> o, const float t) : ball(b), object(o), time(t), countBall(b->cnt()), countObject(o->cnt()) {}

    //copy move destruct
    Event(const Event &) = default;
    Event(Event &&) = default;
    Event &operator=(const Event &) = default;
    Event &operator=(Event &&) = default;
    ~Event() = default;

    //information
    float t() { return time; }

    //action
    void handle();

    //compare
    bool operator<(const Event &event)const { return time < event.time; }

private:
    std::shared_ptr<Ball> ball;
    std::shared_ptr<Object> object;
    float time;
    unsigned int countBall, countObject;
};
std::ostream &operator<<(std::ostream &, const Event &);
std::ostream &operator<<(std::ostream &, std::priority_queue<Event, std::vector<Event>>); //time consuming，这里直接拷贝了整个数组，思考一下这是为什么

class CollisionSystem
{
    friend void display(GLFWwindow *window, double currentTime, CollisionSystem &system);
    friend std::istream &operator>>(std::istream &, CollisionSystem &);
    friend std::ostream &operator<<(std::ostream &, CollisionSystem &);

public:
    //construct and destruct
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    CollisionSystem(std::istream &is) : CollisionSystem() { is >> *this; }

    //action
    void run(float);
    void reverse();
    std::vector<std::shared_ptr<Ball>>& b() { return balls; }
    std::vector<std::shared_ptr<FixedBall>>& fb() { return fixedBalls; }
    std::vector<std::shared_ptr<Wall>>& w() { return walls; }

private:
    void init();
    void move(float);
    std::vector<std::shared_ptr<Ball>> balls;
    std::vector<std::shared_ptr<FixedBall>> fixedBalls;
    std::vector<std::shared_ptr<Wall>> walls;
    float currentTime = 0;
};
std::istream &operator>>(std::istream &, CollisionSystem &);
std::ostream &operator<<(std::ostream &, CollisionSystem &);

#endif