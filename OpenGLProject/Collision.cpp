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
    case Object_type::CONTAINER:
        os << "cont";
        break;
    }
    os << '\t' << event.object->num()
       << '\t' << event.countObject;
    return os;
}
/*
std::ostream &operator<<(std::ostream &os, std::priority_queue<Event, std::vector<Event>> eventqueue) //time consuming
{
    os << "---------------Events-------------------------" << std::endl;
    os << "time\tBall\tcnt\tObj_typ\tObj_num\tObj_cnt"
       << std::endl;
    while (!eventqueue->empty())
    {
        os << eventqueue->top() << std::endl;
        eventqueue->pop();
    }
    return os;
}
*/
#endif

//Event_mgr
void Event_mgr_indexed::push(const Event &event)
{
   // std::cout << "push:----------------------------------------------------------------------------\n"
    //          << event << std::endl;
    const int k = event.ball->num(); //第k个球
    if (contains(k))                 //包含
    {
        //std::cout << "contained" << std::endl;
        if (events[k].valid() && (event > events[k])) //k球事件有效且比k球的事件大
            return;
        //std::cout << "can insert" << std::endl; //k球事件无效或k球事件小，可以插入
        //std::cout << *this;
        events[k] = event; //放入事件
        swim(qp[k]);       //让k球事件上浮，通过qp快速找到pq
        if (qp[k] == -1)
            exit(EXIT_FAILURE);
        sink(qp[k]);
    }
    else //插入
    {
        //std::cout << "not contained:before" << std::endl;
        //std::cout << *this;
        N++;
        qp[k] = N; //tobedone
        pq[N] = k;
        events[k] = event;
        swim(N);
    }
    //std::cout << *this;
}

void Event_mgr_indexed::pop()
{
    //std::cout << "action:pop" << std::endl;
    exch(1, N--); //先换到最后一位，扔掉
    sink(1);      //最上位下沉
    //events[pq(N + 1)] = null;//?
    qp[pq[N + 1]] = -1; //标记为不存在
}

void Event_mgr_indexed::swim(int k) //对pq中的k进行上浮
{
    while (k > 1 && more(k / 2, k)) //k不是根节点，k的父节点比k大
    {
        //std::cout << "action:swim" << std::endl;
        exch(k / 2, k); //交换父节点
        k = k / 2;      //向下搜索
    }
}

void Event_mgr_indexed::sink(int k) //对k下沉，k子节点比较小的换上来
{
    while (2 * k <= N) //k的子节点在堆内部
    {
        //std::cout << "action:sink" << std::endl;
        int j = 2 * k;               //去k的子节点
        if (j < N && more(j, j + 1)) //子节点不是孤节点且右节点小
            j++;                     //切换到右节点
        if (less(k, j))              //k比子节点小
            break;                   //走人
        exch(k, j);                  //下沉至子节点
        k = j;
    }
}

void Event_mgr_indexed::exch(int i, int j)
{
    //std::cout << "action:exch: << std::endl";
    /*std::cout
        << "i,j,qp,pq" << std::endl
        << i << '\t' << qp[i] << '\t' << pq[i] << std::endl
        << j << '\t' << qp[j] << '\t' << pq[j] << std::endl;*/
    qp[0] = qp[pq[j]];
    qp[pq[j]] = qp[pq[i]];
    qp[pq[i]] = qp[0];
    pq[0] = pq[i];
    pq[i] = pq[j];
    pq[j] = pq[0];
}

std::ostream &operator<<(std::ostream &os, Event_mgr_indexed &eventQueue)
{
    os << "---------------Events-------------------------" << std::endl;
    os << "time\tBall\tcnt\tObj_typ\tObj_num\tObj_cnt"
       << std::endl;
    for (int i = 1; i <= eventQueue.size(); i++)
    {
        os << eventQueue.events[eventQueue.pq[i]] << std::endl;
    }

    os << "num\tpq\tqp" << std::endl;
    for (int i = 1; i <= eventQueue.size(); i++)
    {
        os << i << '\t' << eventQueue.pq[i] << '\t' << eventQueue.qp[i] << std::endl;
    }
    return os;
}

//#############
//CollisionSystem
//#############

//int flag = 1;

void CollisionSystem::run(const double t)
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
                    sumbounce++;
                }
            }
            if (!containers->empty())
                for (auto j = containers.cbegin(); j != containers.cend(); j++)
                {
                    //sumexam++;
                    if ((**i).examine(**j))
                    {
                        (**i).bounce(**j);
                        sumbounce++;
                    }
                }
        }
        move(DELTATIME);
    }
#endif

