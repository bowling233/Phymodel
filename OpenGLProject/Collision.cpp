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
    if ((ball->cnt() == countBall) && (object->cnt() == countObject))
        ball->bounce(*object); //从指针变成直接对对象的引用，从这里控制权交还给ball类
    OUTPUT << "::::::::::::::::::::::::::::::Event handle: " << *this << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Event &event)
{
    os << std::setprecision(3) << std::fixed;
    os << std::setw(8) << event.time
       << " | " << std::setw(4) << event.ball->num()
       << " | " << std::setw(3) << event.countBall
       << " | " << std::setw(10);
    switch (event.object->type())
    {
    case Object_type::FIXEDBALL:
        os << "fixedball";
        break;
    case Object_type::BALL:
        os << "ball";
        break;
    case Object_type::WALL:
        os << "wall";
        break;
    }
    os << " | " << std::setw(3) << event.object->num()
       << " | " << std::setw(3) << event.countObject
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::priority_queue<Event, std::vector<Event>> eventqueue) //time consuming
{
    os << "---------------Events-------------------------" << std::endl;
    os << "   time  | Ball | cnt | Obj_type  | Obj | cnt"
       //"    1.000 |    1 |   3 | fixedball |   1 |   3"
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
            for (auto j = i + 1; j != balls.cend(); j++)
            {
                sumexam++;
                if ((**i).examine(**j))
                {
                    (**i).bounce(**j);
                    sumbounce++;
                }
            }
        move(DELTATIME);
    }
#endif

#ifdef EVENT_DRIVEN
    while (!(eventQueue.empty()) && (targetTime >= eventQueue.top().t())) //事件发生//notice:处理事件以后记得刷新小球位置//确保队列非空
    {
        move(eventQueue.top().t() - currentTime); //跳转到事件发生时间
        Event temp = eventQueue.top();
        eventQueue.top().handle(); //处理事件
        eventQueue.pop();

        //std::cout << currentTime << std::endl;

        { //主小球检测
            std::shared_ptr<Ball> ball = temp.b();
            Object_type type = temp.obj()->type();
            unsigned int num = temp.obj()->num();

            for (auto const &i : balls)
                if ((type == Object_type::BALL) && (i->num() == num))
                    continue; //防止另外一个对象的重复
                else if ((t = (ball->predict(*i)) > 0))
                    eventQueue.push(Event(ball, i, t + currentTime));

            for (auto const &i : fixedBalls)
                if ((type == Object_type::FIXEDBALL) && (i->num() == num))
                    continue; //防止另外一个对象的重复
                else if ((t = (ball->predict(*i)) > 0))
                    eventQueue.push(Event(ball, i, t + currentTime));
            for (auto const &i : walls)
                if ((type == Object_type::WALL) && (i->num() == num))
                    continue; //防止另外一个对象的重复
                else if ((t = (ball->predict(*i)) > 0))
                    eventQueue.push(Event(ball, i, t + currentTime));
        }

        if (temp.obj()->type() == Object_type::BALL) //副小球的检测,无需防重复
        {
            std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(temp.obj());
            unsigned int num = temp.b()->num();
            for (auto const &i : balls)
                if (!(i->num() == num))
                    if ((t = (ball->predict(*i)) > 0))
                        eventQueue.push(Event(ball, i, t + currentTime));

            for (auto const &i : fixedBalls)
                if ((t = (ball->predict(*i)) > 0))
                    eventQueue.push(Event(ball, i, t + currentTime));

            for (auto const &i : walls)
                if ((t = (ball->predict(*i)) > 0))
                    eventQueue.push(Event(ball, i, t + currentTime));
        }

        while ((!eventQueue.empty()) && (!temp.status()))
            eventQueue.pop(); //清理后续无效事件，方便下一次操作

        std::cout << eventQueue;
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
}




//#############
//private
//#############
void CollisionSystem::init()
{
    for (auto i = balls.cbegin(); i != balls.cend(); i++)
    {
        for (auto j = i + 1; j != balls.cend(); j++)
            if ((**i).examine(**j))
            {
                std::cerr << "init error:crossed" << std::endl;
                exit(EXIT_FAILURE);
            }
    }
}

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
            std::cerr << "input err" << std::endl;
#ifdef DEBUG
            std::cerr << is.eof() << is.bad() << is.fail() << is.good() << std::endl;
#endif
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i != num; i++)
        {
            switch (identifier)
            {
            case 'B':
            {
                system.balls.push_back(std::make_shared<Ball>(is));
                break;
            }
            case 'W':
            {
                system.walls.push_back(std::make_shared<Wall>(is));
                break;
            }
            case 'F':
            {
                system.fixedBalls.push_back(std::make_shared<FixedBall>(is));
                break;
            }
            }
        }
    }
    std::clog << "System read in over::::::::::::::::" << std::endl;
    
    //system.init();//notice
    return is;
}

std::ostream &operator<<(std::ostream &os, CollisionSystem &system)
{
    if (!system.balls.empty())
        os << system.balls;
    if (!system.fixedBalls.empty())
        os << system.fixedBalls;
    if (!system.walls.empty())
        os << system.walls;
#ifdef EVENT_DRIVEN
    if(!system.eventQueue.empty())
    os << system.eventQueue;
#endif
    return os;
}