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
    //std::cout << "info:事件处理信息：" << std::endl << *this << std::endl;//<debug>
    if ((ball->cnt() == countBall) && (object->cnt() == countObject))
        ball->bounce(*object); //从指针变成对象的引用，由ball类进行运行时绑定
}

bool Event::valid() const
{
    //std::cout << "info:事件有效性检测\n" << *this << '\t' << ball->cnt() << '\t' << object->cnt() << std::endl;//debug
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

//int flag = 1;

void CollisionSystem::run(const float t)
{
    targetTime = currentTime + t;
#ifdef TIME_DRIVEN
    while (currentTime < targetTime)
    {
        for (auto i = balls.cbegin(); i != balls.cend(); i++)
        {
            for (auto j = i + 1; j != balls.cend(); j++)
            {
                //sumexam++;
                if ((**i).examine(**j))
                {
                    //std::cout << "log:###ball bounce\t当前系统时间" << currentTime <<"\ndetail:\t" << (**i).num() << '\t' << (**j).num() << std::endl;//<debug>
                    (**i).bounce(**j);
                    //sumbounce++;
                }
            }
            if (!walls.empty())
                for (auto j = walls.cbegin(); j != walls.cend(); j++)
                {
                    //sumexam++;
                    if ((**i).examine(**j))
                    {
                        (**i).bounce(**j);
                        //sumbounce++;
                    }
                }
            if(!containers.empty())
                for (auto j = containers.cbegin(); j != containers.cend(); j++)
                {
                    //sumexam++;
                    if ((**i).examine(**j))
                    {
                        (**i).bounce(**j);
                        //sumbounce++;
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
        //std::cout << "log:事件队列处理前" << eventQueue;//<debug>
        move(eventQueue.top().t() - currentTime); //有效，跳转到事件发生时间
        tempEvent = eventQueue.top();       //提出放置，因为需要检测对应物体
        eventQueue.pop();
        //sumbounce++; //<info>
        //std::cout << "当前系统时间：" << currentTime << std::endl;//<debug>
        tempEvent.handle(); //处理事件，处理时小球自动递增计数器

        { //主小球检测note:只要碰撞处理得好就不应该出现再次预测的情况
            for (auto const &i : balls)
                if ((temp = tempEvent.ball->predict(*i)) >= 0) //predict内置防重复
                    eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));
            if (!hidden_balls.empty())
                for (auto const &i : hidden_balls)
                    if ((temp = tempEvent.ball->predict(*i)) >= 0) //predict内置防重复
                        eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));/*
            if (!walls.empty())
                for (auto const &i : walls)
                    if ((temp = tempEvent.ball->predict(*i)) >= 0) //平面无需防重复
                        eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));/**/
            if (!containers.empty())
                for (auto const &i : containers)
                    if ((temp = tempEvent.ball->predict(*i)) >= 0) //容器无需防重复
                        eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));
        }

        if (tempEvent.object->type() == Object_type::BALL) //副小球检测
        {
            tempball = std::dynamic_pointer_cast<Ball>(tempEvent.object); //智能指针转换
            for (auto const &i : balls)
                if ((temp = tempball->predict(*i)) >= 0)
                    eventQueue.push(Event(tempball, i, temp + currentTime));
            if (!hidden_balls.empty())
                for (auto const &i : hidden_balls)
                    if ((temp = tempball->predict(*i)) >= 0) //predict内置防重复
                        eventQueue.push(Event(tempball, i, temp + currentTime));/*/
            if (!walls.empty())
                for (auto const &i : walls)
                    if ((temp = tempball->predict(*i)) >= 0)
                        eventQueue.push(Event(tempball, i, temp + currentTime));//*/
            if (!containers.empty())
                for (auto const &i : containers)
                    if ((temp = tempball->predict(*i)) >= 0)
                        eventQueue.push(Event(tempball, i, temp + currentTime));
        }
        //std::cout << "log:事件队列处理后" << std::endl << eventQueue;//<debug>
        while ((!eventQueue.empty()) && (!eventQueue.top().valid())) //队列非空且队首事件无效
            eventQueue.pop();
        //std::cout << "log:事件队列清理后" << std::endl << eventQueue;//<debug>
    }
    //发生的事件全部处理完成
    move(targetTime - currentTime);
    /*if (flag == 0)
    {
        this->reverse();
        flag = -1;
    }*/
