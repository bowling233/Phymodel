#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#define square(x) ((x) * (x))

class FixedObject;
class FixedPoint;
class FixedBall;
class Wall;
class Point;
class Ball;
//全部使用float类型
//名称约定：私有数据用全名，接口函数简写，大写分隔单词，尽量不使用下划线

//Abstract base class--------------------------------------------------------------------------------
class FixedObject //protected:location,count*,state*
{
public:
    //construct
    FixedObject() : location(glm::vec3(0.0f)){}
    FixedObject(const glm::vec3& loc) : location(loc) {}
    virtual ~FixedObject() = default;
    //information
    glm::vec3 loc() const { return location; }
    unsigned int cnt() const { return count; }
    bool st() const { return state; }
    //action
protected:
    glm::vec3 location;
    unsigned int count = 0;
    bool state = true;
};
//说明：默认创建一个在原点的物体

class MovableObject : public virtual FixedObject //protected:velocity,mass  //virtual from:FixedObject           //virtual:bounceOff,predict,judge
{
public:
    //construct
    //MovableObject() : MovableObject(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    MovableObject() : MovableObject(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f){}
    MovableObject(const glm::vec3& loc, const glm::vec3& vel, const float m) : FixedObject(loc), velocity(vel), mass(m) {}
    MovableObject(const glm::vec3& vel, const float m) : velocity(vel), mass(m) {}
    //information
    glm::vec3 vel() { return velocity; }
    float m() { return mass; }
    float ek(){return 0.5f * m * square(glm::length(velocity));}
    //action
    void move(float t) { this->location += velocity * t; }
    //abstract virtual
    virtual bool judge(FixedPoint&) = 0;
    virtual bool judge(FixedBall&) = 0;
    virtual bool judge(FixedPoint&) = 0;
    /*
    virtual void bounceOff(FixedObject &) = 0;
    virtual float predict(FixedObject &) = 0;
    virtual bool judge(FixedObject &) = 0;
    */
    

protected:
    glm::vec3 velocity;
    float mass;
};
//说明：默认拥有向x轴正方向的速度1，质量1

//Concrete class--------------------------------------------------------------------------------

//Fixed +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FixedPoint final : public FixedObject //Ac//new:0
{
public:
    //construct
    FixedPoint() = default;
    FixedPoint(const glm::vec3& loc) :FixedObject(loc){}
    ~FixedPoint() = default;
};

class FixedBall : public virtual FixedObject //Ac//protected:radius    //virtual from:FixedObject
{
public:
    //construct//Ac
    FixedBall() : radius(1.0f){}
    FixedBall(const float r) : radius(r) {}
    FixedBall(const glm::vec3& loc, const float r) : FixedObject(loc), radius(r) {}
    ~FixedBall() = default;
    //information//Ac
    float r() const{ return radius; }
protected:
    float radius;
};

class Wall final : public FixedObject //private:normalVector
{
    //friend
    //friend std::istream &read(std::istream &, Wall &);
    //friend std::ostream &print(std::ostream &, const Wall &);

public:
    //construct
    Wall() : normalVector(glm::vec3(0.0f, 0.0f, 1.0f)){}
    Wall(const glm::vec3& norv) : normalVector(norv) {}
    Wall(const glm::vec3& loc, const glm::vec3& norv) :FixedObject(loc), normalVector(norv){}
    ~Wall() = default;
    //information
    glm::vec3 norm() const { return normalVector; }
private:
    glm::vec3 normalVector;
};
/*
std::istream &read(std::istream &, Wall &);
std::ostream &print(std::ostream &, const Wall &);
void vecprint(std::ostream &os, const std::vector<Wall> &Walls);
*/
//Movable +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Point final : public MovableObject //new:0
{
    //friend
    //friend std::istream &read(std::istream &, Point &);
    //friend std::ostream &print(std::ostream &, const Point &);

public:
    //construct
    Point() = default;
    Point(const glm::vec3& loc, const glm::vec3& vel,const float m) : MovableObject(loc, vel, m) {}
    //Point(std::istream &is) : Point(){read(is, *this)};

    //action
    bool judge(FixedPoint&)override;

private:
    //no
}; 
/*
std::istream &read(std::istream& is, Point& point);
std::ostream &print(std::ostream &os, const Point &point);


class Ball final : public MovableObject, public FixedBall //new:r
{
    //friend
    friend std::istream &read(std::istream &, Ball &);
    friend std::ostream &print(std::ostream &, const Ball &);

public:
    //construct
    Ball() =default;
    Ball(glm::vec3& loc, glm::vec3& vel, float m, float r) : FixedObject(loc), MovableObject(vel, m), FixedBall(r) {}
    //Ball(std::istream& is) : Ball() { read(is, *this); }
    ~Ball() = default;
};
/*
class Ball
{
    //友元



public: //接口
    //构造函数
    Ball() : Ball(1.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 0) {}
    Ball(const float &mass, const float &radium,
         const glm::vec3 location, const glm::vec3 velocity,
         const int counter)
        : m(mass), r(radium), loc(location), vel(velocity), count(counter) {}
     //外部定义构造函数，从输入读取小球

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
*/
#endif