#include <iostream>
#include "Object.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <glm\glm.hpp>
#include <memory>


//definition
unsigned int FixedBall::sum = 0;
unsigned int Wall::sum = 0;
unsigned int Ball::sum = 0;

//tools---------------------------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const glm::vec3 &v)
{
    os << std::setprecision(3) << std::fixed;
    os << std::setw(8) << v.x << " |"
       << std::setw(8) << v.y << " |"
       << std::setw(8) << v.z << " |"
       << std::defaultfloat;
    return os;
}

std::istream &operator>>(std::istream &is, glm::vec3 &v)
{
    is >> v.x >> v.y >> v.z;
    return is;
}

//fixedBall ---------------------------------------------------------------------------------------------
FixedBall::FixedBall(std::istream &is) : FixedBall()
{
    is >> *this;
    number = ++sum;
}

std::istream &operator>>(std::istream &is, FixedBall &fixedBall)
{
    is >> fixedBall.location >> fixedBall.radius;
    return is;
}

std::ostream &operator<<(std::ostream &os, const FixedBall &fixedBall)
{
    os << std::setprecision(3) << std::fixed;
    os << std::setw(8) << fixedBall.number << " |"
       << fixedBall.location
       << std::setw(8) << fixedBall.radius << std::endl
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::shared_ptr<FixedBall>> &fixedBalls)
{
    os << "-----------FixedBalls------------------------------" << std::endl;
    os << "   Ball  | locX    | loxY    | locZ    | Radius  |"
       // "       1 | 1111.00 | 1111.00 | 1111.00 | 1111.00 |"
       << std::endl;
    for (auto const &i : fixedBalls)
        os << *i << std::endl;
    return os;
}

//Wall---------------------------------------------------------------------------------------------
Wall::Wall(std::istream &is) : Wall()
{
    is >> *this;
    number = ++sum;
}

std::istream &operator>>(std::istream &is, Wall &wall)
{
    is >> wall.location >> wall.normalVector;
    return is;
}

std::ostream &operator<<(std::ostream &os, const Wall &wall)
{
    os << std::setprecision(3) << std::fixed;
    os << std::setw(8) << wall.number << " |"
       << wall.location << wall.normalVector << std::endl
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::shared_ptr<Wall>> &walls)
{
    os << "-----------Walls------------------------------------------------------" << std::endl;
    os << "   Wall  | locX    | loxY    | locZ    | norX    | norY    | norZ    |"
       // "       1 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 |"
       << std::endl;
    for (auto const &i : walls)
        os << *i << std::endl;
    return os;
}

//Ball---------------------------------------------------------------------------------------------
Ball::Ball(std::istream &is) : Ball()
{
    is >> *this;
    number = ++sum;
}

//predict------------------------------------------------------------------------------------------------------------
/*no need
float Ball::predict(Object &object)
{
    switch (object.type())
    {
    case Object_type::FIXEDBALL:

    {
        FixedBall &fixedBall = dynamic_cast<FixedBall &>(object);
        return this->predict(fixedBall);
    }
    case Object_type::BALL:
    {
        Ball &ball = dynamic_cast<Ball &>(object);
        return this->predict(ball);
    }
    case Object_type::WALL:
    {
        Wall &wall = dynamic_cast<Wall &>(object);
        return this->predict(wall);
    }
    }
}
*/

float Ball::predict(const FixedBall &fixedBall) //tochk
{
    glm::vec3 dLoc = location - fixedBall.loc();
    float a = square(glm::length(velocity)),
          b = 2 * glm::dot(velocity, dLoc),
          c = square(glm::length(dLoc)) - square(radius + fixedBall.r());

    float delta = square(b) - 4.0 * a * c;
    if (delta < 0.0)
        return -1.0;

    float x1 = ((-b + std::sqrt(delta)) / (2.0 * a));
    float x2 = ((-b - std::sqrt(delta)) / (2.0 * a));

    if (x2 > 0)
        return x2;
    if (x1 > 0)
        return x1;
    return -1.0;
}