#ifdef EVENT_DRIVEN
    while (!(eventQueue.empty()) && (targetTime >= eventQueue.top().t())) //队列非空、事件发生
    {                                                                     //notice:处理事件以后记得刷新小球位置
        move(eventQueue.top().t() - currentTime);                         //有效，跳转到事件发生时间
        tempEvent = eventQueue.top();                                     //提出放置，因为需要检测对应物体
        eventQueue.top().handle();
        eventQueue.pop();
        //sumbounce++;        //<info>

        { //主小球检测note:只要碰撞处理得好就不应该出现再次预测的情况
            for (auto const &i : balls)
                if (((temp = tempEvent.ball->predict(*i)) >= 0)) //predict内置防重复
                    eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));
            if (!walls.empty())
                for (auto const &i : walls)
                    if (((temp = tempEvent.ball->predict(*i)) >= 0)) //容器无需防重复
                        eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));
            if (!containers.empty())
                for (auto const &i : containers)
                    if (((temp = tempEvent.ball->predict(*i)) >= 0)) //容器无需防重复
                        eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));
        }

        if (tempEvent.object->type() == Object_type::BALL) //副小球检测
        {
            tempball = std::dynamic_pointer_cast<Ball>(tempEvent.object); //智能指针转换
            for (auto const &i : balls)
                if (((temp = tempball->predict(*i)) >= 0))
                    eventQueue.push(Event(tempball, i, temp + currentTime));
            if (!walls.empty())
                for (auto const &i : walls)
                    if (((temp = tempball->predict(*i)) >= 0))
                        eventQueue.push(Event(tempball, i, temp + currentTime));
            if (!containers.empty())
                for (auto const &i : containers)
                    if (((temp = tempball->predict(*i)) >= 0))
                        eventQueue.push(Event(tempball, i, temp + currentTime));
        }
        //此处需要注意，使用索引优先队列后，每次弹出empty的事件时都必须检测
        while ((!eventQueue.empty()) && (!eventQueue.top().valid())) //循环不变式：队列非空且队首事件无效
        {                                                            //特别检查
            tempEvent = eventQueue.top();                            //提出放置，因为需要检测对应物体
            //eventQueue.top().handle();不处理，其他都相同
            eventQueue.pop();
            for (auto const &i : balls)
                if (((temp = tempEvent.ball->predict(*i)) >= 0)) //predict内置防重复
                    eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));
            if (!walls.empty())
                for (auto const &i : walls)
                    if (((temp = tempball->predict(*i)) >= 0))
                        eventQueue.push(Event(tempball, i, temp + currentTime));
            if (!containers.empty())
                for (auto const &i : containers)
                    if (((temp = tempEvent.ball->predict(*i)) >= 0)) //容器无需防重复
                        eventQueue.push(Event(tempEvent.ball, i, temp + currentTime));
        }
    }
    //发生的事件全部处理完成
    move(targetTime - currentTime);
#endif
    //std::cout << eventQueue.size() << std::endl; //<debug>
}

void CollisionSystem::reverse()
{
    for (auto &i : balls)
        i->reverse();

#ifdef EVENT_DRIVEN
    eventQueue.init();
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
        if (!walls.empty())
            for (auto const &j : walls)
                if (((temp = (**i).predict(*j)) >= 0))
                    eventQueue.push(Event(*i, j, temp + currentTime));
        if (!containers.empty())
            for (auto const &j : containers)
                if (((temp = (**i).predict(*j)) >= 0))
                    eventQueue.push(Event(*i, j, temp + currentTime));
    }

    std::cout << "log:当前使用事件驱动方式:" << eventQueue.size() << std::endl;
    //std::cout << eventQueue << std::endl;
#endif
    std::cout << "log:system 初始化成功" << std::endl;
}

double CollisionSystem::ek()
{
    temp = 0.0f;
    for (auto const i : balls)
        temp += i->ek();
    return temp;
}

//#############
//private
//#############
void CollisionSystem::move(double t)
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
    if (!is)
    {
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
        case 'B':
        {
            for (int i = 0; i != num; i++)
                system.balls.push_back(std::make_shared<Ball>(is));
            break;
        }
        case 'C':
        {
            for (int i = 0; i != num; i++)
                system.containers.push_back(std::make_shared<Container>(is));
            break;
        }
        case 'W':
        {
            for (int i = 0; i != num; i++)
                system.walls.push_back(std::make_shared<Wall>(is));
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
    if (!system.walls.empty())
        os << system.walls;
    if (!system.containers.empty())
        os << system.containers;
#ifdef EVENT_DRIVEN
        //if (!system.eventQueue->empty())
        //os << system.eventQueue;
#endif
    return os;
}