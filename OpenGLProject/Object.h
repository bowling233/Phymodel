#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>

#define square(x) ((x) * (x))

class Object;     //ac
class FixedPoint; //ac
class FixedBall;
class Wall;
class Point;
class Ball;
//not must
class Segment;
class Cuboid;
class Polygon;
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
    virtual unsigned int cnt() { return 0; }

protected:
    glm::vec3 location;
};
//说明：默认创建一个在原点的物体

//Concrete class--------------------------------------------------------------------------------

//Fixed +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FixedPoint final : public Object //Ac//new:0
{
public:
    //construct
    FixedPoint() = default;
    FixedPoint(const glm::vec3 &loc) : Object(loc) {}
    //copy move destruct
    FixedPoint(const FixedPoint &) = default;
    FixedPoint(FixedPoint &&) = default;
    FixedPoint &operator=(const FixedPoint &) = default;
    FixedPoint &operator=(FixedPoint &&) = default;
    ~FixedPoint() = default;
};

class FixedBall : public virtual Object //Ac//protected:radius    //virtual from:Object
{
public:
    //construct
    FixedBall() : radius(1.0f) {}
    FixedBall(const float r) : radius(r) {}
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

#ifdef EXOBJECT
class Polygon final:public Wall
{
public:
    //construct
    Polygon() = default;
    Polygon(const vector<glm::vec3> &vert) :
    {
        this->vertices = vert;
        numVert = vertices.size();
    }
    //copy move destruct
    Object(const Object &) = default;
    Object(Object &&) = default;
    Object &operator=(const Object &) = default;
    Object &operator=(Object &&) = default;
    ~Polygon() = default;

    //informatioin
    vector<glm::vec3> vert() const { return vertices; }
    unsigned int num() const { return numVert; }

private:
    vector<glm::vec3> vertices;
    unsigned int numVert;
};

class Segment final:public Object
{
public:
    //construct
    Segment() : vertices{glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)} {}
    Segment(const glm::vec3 &front, const glm::vec3 &end) : vertices{front, end} {}
    ~Segment() = default;
    //information


private:
    glm::vec3 end;
};

class Cuboid final:public Object
{
public:
    //construct
    Cuboid() : vertices{
                   glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                   glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                   glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)} {}
    Cuboid(const glm::vec3 &vert1, const glm::vec3 &vert2, const glm::vec3 &vert3,
           const glm::vec3 &vert4, const glm::vec3 &vert5, const glm::vec3 &vert6)
        : vertices{vert1, vert2, vert3, vert4, vert5, vert6} {}
    ~Cuboid() = default;
    //information
    array<glm::vec3, 6> vert() const { return vertices; }

private:
    array<glm::vec3, 5> vertices;
};
#endif


//Movable +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class MovableObject : public virtual Object //protected:velocity,mass,count*  //virtual from:Object   //virtual:bounceOff,predict
{
public:
    //construct
    MovableObject() : MovableObject(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f) {}
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
    unsigned int cnt() { return count; }
    //action
    void move(float t) { location += velocity * t; }
    void reverse() { velocity = -velocity; }

    //abstract virtual
    //predict
    virtual float predict(FixedPoint &) = 0;
   /* virtual float predict(FixedBall &) = 0;
    virtual float predict(Wall &) = 0;
    virtual float predict(Point &) = 0;
    virtual float predict(Ball &) = 0;
    /*float predict(Object &obj)
    {
        if (obj.type() == FIXEDPOINT)
            return this->predict(obj.fixedpoint());
    }
    
    //bounceOff
    virtual void bounceOff(FixedPoint &) = 0;
    virtual void bounceOff(FixedBall &) = 0;
    virtual void bounceOff(Wall &) = 0;
    virtual void bounceOff(Point &) = 0;
    virtual void bounceOff(Ball &) = 0;
    */
protected:
    glm::vec3 velocity;
    float mass;
    unsigned int count = 0;
};
//说明：默认拥有向x轴正方向的速度1，质量1

class Point final : public MovableObject //new:0
{
    //friend
    //friend std::istream &read(std::istream &, Point &);
    //friend std::ostream &print(std::ostream &, const Point &);

public:
    //construct
    Point() = default;
    Point(const glm::vec3 &loc, const glm::vec3 &vel, const float m) : MovableObject(loc, vel, m) {}
    //copy move destruct
    Point(const Point &) = default;
    Point(Point &&) = default;
    Point &operator=(const Point &) = default;
    Point &operator=(Point &&) = default;
    ~Point() = default;
    //Point(std::istream &is) : Point(){read(is, *this)};

    //abstract virtual
    //predict
    float predict(FixedPoint &) override;
    /*
    float predict(FixedBall &) override;
    float predict(Wall &) override;
    float predict(Point &) override;
    float predict(Ball &) override;
    //bounceOff
    void bounceOff(FixedPoint &) override;
    void bounceOff(FixedBall &) override;
    void bounceOff(Wall &) override;
    void bounceOff(Point &) override;
    void bounceOff(Ball &) override;
    */
};
/*
std::istream &read(std::istream& is, Point& point);
std::ostream &print(std::ostream &os, const Point &point);
*/

class Ball final : public MovableObject, public FixedBall //no new
{
    //friend
    //friend std::istream &read(std::istream &, Ball &);
    //friend std::ostream &print(std::ostream &, const Ball &);
public:
    //construct
    Ball() = default;
    Ball(glm::vec3 &loc, glm::vec3 &vel, float m, float r) : Object(loc), MovableObject(vel, m), FixedBall(r) {}
    //Ball(std::istream& is) : Ball() { read(is, *this); }
    //copy move destruct
    Ball(const Ball &) = default;
    Ball(Ball &&) = default;
    Ball &operator=(const Ball &) = default;
    Ball &operator=(Ball &&) = default;
    ~Ball() = default;
    //action
};



#endif