float Ball::predict(const Wall &wall) //tochk
{
    glm::vec3 r = location - wall.loc();
    float r_l = glm::dot(r, wall.norm());        //球到平面的垂直距离
    float v_l = glm::dot(velocity, wall.norm()); //速度在平面法向量方向上的分量

    if (v_l * r_l >= 0)
        return -1.0f;
    return -r_l / v_l;
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
    if (number == ball.number) //防止自预测
        return -1.0f;
    glm::vec3 r = location - ball.location, //以ball为中心
        dv = velocity - ball.velocity;

    if (this->back(ball))
        return -1.0;
    if (this->examine(ball))
        return 0.0f;

    float a = square(glm::length(dv));
    float b = 2.0 * glm::dot(dv, r);
    float c = square(glm::length(r)) - square(radius + ball.radius);
    float delta = square(b) - 4.0 * a * c;

    if (delta < 0.0)
        return -1.0;

    float x1 = ((-b + std::sqrt(delta)) / (2.0 * a));
    float x2 = ((-b - std::sqrt(delta)) / (2.0 * a));
    //question:x2是较小的根吗？是的，因为sqrt(delta)一定是个正数，所以减去该项的一定更小。我们应该传回最小实根

    //question:小根大于0必定返回小根吗？是这样的，我们要求离当前时刻最近的一次碰撞解，因此返回最小正根

    if (x2 > 0)
        return x2;
    if (x1 > 0)
        return x1;
    return -1.0;
}

//bounce------------------------------------------------------------------------------------------------------------
void Ball::bounce(Object &object)
{
    count++;
    switch (object.type())
    {
    case Object_type::FIXEDBALL:

    {
        FixedBall &fixedBall = dynamic_cast<FixedBall &>(object);
        this->bounce(fixedBall);
        break;
    }
    case Object_type::BALL:
    {
        Ball &ball = dynamic_cast<Ball &>(object);
        this->bounce(ball);
        break;
    }
    case Object_type::WALL:
    {
        Wall &wall = dynamic_cast<Wall &>(object);
        this->bounce(wall);
        break;
    }
    }
}

void Ball::bounce(const FixedBall &fixedBall) //tochk
{
    glm::vec3 r = location - fixedBall.loc();
    velocity += -2 * glm::dot(velocity, r) * r; //速度反两倍
}

void Ball::bounce(const Wall &wall) //tochk
{
    glm::vec3 ckd_nor = glm::dot((location - wall.loc()), wall.norm()) > 0 ? wall.norm() : -wall.norm();
    velocity += -2 * (dot(velocity, ckd_nor)) * ckd_nor; //速度反两倍
}

void Ball::bounce(Ball &ball)
{
    if (this->back(ball))return;

    glm::vec3 r = glm::normalize(location - ball.location);

    float v10 = glm::dot(r, velocity),
          v20 = glm::dot(r, ball.velocity),
          m1 = mass,
          m2 = ball.mass;

    float v1 = ((m1 - m2) * v10 + 2 * m2 * v20) / (m1 + m2),
          v2 = ((m2 - m1) * v20 + 2 * m1 * v10) / (m1 + m2);

    velocity += (v1 - v10) * r;
    ball.velocity += (v2 - v20) * r;
    ball.count++;
}


bool Ball::examine(const Ball &ball)
{
    if (glm::length(location - ball.location) < (radius + ball.radius)*1.0001f)
    {
        std::cout << "crossed" << std::endl;
        return true;
    }
    return false;
}

bool Ball::back(const Ball& ball)
{
    if ((glm::dot(-(location - ball.location), velocity) < 0) && (glm::dot(location - ball.location, ball.velocity) < 0))
    {
        std::cout << "back error" << std::endl;
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
    os << std::setw(8) << ball.number << " |"
       << ball.location << ball.velocity
       << std::setw(8) << ball.mass << " |"
       << std::setw(8) << ball.radius << " |"
       << std::setw(8) << ball.count << " |"
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::shared_ptr<Ball>> &balls)
{
    os << "-----------Balls------------------------------------------------------------------------------------" << std::endl;
    os << "   Ball  | locX    | loxY    | locZ    | velX    | velY    | velZ    |  Mass   | Radius  | cnt     |"
       // "       1 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 |"
       << std::endl;
    for (auto const &i : balls)
        os << *i << std::endl;
    return os;
}
