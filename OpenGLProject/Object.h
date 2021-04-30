#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>

class Object;
class MovableObject;
class FixedBall;
class Wall;
class Ball;

//definition
enum class Object_type
{
    NUL,
    FIXEDBALL,
    BALL,
    WALL
};

//tools
#define square(x) ((x) * (x))
std::ostream &operator<<(std::ostream &, const glm::vec3 &);
std::istream &operator>>(std::istream &, glm::vec3 &);

//全部使用float类型
//名称约定：私有数据用全名，接口函数简写，大写分隔单词，尽量不使用下划线

//Abstract base class--------------------------------------------------------------------------------
class Object //protected:location
{
public:
    //construct
    Object() : location(glm::vec3(0.0f)) {}
    Object(const glm::vec3 &loc) : location(loc) {}
    //copy move destruct
    Object(const Object &) = default;
    Object(Object &&) = default;
    Object &operator=(const Object &) = default;
    Object &operator=(Object &&) = default;
    virtual ~Object() = default;

    //information
    glm::vec3 loc() const { return location; }
    virtual unsigned int cnt() = 0;
    virtual Object_type type() = 0;
    virtual unsigned int num() = 0;

protected:
    glm::vec3 location;
    unsigned int count = 0;
};
//说明：默认创建一个在原点的物体





//FixedBall ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FixedBall : public Object //protected:radius
{
    //io
    friend std::istream &operator>>(std::istream &, FixedBall &);
    friend std::ostream &operator<<(std::ostream &, const FixedBall &);

public:
    //construct
    FixedBall() : radius(1.0f) {}
    FixedBall(const float &r) : radius(r) {}
    FixedBall(const glm::vec3 &loc, const float r) : Object(loc), radius(r) {}
    FixedBall(std::istream &);
    //copy move destruct
    FixedBall(const FixedBall &) = default;
    FixedBall(FixedBall &&) = default;
    FixedBall &operator=(const FixedBall &) = default;
    FixedBall &operator=(FixedBall &&) = default;
    ~FixedBall() = default;

    //information
    float r() const { return radius; }
    unsigned int cnt() { return 0; }
    Object_type type() { return Object_type::FIXEDBALL; }
    unsigned int num() { return number; }

protected:
    float radius;

private:
    static unsigned int sum;
    unsigned int number = 0;
};
std::istream &operator>>(std::istream &, FixedBall &);
std::ostream &operator<<(std::ostream &, const FixedBall &);

std::ostream &operator<<(std::ostream &, const std::vector<std::shared_ptr<FixedBall>> &);





//Wall ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Wall : public Object //private:normalVector
{
    //friend
    //io
    friend std::istream &operator>>(std::istream &, Wall &);
    friend std::ostream &operator<<(std::ostream &, const Wall &);

public:
    //construct
    Wall() : normalVector(glm::vec3(0.0f, 0.0f, 1.0f)) {}
    Wall(const glm::vec3 &norv) : normalVector(norv) {}
    Wall(const glm::vec3 &loc, const glm::vec3 &norv) : Object(loc), normalVector(norv) {}
    Wall(std::istream &);
    //copy move destruct
    Wall(const Wall &) = default;
    Wall(Wall &&) = default;
    Wall &operator=(const Wall &) = default;
    Wall &operator=(Wall &&) = default;
    ~Wall() = default;

    //information
    glm::vec3 norm() const { return normalVector; }
    unsigned int cnt() { return 0; }
    Object_type type() { return Object_type::WALL; }
    unsigned int num() { return number; }

protected:
    glm::vec3 normalVector;

private:
    static unsigned int sum;
    unsigned int number = 0;
};
std::istream &operator>>(std::istream &, Wall &);
std::ostream &operator<<(std::ostream &, const Wall &);

std::ostream& operator<<(std::ostream&, const std::vector<FixedBall>&);





//Ball ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Ball final : public FixedBall //velocity,mass
{
    //friend
    //io
    friend std::istream &operator>>(std::istream &, Ball &);
    friend std::ostream &operator<<(std::ostream &, const Ball &);

public:
    //construct
    Ball() : velocity(glm::vec3(1.0f, 0.0f, 0.0f)), mass(1.0f) {} //说明：默认拥有向x轴正方向的速度1，质量1
    Ball(glm::vec3 loc, glm::vec3 vel, float m, float r) : FixedBall(loc, r), velocity(vel), mass(m) {}
    Ball(std::istream &);
    //Ball递增自己的计数器

    //copy move destruct
    Ball(const Ball &) = default;
    Ball(Ball &&) = default;
    Ball &operator=(const Ball &) = default;
    Ball &operator=(Ball &&) = default;
    ~Ball() = default;

    //information
    glm::vec3 vel() { return velocity; }
    float m() { return mass; }
    float ek() { return 0.5f * mass * square(glm::length(velocity)); }
    unsigned int cnt() { return count; }
    Object_type type() { return Object_type::BALL; }
    unsigned int num() { return number; }

    //action
    void move(float t) { location += velocity * t; }
    void rev() { velocity = -velocity; }

    //predict
    float predict(Object &);
    float predict(const FixedBall &);
    float predict(const Wall &);
    float predict(Ball &);

    //bounce
    void bounce(Object &);
    void bounce(const FixedBall &);
    void bounce(const Wall &);
    void bounce(Ball &);

private:
    glm::vec3 velocity;
    float mass;
    static unsigned int sum;
    unsigned int number = 0,count = 0;
};

std::istream &operator>>(std::istream &, Ball &);
std::ostream &operator<<(std::ostream &, const Ball &);

std::ostream &operator<<(std::ostream &, const std::vector<std::shared_ptr<Ball>> &);

#endif