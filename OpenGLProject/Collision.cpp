#include "Collision.h"
#include "Object.h"
#include <iomanip>
#include <memory>

int sumbounce = 0;
int sumexam = 0;

//#############
//Event
//#############
#ifdef EVENT_DRIVEN
void Event::handle() const //notice
{
    std::cout << "info:事件处理信息：" << std::endl << *this << std::endl;//<debug>
    if ((ball->cnt() == countBall) && (object->cnt() == countObject))
        ball->bounce(*object); //从指针变成对象的引用，交给ball类运行时绑定
}

bool Event::valid() const
{
    //std::clog << "info:事件有效性检测\n" << *this << '\t' << ball->cnt() << '\t' << object->cnt() << std::endl;//debug
    return ((ball->cnt() == countBall) && (object->cnt() == countObject));
}

std::ostream &operator<<(std::ostream &os, const Event &event)
{
    os << std::setprecision(3) << std::fixed;
    os << event.time
       << '\t' << event.ball->num()
       << '\t' << event.countBall
       << '\t';
    switch (event.object->type())
    {
    case Object_type::BALL:
        os << "ball";
        break;
    case Object_type::WALL:
        os << "wall";
        break;
    case Object_type::CONTAINER:
        os << "cont";
        break;
    }
    os << '\t' << event.object->num()
       << '\t' << event.countObject
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::priority_queue<Event, std::vector<Event>> eventqueue) //time consuming
{
    os << "---------------Events-------------------------" << std::endl;
    os << "time\tBall\tcnt\tObj_typ\tObj_num\tObj_cnt"
       << std::endl;
    while (!eventqueue.empty())
    {
        os << eventqueue.top() << std::endl;
        eventqueue.pop();
    }
    return os;
}
#endif

//#############
//CollisionSystem
//#############

void CollisionSystem::run(float t)
{
    float targetTime = currentTime + t;
#ifdef TIME_DRIVEN
    while (currentTime < targetTime)
    {
        for (auto i = balls.cbegin(); i != balls.cend(); i++)
        {
            for (auto j = i + 1; j != balls.cend(); j++)
            {
                sumexam++;
                if ((**i).examine(**j))
                {
                    std::clog << "log:###ball bounce\t当前系统时间" << currentTime << std::endl;
                    (**i).bounce(**j);
                    sumbounce++;
                }
            }
            for (auto j = walls.cbegin(); j != walls.cend(); j++)
            {
                sumexam++;
                if ((**i).examine(**j))
                {
                    std::cout << "log:###wall bounce\t当前系统时间" << currentTime << std::endl;
                    (**i).bounce(**j);
                    sumbounce++;
                }
            }
        }
        move(DELTATIME);
    }
#endif

#ifdef EVENT_DRIVEN
    while (!(eventQueue.empty()) && (targetTime >= eventQueue.top().time)) //队列非空、事件发生
    {                                                                      //notice:处理事件以后记得刷新小球位置
        /*if (!eventQueue.top().valid())//首先判断是否有效，否则清除并检查下一个
        {
            eventQueue.pop();
            continue;
        }//逻辑：第一次进入必定有效，不检测。循环尾清除后必定有效，不检测。因此这个模块可以删除<delete>*/
        //std::clog << "log:事件队列处理前" << eventQueue;//<debug>
        move(eventQueue.top().t() - currentTime); //有效，跳转到事件发生时间
        Event tempEvent = eventQueue.top();            //提出放置，因为需要检测对应物体
        eventQueue.pop();

        std::cout << "当前系统时间：" << currentTime << std::endl;//<debug>
        tempEvent.handle();                //处理事件，处理时小球自动递增计数器

        { //主小球检测
            for (auto const &i : balls)
                if ((t = tempEvent.ball->predict(*i)) >= 0) //predict内置防重复
                    eventQueue.push(Event(tempEvent.ball, i, t + currentTime));
            if (!walls.empty())
                for (auto const& i : walls)
                    if ((tempEvent.object != i) && ((t = tempEvent.ball->predict(*i)) >= 0)) //智能指针防重复
                        eventQueue.push(Event(tempEvent.ball, i, t + currentTime));
            if (!containers.empty())
                for (auto const& i : containers)
                    if ((t = tempEvent.ball->predict(*i)) >= 0)//容器无需防重复
                        eventQueue.push(Event(tempEvent.ball, i, t + currentTime));
        }

        if (tempEvent.object->type() == Object_type::BALL) //副小球检测
        {
            ball = std::dynamic_pointer_cast<Ball>(tempEvent.object); //智能指针转换
            for (auto const &i : balls)
                if (((t = ball->predict(*i)) >= 0))
                    eventQueue.push(Event(ball, i, t + currentTime));
            if (!walls.empty())
                for (auto const& i : walls)
                    if (((t = ball->predict(*i)) >= 0))
                        eventQueue.push(Event(ball, i, t + currentTime));
            if (!containers.empty())
                for (auto const& i : containers)
                    if (((t = ball->predict(*i)) >= 0))
                        eventQueue.push(Event(ball, i, t + currentTime));
        }
        //std::clog << "log:事件队列处理后" << std::endl << eventQueue;//<debug>
        while((!eventQueue.empty()) && (!eventQueue.top().valid()))//队列非空且队首事件无效
            eventQueue.pop();
        //std::clog << "log:事件队列清理后" << std::endl << eventQueue;//<debug>
    }
    //发生的事件全部处理完成
    move(targetTime - currentTime);
#endif
}

