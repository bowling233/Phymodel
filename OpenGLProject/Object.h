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
static unsigned int ballNum = 0;

//tools
#define square(x) ((x) * (x))
std::ostream& operator<<(std::ostream&, const glm::vec3&);
std::istream& operator>>(std::istream&, glm::vec3&);

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
    unsigned int cnt() { return count; }

protected:
    glm::vec3 location;
    unsigned int count = 0;
};
//说明：默认创建一个在原点的物体

//Concrete class--------------------------------------------------------------------------------

//Fixed +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class FixedBall : public virtual Object //Ac//protected:radius    //virtual from:Object
{
public:
    //construct
    FixedBall() : radius(1.0f) {}
    FixedBall(const float &r) : radius(r) {}
    FixedBall(const glm::vec3 &loc, const float r) : Object(loc), radius(r) {}
    //copy move destruct
    FixedBall(const FixedBall &) = default;
    FixedBall(FixedBall &&) = default;
    FixedBall &operator=(const FixedBall &) = default;
    FixedBall &operator=(FixedBall &&) = default;
    ~FixedBall() = default;

    //information
    float r() const { return radius; }

protected:
    float radius;
};

class Wall : public Object //private:normalVector
{
    //friend
    //io
    friend std::istream& operator>>(std::istream&, Wall&);
    friend std::ostream& operator<<(std::ostream&, const Wall&);
public:
    //construct
    Wall() : normalVector(glm::vec3(0.0f, 0.0f, 1.0f)) {}
    Wall(const glm::vec3 &norv) : normalVector(norv) {}
    Wall(const glm::vec3 &loc, const glm::vec3 &norv) : Object(loc), normalVector(norv) {}
    //copy move destruct
    Wall(const Wall &) = default;
    Wall(Wall &&) = default;
    Wall &operator=(const Wall &) = default;
    Wall &operator=(Wall &&) = default;
    ~Wall() = default;

    //information
    glm::vec3 norm() const { return normalVector; }

protected:
    glm::vec3 normalVector;
};


//Movable +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class MovableObject : public virtual Object //protected:velocity,mass  //virtual from:Object   //virtual:bounceOff,predict
{
public:
    //construct
    MovableObject() : velocity(glm::vec3(1.0f, 0.0f, 0.0f)), mass(1.0f) {}
    MovableObject(const glm::vec3 &loc, const glm::vec3 &vel, const float m) : Object(loc), velocity(vel), mass(m) {}
    MovableObject(const glm::vec3 &vel, const float m) : velocity(vel), mass(m) {}
    //copy move destruct
    MovableObject(const MovableObject &) = default;
    MovableObject(MovableObject &&) = default;
    MovableObject &operator=(const MovableObject &) = default;
    MovableObject &operator=(MovableObject &&) = default;
    virtual ~MovableObject() = default;

    //information
    glm::vec3 vel() { return velocity; }
    float m() { return mass; }
    float ek() { return 0.5f * mass * square(glm::length(velocity)); }

    //action
    void move(float t) { location += velocity * t; }
    void reverse() { velocity = -velocity; }

    //abstract virtual
    //predict
    virtual float predict(const FixedBall &) = 0;
    virtual float predict(const Wall &) = 0;
    virtual float predict(Ball &) = 0;

    //bounceOff
    virtual void bounceOff(const FixedBall &) = 0;
    virtual void bounceOff(const Wall &) = 0;
    virtual void bounceOff(Ball &) = 0;

protected:
    glm::vec3 velocity;
    float mass;
};
//说明：默认拥有向x轴正方向的速度1，质量1

class Ball final : public MovableObject, public FixedBall //no new
{
    //friend
    //io
    friend std::istream &operator>>(std::istream &, Ball &);
    friend std::ostream &operator<<(std::ostream &, const Ball &);

public:
    //construct
    Ball() = default;
    Ball(glm::vec3 loc, glm::vec3 vel, float m, float r) : Object(loc), MovableObject(vel, m), FixedBall(r) { }
    Ball(std::istream& is) :Ball(){is >> *this; }
    //copy move destruct
    Ball(const Ball &) = default;
    Ball(Ball &&) = default;
    Ball &operator=(const Ball &) = default;
    Ball &operator=(Ball &&) = default;
    ~Ball() = default;

    //action
    //abstract virtual
    //predict
    float predict(const FixedBall &);
    float predict(const Wall &);
    float predict(Ball &);

    //bounceOff
    void bounceOff(const FixedBall &) ;
    void bounceOff(const Wall &);
    void bounceOff(Ball &) ;
private:
    unsigned int number=++ballNum;
};
std::ostream& operator<<(std::ostream&, const std::vector<Ball>&);


#endif