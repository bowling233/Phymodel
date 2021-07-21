#ifndef COLLISION_H
#define COLLISION_H

#define EVENT_DRIVEN

#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
extern int sumbounce;
extern int sumexam;

#ifdef TIME_DRIVEN
constexpr auto DELTATIME = 1.0 / 3600.0; //时间驱动专用
#endif

#ifdef EVENT_DRIVEN
constexpr auto TIME_LIMIT = 10.0;
#define CAPACITY = 512
#endif

//预先声明所有用到的外部类
class Object;
class Ball;

#ifdef EVENT_DRIVEN
class Event;
class Event_mgr;
#endif

class CollisionSystem;

#ifdef EVENT_DRIVEN
class Event
{
    //io
    friend std::ostream &operator<<(std::ostream &, const Event &);
    friend CollisionSystem;
    friend Event_mgr;
public:
    //construct
    Event(std::shared_ptr<Ball> b, std::shared_ptr<Object> o, const double t) : ball(b), object(o), time(t), countBall(b->cnt()), countObject(o->cnt()) {}
    Event() = default;

    //copy move destruct
    Event(const Event &) = default;
    Event(Event &&) = default;
    Event &operator=(const Event &) = default;
    Event &operator=(Event &&) = default;
    ~Event() = default;

    //information
    double t() const { return time; }

    //action
    void handle() const;

    //compare
    bool operator<(const Event &event) const { return time < event.time; }
    bool valid() const;

private:
    std::shared_ptr<Ball> ball;
    std::shared_ptr<Object> object;
    double time;
    unsigned int countBall, countObject;
};
std::ostream &operator<<(std::ostream &, const Event &);
std::ostream &operator<<(std::ostream &, std::priority_queue<Event, std::vector<Event>>); //time consuming，这里直接拷贝了整个数组，思考一下这是为什么

class Event_mgr
{
public:
    Event_mgr(int capa) : capacity(capa) {
        events = new Event[capacity + 1];
        pq = new int[capacity + 1];
        qp = new int[capacity + 1];
        for (int i = 0; i <= capacity; i++)qp[i] = -1;
    };
    Event_mgr() = default;

    //copy move destruct
    Event_mgr(const Event_mgr&) = default;
    Event_mgr(Event_mgr&&) = default;
    Event_mgr& operator=(const Event_mgr&) = default;
    Event_mgr& operator=(Event_mgr&&) = default;
    ~Event_mgr() = default;

    bool empty() { return N == 0; }
    bool contains(int k) { if (k < N)return qp[k] != -1; else return false; }
    void push(const Event&  event) {
    int k = event.ball->num(); 
    if (contains(k)) { events[k] = event; swim(qp[k]); sink(qp[k]); }
    else { N++; qp[k] = N; pq[N] = k; events[k] = event; swim(N); }
    }
    Event& top() { return events[pq[1]]; }
    void pop() { int indexOfMin = pq[1]; exch(1, N--); sink(1); qp[pq[N + 1]] = -1; }
    int size() { return N; }
private:
    Event* events;
    int* pq;
    int* qp;
    int capacity,N;
    bool less(int i, int j) { return events[pq[i]] < events[pq[j]]; }
    void exch(int i, int j) {
        qp[0] = qp[pq[j]]; qp[pq[j]] = qp[pq[i]]; qp[pq[i]] = qp[0];
        pq[0] = pq[i]; pq[i] = pq[j]; pq[j] = pq[0]; }
    void swim(int k) { while (k > 1 && less(k / 2, k)) { exch(k / 2, k); k = k / 2; } }
    void sink(int k) { while (2 * k <= capacity) { int j = 2 * k; if (j < capacity && less(j, j + 1))j++;if (!less(k, j))break; exch(k, j); k = j; } }
};

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
    CollisionSystem(std::istream &is) : CollisionSystem()
    {
        is >> *this;
        this->init();
    } //输入后自动初始化

    //action
    void run(double);
    void reverse();

    //information
    double time() { return currentTime; }
    double ek();
    std::vector<std::shared_ptr<Ball>> &b() { return balls; }
    std::vector<std::shared_ptr<Container>> &c() { return containers; }
#ifdef EVENT_DRIVEN
    //std::priority_queue<Event, std::vector<Event>>& e() { return eventQueue; }
#endif // EVENT_DRIVEN

private:
    void move(double);
    void init();

#ifdef EVENT_DRIVEN
    std::shared_ptr<Ball> tempball; //temp:副小球检测
    Event tempEvent;
#endif
    std::vector<std::shared_ptr<Ball>> balls;
    std::vector<std::shared_ptr<Container>> containers;
    double currentTime = 0.0, targetTime = 0.0, temp = 0.0; //temp:各种计算
#ifdef EVENT_DRIVEN
    //std::priority_queue<Event, std::vector<Event>> eventQueue; //事件驱动队列专用
    Event_mgr eventQueue;
#endif
};
std::istream &operator>>(std::istream &, CollisionSystem &);
std::ostream &operator<<(std::ostream &, CollisionSystem &);

#endif