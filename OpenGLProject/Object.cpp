#include <iostream>
#include "Object.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <glm\glm.hpp>

//vec3 tools
std::ostream &operator<<(std::ostream &os, glm::vec3 &v)
{
    os << v.x << v.y << v.z;
}

std::istream &operator>>(std::istream &is, glm::vec3 &v)
{
    is >> v.x >> v.y >> v.z;
}

//Ball------------------------------------------------------
//predict
float Ball::preditc(const FixedBall &fixedball) //todo
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

float Ball::predict(Ball &ball)//tochk
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
    float c = square(dx) + square(dy) + square(dz) - square(r + ball.r);
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

//bounceOff
void Ball::bounceOff(const FixedBall &fixedball)//todo
{
}

void Ball::bounceOff(const Wall &wall) //tochk
{
    glm::vec3 ckd_nor = glm::dot((location - wall.loc()), wall.norm()) > 0 ? wall.norm() : -wall.norm();
    velocity = velocity - 2 * (dot(velocity, ckd_nor)) * ckd_nor;
    //std::cout << "handle wall end" << std::endl;
}

void Ball::bounceOff(Ball &ball) //tochk
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
    is >> ball.mass >> ball.radius >> ball.location.x >> ball.location.y >> ball.location.z >> ball.velocity.x >> ball.velocity.y >> ball.velocity.z;
    /*if (is.good())
        std::cout << "read in ball finished" << std::endl;
    /*/
    return is;
}

std::ostream &operator<<(std::ostream &os, const Ball &ball)
{
    os << std::setprecision(2) << std::fixed;
    os << std::setw(5) << ball.m << " |"
       << std::setw(5) << ball.r << " |"
       << std::setw(5) << ball.location.x << " |"
       << std::setw(5) << ball.location.y << " |"
       << std::setw(5) << ball.location.z << " |"
       << std::setw(5) << ball.vel.x << " |"
       << std::setw(5) << ball.vel.y << " |"
       << std::setw(5) << ball.vel.z << " |"
       << std::setw(5) << ball.count
       << std::endl
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Ball> &balls)
{
    os << "-----------Balls---------------------------------------------------" << std::endl;
    os << "Ball | Mass | Radi | locX | loxY | locZ | velX | velY | velZ | cout" << std::endl;
    int numBalls = (int)(balls.size());
    for (int i = 0; i != numBalls; i++)
    {
        os << std::setw(5) << i << '|';
        print(os, balls[i]);
    }
    return os;
}

//Wall---------------------------------------------------------------------------------------------

std::istream &operator>>(std::istream &is, Wall &wall)
{
    is >> wall.location.x >> wall.location.y >> wall.location.z >> wall.normalVector.x >> wall.normalVector.y >> wall.normalVector.z;
    /*if (is.good())
        std::cout << "read in wall finished" << std::endl;
    /*/
    return is;
}

std::ostream &operator<<(std::ostream &os, const Wall &wall)
{
    os << std::setprecision(2) << std::fixed;
    os << wall.location.x << " |"
       << wall.location.y << " |"
       << wall.location.z << " |"
       << wall.normalVector.x << " |"
       << wall.normalVector.y << " |"
       << wall.normalVector.z
       << std::endl
       << std::defaultfloat;
    return os;
}

/*
//probably no use
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
* /