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
constexpr auto DELTATIME = 1.0f / 360.0f; //时间驱动专用
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
    bool operator<(const Event &event) const { return time > event.time; }
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
    CollisionSystem(std::istream& is) : CollisionSystem() { is >> *this; this->init(); }//输入后自动初始化

    //action
    void run(float);
    void reverse();
    
    //information
    float time() { return currentTime; }
    float ek();
    std::vector<std::shared_ptr<Ball>>& b() { return balls; }
    std::vector<std::shared_ptr<Ball>>& hb() { return hidden_balls; }
    std::vector<std::shared_ptr<Wall>>& w() { return walls; }
    std::vector<std::shared_ptr<Container>>& c() { return containers; }
    std::priority_queue<Event, std::vector<Event>>& e() { return eventQueue; }

private:
    void move(float);
    void init();

#ifdef EVENT_DRIVEN
    std::shared_ptr<Ball> tempball;//temp:副小球检测
#endif
    std::vector<std::shared_ptr<Ball>> balls, hidden_balls;
    std::vector<std::shared_ptr<Wall>> walls;
    std::vector<std::shared_ptr<Container>> containers;
    float currentTime = 0.0, targetTime=0.0, temp = 0.0;//temp:各种计算

#ifdef EVENT_DRIVEN
    std::priority_queue<Event, std::vector<Event>> eventQueue; //事件驱动队列专用
#endif
};
std::istream &operator>>(std::istream &, CollisionSystem &);
std::ostream &operator<<(std::ostream &, CollisionSystem &);

#endif