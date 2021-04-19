#include <iostream>
#include "Object.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <glm\glm.hpp>

//vec3 tools

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

//Ball------------------------------------------------------
//predict
float Ball::predict(const FixedBall &fixedball) //todo
{
    return -1.0f;
}

float Ball::predict(const Wall &wall) //tochk
{
    glm::vec3 r = location - wall.loc();
    float r_l = glm::dot(r, wall.norm());        //球到平面的垂直距离
    float v_l = glm::dot(velocity, wall.norm()); //速度在平面法向量方向上的分量

    if (v_l * r_l >= 0)
        return -1.0f;
    return -r_l / v_l;
}

float Ball::predict(Ball &ball) //tochk
{
    glm::vec3 r = location - ball.location,
              dv = velocity - ball.velocity;
    /*/tochk
    if (glm::dot(-r, velocity) < 0)
        return -1.0;
    if (glm::dot(r, ball.velocity) < 0)
        return -1.0;
/*/

    float dvx = velocity.x - ball.velocity.x,
          dvy = velocity.y - ball.velocity.y,
          dvz = velocity.z - ball.velocity.z,
          dx = location.x - ball.location.x,
          dy = location.y - ball.location.y,
          dz = location.z - ball.location.z;

    //float a = square(dvx) + square(dvy) + square(dvz);
    float a = square(glm::length(dv));
    float b = 2.0 * (dvx * dx + dvy * dy + dvz * dz);
    float c = square(dx) + square(dy) + square(dz) - square(radius + ball.radius);
    float delta = square(b) - 4.0 * a * c;

    if (delta < 0.0)
        return -1.0;

    float x1 = ((-b + std::sqrt(delta)) / (2.0 * a));
    float x2 = ((-b - std::sqrt(delta)) / (2.0 * a)); //question:x2是较小的根吗？是的，因为sqrt(delta)一定是个正数，所以减去该项的一定更小。我们应该传回最小实根

    //debug_chunk
    //std::cout << dvx << ' ' << dvy << ' ' << dvz << ' ' << dx << ' ' << dy << ' ' << dz << ' ' << a << ' ' << b << ' ' << c << ' ' << delta << ' '<<x1 << ' ' << x2<<std::endl;

    //question:小根大于0必定返回小根吗？是这样的，我们要求离当前时刻最近的一次碰撞解，因此返回最小正根

    if (x2 > 0)
        return x2;
    if (x1 > 0)
        return x1;
    return -1.0;
}

//bounce
void Ball::bounce(Object &object)
{
    switch (object.type())
    {
    case:
        FIXEDBALL
        {
            FixedBall &fixedBall = dynamic_cast<FixedBall &>(object);
            this->bounce(fixedBall);
            break;
        }
    case:
        BALL
        {
            Ball &ball = dynamic_cast<Ball &>(object);
            this->bounce(ball);
            break;
        }
    case:
        WALL
        {
            Wall &wall = dynamic_cast<Wall &>(object);
            this->bounce(wall);
            break;
        }
    }
}
void Ball::bounce(const FixedBall &fixedball) //todo
{
}

void Ball::bounce(const Wall &wall) //tochk
{
    glm::vec3 ckd_nor = glm::dot((location - wall.loc()), wall.norm()) > 0 ? wall.norm() : -wall.norm();
    velocity = velocity - 2 * (dot(velocity, ckd_nor)) * ckd_nor;
    //std::cout << "handle wall end" << std::endl;
}

void Ball::bounce(Ball &ball) //tochk
{
    glm::vec3 r = glm::normalize(location - ball.location);
    float v10 = glm::dot(r, velocity),
          v20 = glm::dot(r, ball.velocity),
          m1 = mass,
          m2 = ball.mass;

    float v1 = ((m1 - m2) * v10 + 2 * m2 * v20) / (m1 + m2),
          v2 = ((m2 - m1) * v20 + 2 * m1 * v10) / (m1 + m2);
    glm::vec3 dv1 = (v1 - v10) * r,
              dv2 = (v2 - v20) * r;
    velocity += dv1;
    ball.velocity += dv2;

    /*debug_chunk
    std::cout << "handle ball info: "
              << "vecr:";
    printvec3(r);
    std::cout << "v10,v20" << v10 << v20 << m1 << m2
              << "v1,v2" << v1 << v2
              << "dv1,dv2";
    printvec3(dv1);
    printvec3(dv2);
    /*/

    //std::cout << "handle ball end" << std::endl;
}

//io
std::istream &operator>>(std::istream &is, Ball &ball)
{
    is >> ball.location >> ball.velocity >> ball.mass >> ball.radius;
    /*if (is.good())
        std::cout << "read in ball finished" << std::endl;
    /*/
    return is;
}

std::ostream &operator<<(std::ostream &os, const Ball &ball)
{
    os << std::setprecision(3) << std::fixed;
    os << std::setw(8) << ball.number << " |"
       << std::setw(8) << ball.mass << " |"
       << std::setw(8) << ball.radius << " |"
       << ball.location << ball.velocity
       << std::setw(8) << ball.count << " |"
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Ball> &balls)
{
    os << "-----------Balls------------------------------------------------------------------------------------" << std::endl;
    os << "   Ball  |  Mass   | Radius  | locX    | loxY    | locZ    | velX    | velY    | velZ    | cnt     |"
       // "       1 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 | 1111.00 |"
       << std::endl;
    for (auto const &i : balls)
        os << i << std::endl;
    return os;
}

//Wall---------------------------------------------------------------------------------------------

std::istream &operator>>(std::istream &is, Wall &wall)
{
    is >> wall.location >> wall.normalVector;
    return is;
}

std::ostream &operator<<(std::ostream &os, const Wall &wall)
{
    os << std::setprecision(3) << std::fixed;
    os << wall.location << wall.normalVector << std::endl
       << std::defaultfloat;
    return os;
}

/*
probably no use
void vecprint(std::ostream &os, const std::vector<Wall> &walls) //tochk
{
    os << "-----------walls---------------------------------------------------" << std::endl;
    os << "wall | locX | loxY | locZ | norX | norY | norZ |" << std::endl;
    int numWalls = (int)(walls.size());
    for (int i = 0; i != numWalls; i++)
    {
        os << std::setw(5) << i << '|';
        print(os, walls[i]);
    }
}
*/
std::istream &operator>>(std::istream &is, FixedBall &fixedBall)
{
    is >> fixedBall.location >> fixedBall.radius;
    return is;
}
std::ostream &operator<<(std::ostream &os, const FixedBall &fixedBall)
{
    os << std::setprecision(3) << std::fixed;
    os << fixedBall.location << fixedBall.radius << std::endl
       << std::defaultfloat;
    return os;
}
