#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#define square(x) ((x) * (x))
//全部使用float类型
//名称约定：私有数据用单字母，接口函数都全名，大写分隔单词，尽量不使用下划线

class Wall;
class Event;

//Ball-------------------------------------------------------------

class Ball
{
    //友元
    friend class Event;
    friend std::istream &read(std::istream &, Ball &);
    friend std::ostream &print(std::ostream &, const Ball &);

public: //接口
    //构造函数
    Ball() : Ball(1.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 0) {}
    Ball(const float &mass, const float &radium,
         const glm::vec3 location, const glm::vec3 velocity,
         const int counter)
        : m(mass), r(radium), loc(location), vel(velocity), count(counter) {}
    Ball(std::istream &is) : Ball() { read(is, *this); } //外部定义构造函数，从输入读取小球

    //返回
    float mass() const { return m; }
    float radium() const { return r; }
    glm::vec3 location() const { return loc; }
    glm::vec3 velocity() const { return vel; }
    float energe_k() const { return 0.5f * m * square(glm::length(vel)); }
    int count() const { return count; }

    //操作：移动。判断。
    void move(const float time) { (*this).loc += time * (*this).vel; }
    float timeToCollision(const Ball &) const;
    float timeToCollision(const Wall &) const;
    void handleCollision(Ball &);
    void handleCollision(const Wall &);

private: //私有
    float m, r;
    glm::vec3 loc, vel;
    int count = 0;
    bool state = true;
};

std::istream &read(std::istream &is, Ball &ball);
std::ostream &print(std::ostream &os, const Ball &ball);
void vecprint(std::ostream &os, const std::vector<Ball> &balls);

//Wall-------------------------------------------------------------

//notice:Wall的nor向量在创建时就已经归一化
class Wall
{
    friend class Ball;
    friend std::istream &read(std::istream &, Wall &);
    friend std::ostream &print(std::ostream &, const Wall &);

public:
    Wall() : Wall(glm::vec3(10.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0)) {}
    Wall(const glm::vec3 location, const glm::vec3 n) : loc(location), nor(glm::normalize(n)) {}
    glm::vec3 location() const { return this->loc; }
    glm::vec3 norvec() const { return this->nor; }

private:
    glm::vec3 loc;
    glm::vec3 nor;
};

std::istream &read(std::istream &, Wall &);
std::ostream &print(std::ostream &, const Wall &);
void vecprint(std::ostream &os, const std::vector<Wall> &Walls);

#endif