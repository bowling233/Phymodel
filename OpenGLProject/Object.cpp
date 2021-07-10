#include <iostream>
#include "Object.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <glm\glm.hpp>
#include <memory>

//definition

unsigned int Wall::sum = 0;
unsigned int Ball::sum = 0;
unsigned int Container::sum = 0;

//tools---------------------------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const glm::vec3 &v)
{
    os << std::setprecision(3) << std::fixed;
    os << v.x << '\t'
       << v.y << '\t'
       << v.z << '\t'
       << std::defaultfloat;
    return os;
}

std::istream &operator>>(std::istream &is, glm::vec3 &v)
{
    is >> v.x >> v.y >> v.z;
    return is;
}

float max(float a, float b)
{
    if (a >= b)
        return a;
    else
        return b;
}

//Wall---------------------------------------------------------------------------------------------
Wall::Wall(std::istream &is) : Wall()
{
    is >> *this;
}

std::istream &operator>>(std::istream &is, Wall &wall)
{
    is >> wall.location >> wall.normalVector;
    wall.normalVector = glm::normalize(wall.normalVector); //预先归一化
    return is;
}

std::ostream &operator<<(std::ostream &os, const Wall &wall)
{
    os << std::setprecision(3) << std::fixed;
    os << wall.number << '\t'
       << wall.location << wall.normalVector << std::endl
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::shared_ptr<Wall>> &walls)
{
    os << "Walls-----------------------------------------------------------------" << std::endl;
    os << "Wall\tlocX\tloxY\tlocZ\tnorX\tnorY\tnorZ"
       << std::endl;
    for (auto const &i : walls)
        os << *i << std::endl;
    return os;
}

//Ball---------------------------------------------------------------------------------------------
Ball::Ball(std::istream &is) : Ball()
{
    is >> *this;
}

//################
//predict
//################

float Ball::predict(const Wall &wall) //tochk
{
    //std::clog << "predict:墙体" << this->number << "|" << wall.num() << std::endl;//<debug>
    glm::vec3 p = location - wall.loc();
    float p_n = glm::dot(p, wall.norm());        //球到平面的垂直距离
    float v_n = glm::dot(velocity, wall.norm()); //速度在平面法向量方向上的分量

    if (v_n * p_n >= 0)
        return -1.0f;
    return -p_n / v_n;
    //another solution
    /*
    glm::vec3 r = location - wall.loc();
    glm::vec3 chk_nor = (glm::dot(r, wall.norm())) > 0 ? wall.norm() : -wall.norm(); //选择一个背向球的法向量
    float v_l = glm::dot(velocity, chk_nor);//速度在平面法向量方向上的分量
    if (v_1 < 0)
        return -1.0f;                 //背向运动
    float r_l = glm::dot(r, chk_nor); //球到平面的垂直距离
    return r_l / v_l;
    /*/
}

float Ball::predict(const Ball &ball) //tochk
{
    //std::clog << "predict:球体" << this->num() << "|" << ball.number <<std::endl;//<debug>

    if (number == ball.number) //防止自预测
        return -1.0f;

    glm::vec3 dp = location - ball.location, //以ball为中心
        dv = velocity - ball.velocity;

    if (this->back(ball))
        return -1.0;

    //std::cout << "predict:计算信息 dv dp r1 r2\n" << dv << dp << this->radius << '\t' << ball.radius << std::endl;//<debug>

    float a = square(glm::length(dv));
    float b = 2.0 * glm::dot(dv, dp);
    float c = square(glm::length(dp)) - square(radius + ball.radius);
    float delta = square(b) - 4.0 * a * c;

    if (delta < 0.0)
        return -1.0;

    float x1 = ((-b + std::sqrt(delta)) / (2.0 * a));
    float x2 = ((-b - std::sqrt(delta)) / (2.0 * a));

    if (x2 < 0 || x1 < 0)//事实上，这里小球已经相交
        return -1.0;
    //question:x2是较小的根吗？是的，因为sqrt(delta)一定是个正数，所以减去该项的一定更小。我们应该传回最小实根

    //question:小根大于0必定返回小根吗？是这样的，我们要求离当前时刻最近的一次碰撞解，因此返回最小正根
    /*
    if (x2 > 0)
        return x2;
    if (x1 > 0)
        return x1;
        */
    //std::cout << "predict:球体预测通过\t" << "x1:" << x1 << "x2\t" << x2 << std::endl;//<debug>
    return x2;//返回小根
}

float Ball::predict(const Container &container) //tochk
{
    //std::clog << "predict:容器" << this->number << "|" << container.num() <<std::endl;//<debug>
    float x, y, z;

    if (velocity.x = 0)
        x = -1.0;
    else if (velocity.x > 0)
        x = (container.x_p() - location.x) / velocity.x;
    else if (velocity.x < 0)
        x = (location.x - container.x_n()) / velocity.x;

    if (velocity.y = 0)
        y = -1.0;
    else if (velocity.y > 0)
        y = (container.y_p() - location.y) / velocity.y;
    else if (velocity.y < 0)
        y = (location.y - container.y_n()) / velocity.y;

    if (velocity.z = 0)
        z = -1.0;
    else if (velocity.z > 0)
        z = (container.z_p() - location.z) / velocity.z;
    else if (velocity.z < 0)
        z = (location.z - container.z_n()) / velocity.z;

    if (x < 0 && y < 0 && z < 0)
        return -1.0;
    else
        return max(max(x, y), z); //返回最大值
}