#endif
    //std::cout << "当前系统时间：" << currentTime << std::endl;//<debug>
}

void CollisionSystem::reverse()
{
    for (auto &i : balls)
        i->reverse();

#ifdef EVENT_DRIVEN
    while (!eventQueue.empty())
        eventQueue.pop(); //清空
    this->init();
#endif
}

void CollisionSystem::init()
{
    std::cout << "log:system初始化调用" << std::endl; //<debug>
    /*for (auto i = balls.cbegin(); i != balls.cend(); i++)
    {
        for (auto j = i + 1; j != balls.cend(); j++)
            if ((**i).examine(**j))
            {
                std::cout << "error:system初始化检测重叠" << std::endl;
                exit(EXIT_FAILURE);
            }
    }*/
#ifdef EVENT_DRIVEN
    for (auto i = balls.cbegin(); i != balls.cend(); i++) //遍历初始化
    {
        for (auto j = i + 1; j != balls.cend(); j++)
        {
            if (((temp = (**i).predict(**j)) >= 0))
                eventQueue.push(Event(*i, *j, temp + currentTime));
        }
        if (!hidden_balls.empty())
            for (auto const& j : hidden_balls)
                if ((temp = (**i).predict(*j)) >= 0)
                    eventQueue.push(Event(*i, j, temp + currentTime));
        if (!walls.empty())
            for (auto const& j : walls)
                if ((temp = (**i).predict(*j)) >= 0)
                    eventQueue.push(Event(*i, j, temp + currentTime));
        if (!containers.empty())
            for (auto const& j : containers)
                if ((temp = (**i).predict(*j)) >= 0)
                    eventQueue.push(Event(*i, j, temp + currentTime));
    }
    if(!hidden_balls.empty())
        for (auto i = hidden_balls.cbegin(); i != hidden_balls.cend(); i++)
        {
            for (auto j = i + 1; j != hidden_balls.cend(); j++)
            {
                if (((temp = (**i).predict(**j)) >= 0))
                    eventQueue.push(Event(*i, *j, temp + currentTime));
            }
            if (!walls.empty())
                for (auto const& j : walls)
                    if ((temp = (**i).predict(*j)) >= 0)
                        eventQueue.push(Event(*i, j, temp + currentTime));
            if (!containers.empty())
                for (auto const& j : containers)
                    if ((temp = (**i).predict(*j)) >= 0)
                        eventQueue.push(Event(*i, j, temp + currentTime));
        }
    std::cout << "log:当前使用事件驱动方式:" << eventQueue.size() << std::endl;
    //std::cout << eventQueue << std::endl;
#endif
    std::cout << "log:system 初始化成功" << std::endl;
}

float CollisionSystem::ek()
{
    temp = 0.0f;
    for (auto const i : balls)
        temp += i->ek();
    if (!hidden_balls.empty())
        for (auto const i : hidden_balls)
            temp += i->ek();
    return temp;
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
    if (!is) {
        std::cout << "error:is输入错误" << std::endl;
        std::cout << is.eof() << is.bad() << is.fail() << is.good() << std::endl;
        exit(EXIT_FAILURE);
    }
    char identifier;
    int num;
    while (is >> identifier) //iden//如果读到文件结尾eof自动离开主循环
    {

        if (!(is >> num)) //num
        {
            std::cout << "error:system输入错误" << std::endl;
            //std::cout << is.eof() << is.bad() << is.fail() << is.good() << std::endl;//<debug>
            exit(EXIT_FAILURE);
        }

        switch (identifier)
        {
        case 'H':
        {
            for (int i = 0; i != num; i++)
                system.hidden_balls.push_back(std::make_shared<Ball>(is));
            break;
        }
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
    std::cout << "log:system输入成功" << std::endl;
    return is;
}

std::ostream &operator<<(std::ostream &os, CollisionSystem &system)
{
    if (!system.balls.empty())
        os << system.balls;
    if (!system.hidden_balls.empty())
        os << system.hidden_balls;
    if (!system.walls.empty())
        os << system.walls;
    if (!system.containers.empty())
        os << system.containers;
#ifdef EVENT_DRIVEN
        //if (!system.eventQueue.empty())
        //os << system.eventQueue;
#endif
    return os;
}