void CollisionSystem::reverse()
{
#ifdef EVENT_DRIVEN
    while (!eventQueue.empty())
        eventQueue.pop(); //清空
#ifdef DEBUG
    OUTPUT << eventQueue;
#endif
#endif

    for (auto &i : balls)
        i->rev();

#ifdef EVENT_DRIVEN
    this->init();
#endif
}

void CollisionSystem::init()
{
    std::clog << "log:system初始化调用" << std::endl;
    for (auto i = balls.cbegin(); i != balls.cend(); i++)
    {
        for (auto j = i + 1; j != balls.cend(); j++)
            if ((**i).examine(**j))
            {
                std::cerr << "error:system初始化检测重叠" << std::endl;
                exit(EXIT_FAILURE);
            }
    }
#ifdef EVENT_DRIVEN
    for (auto i = balls.cbegin(); i != balls.cend(); i++)//遍历初始化
    {
        for (auto j = i + 1; j != balls.cend(); j++)
        {
            if (((temp = (**i).predict(**j)) >= 0))
                eventQueue.push(Event(*i, *j, temp + currentTime));
        }
        if (!walls.empty())
            for (auto const &j : walls)
                if (((temp = (**i).predict(*j)) >= 0))
                    eventQueue.push(Event(*i, j, temp + currentTime));
        if (!containers.empty())
            for (auto const &j : containers)
                if (((temp = (**i).predict(*j)) >= 0))
                    eventQueue.push(Event(*i, j, temp + currentTime));
    }
    std::clog << "log:当前使用事件驱动方式：" << std::endl;
    std::clog << eventQueue;
#endif
    std::clog << "log:system 初始化成功" << std::endl;
}

//#############
//private
//#############
void CollisionSystem::move(float t)
{
    for (auto &i : balls)
        i->move(t);
    currentTime += t;
}

//#############
//operator
//#############
std::istream &operator>>(std::istream &is, CollisionSystem &system)
{
    char identifier;
    int num;
    while (is >> identifier) //iden//如果读到文件结尾eof自动离开主循环
    {

        if (!(is >> num)) //num
        {
            std::cerr << "error:system输入错误" << std::endl;
#ifdef DEBUG
            std::cerr << is.eof() << is.bad() << is.fail() << is.good() << std::endl;
#endif
            exit(EXIT_FAILURE);
        }

        switch (identifier)
        {
        case 'B':
        {
            for (int i = 0; i != num; i++)
                system.balls.push_back(std::make_shared<Ball>(is));
            break;
        }
        case 'W':
        {
            for (int i = 0; i != num; i++)
                system.walls.push_back(std::make_shared<Wall>(is));
            break;
        }
        case 'C':
        {
            for (int i = 0; i != num; i++)
                system.containers.push_back(std::make_shared<Container>(is));
            break;
        }
        }
    }
    std::clog << "log:system输入成功" << std::endl;
    if (!system.balls.empty())
        std::clog << system.balls;
    if (!system.walls.empty())
        std::clog << system.walls;
    if (!system.containers.empty())
        std::clog << system.containers;
    return is;
}

std::ostream &operator<<(std::ostream &os, CollisionSystem &system)
{
    if (!system.balls.empty())
        os << system.balls;
    if (!system.walls.empty())
        os << system.walls;
    if (!system.containers.empty())
        os << system.containers;
#ifdef EVENT_DRIVEN
    if (!system.eventQueue.empty())
        os << system.eventQueue;
#endif
    return os;
}