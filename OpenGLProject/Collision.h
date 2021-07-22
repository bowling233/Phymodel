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
    friend std::ostream& operator<<(std::ostream&, const Event&);
    friend CollisionSystem;
    friend Event_mgr;

public:
    //construct
    Event(std::shared_ptr<Ball> b, std::shared_ptr<Object> o, const double t) : ball(b), object(o), time(t), countBall(b->cnt()), countObject(o->cnt()) {}
    Event() = default;

    //copy move destruct
    Event(const Event&) = default;
    Event(Event&&) = default;
    Event& operator=(const Event&) = default;
    Event& operator=(Event&&) = default;
    ~Event() = default;

    //information
    double t() const { return time; }

    //action
    void handle() const;

    //compare
    bool operator<(const Event& event) const { return time < event.time; }
    bool operator>(const Event& event) const { return time > event.time; }
    bool valid() const;

private:
    std::shared_ptr<Ball> ball=nullptr;
    std::shared_ptr<Object> object=nullptr;
    double time = INFINITY;
    unsigned int countBall = 0, countObject = 0;
};
std::ostream &operator<<(std::ostream &, const Event &);
//std::ostream &operator<<(std::ostream &, std::priority_queue<Event, std::vector<Event>>); //time consuming，这里直接拷贝了整个数组，思考一下这是为什么

class Event_mgr
{
    friend std::ostream& operator<<(std::ostream&, Event_mgr&);
public:
    Event_mgr() = delete;
    Event_mgr(int capacity) : maxN(capacity)
    {
        events = new Event[maxN + 1];
        pq = new int[maxN + 1];
        qp = new int[maxN + 1];
        for (int i = 0; i <= maxN; i++)
            qp[i] = -1;
    };

    //copy move destruct
    ~Event_mgr() {
        delete[] events; delete[] pq; delete[] qp;
    }

    //information
    bool empty() { return N == 0; }
    bool contains(int k)
    {
        if (k <= maxN)
            return (qp[k] != -1);
        else {
            std::cout << "contains:error:"<<k;
            exit(EXIT_FAILURE);
        }
    }
    Event &top() { return events[pq[1]]; }
    int size() { return N; }

    //action
    void push(const Event &);
    void pop();
    void init() {
        delete[] events; delete[] pq; delete[] qp;
        events = new Event[maxN + 1];
        pq = new int[maxN + 1];
        qp = new int[maxN + 1];
        for (int i = 0; i <= maxN; i++)
            qp[i] = -1;
    };

private:
    Event *events;
    int *pq;
    int *qp;
    int maxN;
    int N = 0;
    bool less(int i, int j) { return events[pq[i]] < events[pq[j]]; }
    bool more(int i, int j) { return events[pq[i]] > events[pq[j]]; }
    void exch(int, int);
    void swim(int);
    void sink(int);
};

std::ostream& operator<<(std::ostream&, Event_mgr&);

#endif

class CollisionSystem
{
    //friend void display(GLFWwindow *window, double currentTime, CollisionSystem &system);
    friend std::istream &operator>>(std::istream &, CollisionSystem &);
    friend std::ostream &operator<<(std::ostream &, CollisionSystem &);

public:
    //construct and destruct
    //CollisionSystem() = delete; //其实并没有用到默认构造函数，因此删除
    ~CollisionSystem() = default;
    CollisionSystem(int maxN, std::istream &is):eventQueue(maxN)
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
    Event_mgr& e() { return eventQueue; }
#endif // EVENT_DRIVEN

private:
    void move(double);
    void init();

    std::vector<std::shared_ptr<Ball>> balls;
    std::vector<std::shared_ptr<Container>> containers;
    double currentTime = 0.0, targetTime = 0.0, temp = 0.0; //temp:各种计算

#ifdef EVENT_DRIVEN
    //std::priority_queue<Event, std::vector<Event>> eventQueue; //事件驱动队列专用
    Event_mgr eventQueue;
    std::shared_ptr<Ball> tempball; //temp:副小球检测
    Event tempEvent;
#endif
};
std::istream &operator>>(std::istream &, CollisionSystem &);
std::ostream &operator<<(std::ostream &, CollisionSystem &);

#endif