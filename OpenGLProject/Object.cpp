#include <iostream>
#include "Object.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <string>

void inline printvec3(glm::vec3 v) { std::cout << ' ' << v.x << ' ' << v.y << ' ' << v.z << ' '; }

class Wall;

//操作------------------------------------------------------

float Ball::timeToCollision(const Ball &ball) const
{
    glm::vec3 r = this->loc - ball.loc;

    if (glm::dot(-r, this->vel) < 0)
        return -1.0;
    if (glm::dot(r, ball.vel) < 0)
        return -1.0;

    float dvx = (*this).vel.x - ball.vel.x, dvy = (*this).vel.y - ball.vel.y, dvz = (*this).vel.z - ball.vel.z;
    float dx = (*this).loc.x - ball.loc.x, dy = (*this).loc.y - ball.loc.y, dz = (*this).loc.z - ball.loc.z;
    float a = square(dvx) + square(dvy) + square(dvz);
    float b = 2.0 * (dvx * dx + dvy * dy + dvz * dz);
    float c = square(dx) + square(dy) + square(dz) - square((*this).r + ball.r);
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

float Ball::timeToCollision(const Wall &wall) const //tochk
{
    glm::vec3 r = (*this).loc - wall.loc;
    float r_l = glm::dot(r, wall.nor);           //球到平面的垂直距离
    float v_l = glm::dot((*this).vel, wall.nor); //速度在平面法向量方向上的分量

    if (v_l * r_l >= 0)
        return -1.0f;
    return -r_l / v_l;
}

void Ball::handleCollision(Ball &ball)
{
    glm::vec3 r = glm::normalize(this->loc - ball.loc);
    float v10 = glm::dot(r, this->vel);
    float v20 = glm::dot(r, ball.vel);
    float m1 = this->m;
    float m2 = ball.m;

    float v1 = ((m1 - m2) * v10 + 2 * m2 * v20) / (m1 + m2);
    float v2 = ((m2 - m1) * v20 + 2 * m1 * v10) / (m1 + m2);
    glm::vec3 dv1 = (v1 - v10) * r;
    glm::vec3 dv2 = (v2 - v20) * r;
    this->vel += dv1;
    ball.vel += dv2;

    /*debug_chunk
    std::cout << "handle ball info: "
              << "vecr:";
    printvec3(r);
    std::cout << "v10,v20" << v10 << v20 << m1 << m2
              << "v1,v2" << v1 << v2
              << "dv1,dv2";
    printvec3(dv1);
    printvec3(dv2);
    //*/

    std::cout << "handle ball end" << std::endl;
}

void Ball::handleCollision(const Wall &wall) //tochk
{
    glm::vec3 ckd_nor = glm::dot(((*this).loc - wall.loc), wall.nor) > 0 ? wall.nor : -wall.nor;
    (*this).vel = (*this).vel - 2 * (dot((*this).vel, ckd_nor)) * ckd_nor;
    std::cout << "handle wall end" << std::endl;
}

//其他操作函数------------------------------------------------------
std::istream &read(std::istream &is, Ball &ball) //ball_related
{
    is >> ball.m >> ball.r >> ball.loc.x >> ball.loc.y >> ball.loc.z >> ball.vel.x >> ball.vel.y >> ball.vel.z >> ball.count;
    if (is.good())
        std::cout << "read in ball finished" << std::endl;
    return is;
}

std::ostream &print(std::ostream &os, const Ball &ball) //ball_related
{
    os << std::setprecision(2) << std::fixed;
    os << std::setw(5) << ball.m << " |"
       << std::setw(5) << ball.r << " |"
       << std::setw(5) << ball.loc.x << " |"
       << std::setw(5) << ball.loc.y << " |"
       << std::setw(5) << ball.loc.z << " |"
       << std::setw(5) << ball.vel.x << " |"
       << std::setw(5) << ball.vel.y << " |"
       << std::setw(5) << ball.vel.z << " |"
       << std::setw(5) << ball.count
       << std::endl
       << std::defaultfloat;
    return os;
}

void vecprint(std::ostream &os, const std::vector<Ball> &balls) //ball_related
{
    os << "-----------Balls---------------------------------------------------" << std::endl;
    os << "Ball | Mass | Radi | locX | loxY | locZ | velX | velY | velZ | cout" << std::endl;
    int numBalls = (int)(balls.size());
    for (int i = 0; i != numBalls; i++)
    {
        os << std::setw(5) << i << '|';
        print(os, balls[i]);
    }
}

//Wall---------------------------------------------------------------------------------------------

std::istream &read(std::istream &is, Wall &wall)
{
    is >> wall.loc.x >> wall.loc.y >> wall.loc.z >> wall.nor.x >> wall.nor.y >> wall.nor.z;
    if (is.good())
        std::cout << "read in wall finished" << std::endl;
    return is;
}

std::ostream &print(std::ostream &os, const Wall &wall)
{
    os << std::setprecision(2) << std::fixed;
    os << wall.loc.x << " |"
       << wall.loc.y << " |"
       << wall.loc.z << " |"
       << wall.nor.x << " |"
       << wall.nor.y << " |"
       << wall.nor.z
       << std::endl
       << std::defaultfloat;
    return os;
}

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
