#ifndef COLLISION_H
#define COLLISION_H

#define EVENT_DRIVEN

#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <GLFW\glfw3.h> //for GLFW*WINDOWS
extern int sumbounce;
extern int sumexam;

#ifdef TIME_DRIVEN
constexpr auto DELTATIME = 1.0f / 60.0f; //时间驱动专用
#endif

//预先声明所有用到的外部类
class Object;
class MovableObject;
class Wall;
class Ball;

#ifdef EVENT_DRIVEN
class Event;
#endif

class CollisionSystem;

#ifdef EVENT_DRIVEN
class Event
{
    //io
    friend std::ostream &operator<<(std::ostream &, const Event &);
    friend CollisionSystem;

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
    float t() const { return time; }

    //action
    void handle () const;

    //compare
    bool operator<(const Event &event) const { return time < event.time; }
    bool valid() const;

private:
    std::shared_ptr<Ball> ball;
    std::shared_ptr<Object> object;
    float time;
    unsigned int countBall, countObject;
};
std::ostream &operator<<(std::ostream &, const Event &);
std::ostream &operator<<(std::ostream &, std::priority_queue<Event, std::vector<Event>>); //time consuming，这里直接拷贝了整个数组，思考一下这是为什么
#endif

class CollisionSystem
{
    //friend void display(GLFWwindow *window, double currentTime, CollisionSystem &system);
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
    std::vector<std::shared_ptr<Ball>> &b() { return balls; }
    std::vector<std::shared_ptr<Wall>> &w() { return walls; }
    void move(float);

private:
    void init();
    std::vector<std::shared_ptr<Ball>> balls;
    std::vector<std::shared_ptr<Wall>> walls;
    float currentTime = 0;

#ifdef EVENT_DRIVEN
    std::priority_queue<Event, std::vector<Event>> eventQueue; //事件驱动队列专用
#endif
};
std::istream &operator>>(std::istream &, CollisionSystem &);
std::ostream &operator<<(std::ostream &, CollisionSystem &);

#endif