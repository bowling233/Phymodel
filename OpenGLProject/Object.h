#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#define square(x) ((x) * (x))

class Object;      //todo
class FixedObject; //ac
class FixedPoint;  //ac
class FixedBall;
class Wall;
class Point;
class Ball;
class Segment;
class Cuboid;
class Polygon;
//全部使用float类型
//名称约定：私有数据用全名，接口函数简写，大写分隔单词，尽量不使用下划线

//Abstract base class--------------------------------------------------------------------------------
class FixedObject //protected:location,count*,state*
{
public:
    //construct
    FixedObject() : location(glm::vec3(0.0f)) {}
    FixedObject(const glm::vec3 &loc) : location(loc) {}
    virtual ~FixedObject() = default;
    //information
    glm::vec3 loc() const { return location; }
protected:
    glm::vec3 location;
};
//说明：默认创建一个在原点的物体

class MovableObject : public virtual FixedObject //protected:velocity,mass  //virtual from:FixedObject           //virtual:bounceOff,predict,judge
{
public:
    //construct
    MovableObject() : MovableObject(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f) {}
    MovableObject(const glm::vec3 &loc, const glm::vec3 &vel, const float m) : FixedObject(loc), velocity(vel), mass(m) {}
    MovableObject(const glm::vec3 &vel, const float m) : velocity(vel), mass(m) {}
    //information
    glm::vec3 vel() { return velocity; }
    float m() { return mass; }
    float ek() { return 0.5f * m * square(glm::length(velocity)); }
    //action
    void move(float t) { location += velocity * t; }
    void reverse() { velocity = -velocity; }
    //abstract virtual
    //predict
    virtual float predict(FixedPoint &) = 0;
    virtual float predict(FixedBall &) = 0;
    virtual float predict(Wall &) = 0;
    virtual float predict(Point &) = 0;
    virtual float predict(Ball &) = 0;
    float predict(Object &obj)
    {
        if (obj.type() == FIXEDPOINT)
            return this->predict(obj.fixedpoint());
    }

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
    FixedPoint(const glm::vec3 &loc) : FixedObject(loc) {}
    ~FixedPoint() = default;
};

class FixedBall : public virtual FixedObject //Ac//protected:radius    //virtual from:FixedObject
{
public:
    //construct//Ac
    FixedBall() : radius(1.0f) {}
    FixedBall(const float r) : radius(r) {}
    FixedBall(const glm::vec3 &loc, const float r) : FixedObject(loc), radius(r) {}
    ~FixedBall() = default;
    //information//Ac
    float r() const { return radius; }

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
    Wall() : normalVector(glm::vec3(0.0f, 0.0f, 1.0f)) {}
    Wall(const glm::vec3 &norv) : normalVector(norv) {}
    Wall(const glm::vec3 &loc, const glm::vec3 &norv) : FixedObject(loc), normalVector(norv) {}
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
    Point(const glm::vec3 &loc, const glm::vec3 &vel, const float m) : MovableObject(loc, vel, m) {}
    ~Point() = default;
    //Point(std::istream &is) : Point(){read(is, *this)};

    //action
    bool judge(FixedPoint &) override;

    void bounceOff(FixedPoint &) override;

private:
    //no
};
/*
std::istream &read(std::istream& is, Point& point);
std::ostream &print(std::ostream &os, const Point &point);
*/

class Ball final : public MovableObject, public FixedBall //new:0
{
    //friend
    //friend std::istream &read(std::istream &, Ball &);
    //friend std::ostream &print(std::ostream &, const Ball &);
public:
    //construct
    Ball() = default;
    Ball(glm::vec3 &loc, glm::vec3 &vel, float m, float r) : FixedObject(loc), MovableObject(vel, m), FixedBall(r) {}
    //Ball(std::istream& is) : Ball() { read(is, *this); }
    ~Ball() = default;
    //action
};
class Segment
{
public:
    //construct
    Segment() : vertices{glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)} {}
    Segment(const glm::vec3 &front, const glm::vec3 &end) : vertices{front, end} {}
    ~Segment() = default;
    //information
    array<glm::vec3, 2> vert() const { return vertices; }

private:
    array<glm::vec3, 2> vertices;
};
class Cuboid
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
    array<glm::vec3, 6> vertices;
};
class Polygon
{
public:
    //construct
    Polygon() = default;
    Polygon(const vector<glm::vec3> &vert) :
    {
        this->vertices = vert;
        numVert = vertices.size();
    }
    ~Polygon() = default;
    //informatioin
    vector<glm::vec3> vert() const { return vertices; }
    unsigned int num() const { return numVert; }

private:
    vector<glm::vec3> vertices;
    unsigned int numVert;
};

class Object
{
public:
    Object() : type(FP), fixedpoint(FixedPoint()) {}
    Object(const FixedPoint &obj) : type(FP), fixedpoint(obj) {}
    Object(const FixedBall &obj) : type(FB), fixedball(obj) {}
    Object(const Wall &obj) : type(W), wall(obj) {}
    Object(const Point &obj) : type(P), point(obj) {}
    Object(const Ball &obj) : type(B), ball(obj) {}
    Object(const Segment &obj) : type(S), segment(obj) {}
    Object(const Cuboid &obj) : type(C), cuboid(obj) {}
    Object(const Polygon &obj) : type(POLY), polygon(obj) {}
    ~Object() :
    {
        if (type == FP)
            this->fixedpoint.~FixedPoint();
        if (type == FB)
            this->fixedball.~FixedBall();
        if (type == W)
            this->wall.~Wall();
        if (type == P)
            this->point.~Point();
        if (type == B)
            this->ball.~Ball();
        if (type == S)
            this->segment.~Segment();
        if (type == C)
            this->cuboid.~Cuboid();
        if (type == POLY)
            this->polygon.~Polygon();
    }
    //information
    unsigned int cnt() const { return count; }
    //action
    void bounceOff(Object& obj)
    {
        if(type == P)
            this->point.bounceOff(obj);
    }
private:

    unsigned int count = 0;
    eunm{FP, FB, W, P, B, S, C, POLY} type;
    union
    {
        FixedPoint fixedpoint;
        FixedBall fixedball;
        Wall wall;
        Point point;
        Ball ball;
        Segment segment;
        Cuboid cuboid;
        Polygon polygon;
    }
};
#endif