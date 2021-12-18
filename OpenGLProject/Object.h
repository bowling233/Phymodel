﻿#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

//Notice：为保证计算精确度，Object类内部全部使用double和dvec3类型储存和计算，传送至OpenGL时统一转换为vec3类型

//definition
class Object;
class Ball;
class Wall;
class Container;

enum class Object_type
{
    BALL,
    WALL,
    CONTAINER
};

//tools
#define square(x) ((x) * (x))
std::ostream &operator<<(std::ostream &, const glm::dvec3 &);
std::istream &operator>>(std::istream &, glm::dvec3 &);
std::ostream &operator<<(std::ostream &, const glm::vec3 &);
std::istream &operator>>(std::istream &, glm::vec3 &);





//各类型定义
class Object //protected:location
{
public:
    //construct
    Object() : Object(glm::dvec3(0.0f)) {}
    Object(const glm::dvec3 &loc) : location(loc) {}
    //copy move destruct
    Object(const Object &) = default;
    Object(Object &&) = default;
    Object &operator=(const Object &) = default;
    Object &operator=(Object &&) = default;
    virtual ~Object() = default;

    //information
    glm::vec3 loc() const { return location; }

    //virtual
    virtual unsigned int cnt() const = 0;
    virtual Object_type type() const = 0;
    virtual unsigned int num() const = 0;

protected:
    glm::dvec3 location;
};

class Ball final : public Object //private:velocity,mass
{
    //friend io
    friend std::istream &operator>>(std::istream &, Ball &);
    friend std::ostream &operator<<(std::ostream &, const Ball &);

public:
    //construct
    Ball() : Ball(glm::dvec3(0.0f), glm::dvec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f) {}
    Ball(glm::dvec3 loc, glm::dvec3 vel, double m, double r) : Object(loc), velocity(vel), mass(m), radius(r), number(++sum) {}
    Ball(std::istream &);
    //copy move destruct
    Ball(const Ball &) = default;
    Ball(Ball &&) = default;
    Ball &operator=(const Ball &) = default;
    Ball &operator=(Ball &&) = default;
    ~Ball() = default;

    //information
    glm::vec3 v() const { return velocity; }
    float r() const { return radius; }
    double m() const { return mass; }
    double ek() const { return 0.5f * mass * square(glm::length(velocity)); }
    unsigned int cnt() const { return count; }
    Object_type type() const { return Object_type::BALL; }
    unsigned int num() const { return number; }

    //action
    void move(double t) { location += velocity * t; }
    void reverse() { velocity = -velocity; }

    //predict
    double predict(const Ball &);
    double predict(const Wall &);
    double predict(const Container &);

    //bounce
    void bounce(Object &);
    void bounce(Ball &);
    void bounce(Wall &);
    void bounce(Container &);

    //examine
    bool examine(const Ball &);
    bool examine(const Wall &);
    bool examine(const Container &);


private:
    bool back(const Ball &);
    glm::dvec3 velocity;
    double mass, radius;
    static unsigned int sum; //extern
    unsigned int number = 0, count = 0;
};
std::istream &operator>>(std::istream &, Ball &);
std::ostream &operator<<(std::ostream &, const Ball &);
std::ostream &operator<<(std::ostream &, const std::vector<std::shared_ptr<Ball>> &);

class Wall final : public Object //private:normalVector
{
    //friend io
    friend std::istream &operator>>(std::istream &, Wall &);
    friend std::ostream &operator<<(std::ostream &, const Wall &);
    friend class Ball;

public:
    //construct
    Wall() : Wall(glm::dvec3(0.0), glm::dvec3(0.0, 0.0, 1.0)) {}
    Wall(const glm::dvec3 &loc, const glm::dvec3 &norv) : Object(loc), normalVector(norv), number(++sum) {}
    Wall(std::istream &);
    //copy move destructj
    Wall(const Wall &) = default;
    Wall(Wall &&) = default;
    Wall &operator=(const Wall &) = default;
    Wall &operator=(Wall &&) = default;
    ~Wall() = default;

    //information
    glm::vec3 norm() const { return normalVector; }
    unsigned int cnt() const { return 0; }
    Object_type type() const { return Object_type::WALL; }
    unsigned int num() const { return number; }

private:
    glm::dvec3 normalVector;
    static unsigned int sum;
    unsigned int number = 0;
};
std::istream &operator>>(std::istream &, Wall &);
std::ostream &operator<<(std::ostream &, const Wall &);
std::ostream &operator<<(std::ostream &, const std::vector<std::shared_ptr<Wall>> &);

class Container final : public Object //private:length
{
    //friend io
    friend std::istream &operator>>(std::istream &, Container &);
    friend std::ostream &operator<<(std::ostream &, const Container &);

public:
    //construct
    Container() : Container(glm::dvec3(0.0f), glm::dvec3(1.0f)) {}
    Container(glm::dvec3 loc, glm::dvec3 len) : Object(loc), length(len), number(++sum) {}
    Container(std::istream &);

    //copy move destruct
    Container(const Container &) = default;
    Container(Container &&) = default;
    Container &operator=(const Container &) = default;
    Container &operator=(Container &&) = default;
    ~Container() = default;

    //information
    unsigned int cnt() const { return 0; }
    Object_type type() const { return Object_type::CONTAINER; }
    unsigned int num() const { return number; }
    double x_p() const { return location.x + length.x / 2; } //positive
    double x_n() const { return location.x - length.x / 2; } //negative
    double y_p() const { return location.y + length.y / 2; }
    double y_n() const { return location.y - length.y / 2; }
    double z_p() const { return location.z + length.z / 2; }
    double z_n() const { return location.z - length.z / 2; }

    glm::vec3 scale() { return length; }

private:
    glm::dvec3 length;
    static unsigned int sum;            //extern
    unsigned int number, count = 0; //默认为0
};
std::istream &operator>>(std::istream &, Container &);
std::ostream &operator<<(std::ostream &, const Container &);
std::ostream &operator<<(std::ostream &, const std::vector<std::shared_ptr<Container>> &);


#endif