#ifndef COLLISION_H
#define COLLISION_H

#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <GLFW\glfw3.h>
#define DELTATIME 0.001f

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
    friend std::ostream &operator<<(std::ostream &, const Event &);

public:
    //construct
    Event(std::shared_ptr<Ball> b, std::shared_ptr<Object> o, const float t) : ball(b), object(o), time(t), countBall(b->cnt()), countObject(o->cnt()) { std::cout << "event created: " << *this << std::endl; }

    //copy move destruct
    Event(const Event &) = default;
    Event(Event &&) = default;
    Event &operator=(const Event &) = default;
    Event &operator=(Event &&) = default;
    ~Event() = default;

    //information
    float t() const { return time; }
    bool status() const { return (time >= 0) && (ball->cnt() == countBall) && (object->cnt() == countObject); }
    std::shared_ptr<Ball> b() const { return ball; }
    std::shared_ptr<Object> obj() const { return object; }

    //action
    void handle() const;

    //compare
    bool operator<(const Event &event) const { return time > event.time; }

private:
    std::shared_ptr<Ball> ball;
    std::shared_ptr<Object> object;
    float time;
    unsigned int countBall, countObject;
};
std::ostream &operator<<(std::ostream &, const Event &);

std::ostream &operator<<(std::ostream &, std::priority_queue<Event, std::vector<Event>>);

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

private:
    void init();
    void move(float);
    std::vector<std::shared_ptr<Ball>> balls;
    std::vector<std::shared_ptr<FixedBall>> fixedBalls;
    std::vector<std::shared_ptr<Wall>> walls;
    std::priority_queue<Event> eventQueue;
    float currentTime = 0;
};
std::istream &operator>>(std::istream &, CollisionSystem &);
std::ostream &operator<<(std::ostream &, CollisionSystem &);

#endif