//################
//bounce
//################

void Ball::bounce(Object &object)
{
    //为了兼容time驱动，在传递后递增计数器
    switch (object.type())
    {
    case Object_type::BALL:
    {
        this->bounce(dynamic_cast<Ball &>(object));
        break;
    }
    case Object_type::WALL:
    {
        this->bounce(dynamic_cast<Wall &>(object));
        break;
    }
    case Object_type::CONTAINER:
    {
        this->bounce(dynamic_cast<Container &>(object));
        break;
    }
    }
}

void Ball::bounce(Wall &wall) //tochk
{
    std::clog << "info:碰撞wall处理" << std::endl << "主小球：\t" << *this << std::endl;
    glm::vec3 ckd_nor = glm::dot((location - wall.loc()), wall.norm()) > 0 ? wall.norm() : -wall.norm();
    velocity += -2 * (dot(velocity, ckd_nor)) * ckd_nor; //速度反两倍
    count++;
}

void Ball::bounce(Ball &ball)
{
    std::clog << "info:碰撞处理前\n主小球：\t" << *this << "\n副小球：\t" << ball << std::endl;
    //if (this->back(ball))
      //  return;

    glm::vec3 r = glm::normalize(location - ball.location);

    float v10 = glm::dot(r, velocity),
          v20 = glm::dot(r, ball.velocity),
          m1 = mass,
          m2 = ball.mass;

    float v1 = ((m1 - m2) * v10 + 2 * m2 * v20) / (m1 + m2),
          v2 = ((m2 - m1) * v20 + 2 * m1 * v10) / (m1 + m2);

    velocity += (v1 - v10) * r;
    ball.velocity += (v2 - v20) * r;
    count++;
    ball.count++;
    std::clog << "info:碰撞处理后\n主小球：\t" << *this << "\n副小球：\t" << ball << std::endl;
}

void Ball::bounce(Container &container) //tochk
{
    
    float x, y, z;

    if (velocity.x = 0)
        x = -1.0;
    else if (velocity.x > 0)
        x = (container.x_p() - location.x) / velocity.x;
    else if (velocity.x < 0)
        x = (location.x - container.x_n()) / velocity.x;

    if (velocity.y = 0)
        y = -1.0;
    else if (velocity.y > 0)
        y = (container.y_p() - location.y) / velocity.y;
    else if (velocity.y < 0)
        y = (location.y - container.y_n()) / velocity.y;

    if (velocity.z = 0)
        z = -1.0;
    else if (velocity.z > 0)
        z = (container.z_p() - location.z) / velocity.z;
    else if (velocity.z < 0)
        z = (location.z - container.z_n()) / velocity.z;

    if (x < y && x < z)
        velocity.x = -velocity.x;
    else if (y < x && y < z)
        velocity.y = -velocity.y;
    else if (z < x && z < y)
        velocity.z = -velocity.z;
    else
        {
            std::cerr << "error:容器碰撞错误" << std::endl;
            exit(EXIT_SUCCESS);
        }
    count++;
}

//################
//examine
//################


bool Ball::examine(const Ball &ball)
{
    return (glm::length(location - ball.location) < (radius + ball.radius));
}

bool Ball::examine(const Wall &wall)
{
    return (abs(glm::dot(location - wall.loc(), wall.norm())) < radius);
}

bool Ball::examine(const Container &container) //tochk
{
    return ((location.x + radius < container.x_p()) && (location.x - radius > container.x_n())) && ((location.y + radius < container.y_p()) && (location.y - radius > container.y_n())) && ((location.z + radius < container.z_p()) && (location.z - radius > container.z_n()));
}

bool Ball::back(const Ball &ball)
{
    if ((glm::dot(-(location - ball.location), velocity) < 0) && (glm::dot(location - ball.location, ball.velocity) < 0))
    {
#ifdef DEBUG
        std::cout << "error:背向运动" << std::endl;
#endif
        return true;
    }
    return false;
}

//io------------------------------------------------------------------------------------------------------------
std::istream &operator>>(std::istream &is, Ball &ball)
{
    is >> ball.location >> ball.velocity >> ball.mass >> ball.radius;
    return is;
}

std::ostream &operator<<(std::ostream &os, const Ball &ball)
{
    os << std::setprecision(3) << std::fixed;
    os << ball.number << '\t'
       << ball.location << ball.velocity
       << ball.mass << '\t'
       << ball.radius << '\t'
       << ball.count << '\t'
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::shared_ptr<Ball>> &balls)
{
    os << "Balls---------------------------------------------------------------------------" << std::endl;
    os << "Ball\tlocX\tloxY\tlocZ\tvelX\tvelY\tvelZ\tMass\tRadius\tCount"
       << std::endl;
    for (auto const &i : balls)
        os << *i << std::endl;
    return os;
}

//container

Container::Container(std::istream &is)
{
    is >> *this;
}

//io
std::istream &operator>>(std::istream &is, Container &container)
{
    is >> container.location >> container.length;
    return is;
}

std::ostream &operator<<(std::ostream &os, const Container &container)
{
    os << std::setprecision(3) << std::fixed
    << container.number << '\t'
    << container.location << container.length
    << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::shared_ptr<Container>> &containers)
{
    os << "Containers-----------------------------------------------------------------" << std::endl;
    os << "Cont\tlocX\tloxY\tlocZ\tlenX\tlenY\tlenZ"
        << std::endl;
    for (auto const& i : containers)
        os << *i << std::endl;
    return os;
    return os;
}
