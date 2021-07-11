#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

//definition
class Object;
class Wall;
class Ball;
class Container;

enum class Object_type
{
    BALL,
    WALL,
    CONTAINER
};

//tools
#define square(x) ((x) * (x))
std::ostream &operator<<(std::ostream &, const glm::vec3 &);
std::istream &operator>>(std::istream &, glm::vec3 &);



class Object //protected:location
{
public:
    //construct
    Object() : Object(glm::vec3(0.0f)) {}
    Object(const glm::vec3 &loc) : location(loc) {}
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
    glm::vec3 location;
};



class Wall final : public Object //private:normalVector
{
    //friend io
    friend std::istream &operator>>(std::istream &, Wall &);
    friend std::ostream &operator<<(std::ostream &, const Wall &);

public:
    //construct
    Wall() : Wall(glm::vec3(0.0f),glm::vec3(0.0f, 0.0f, 1.0f)) {}
    Wall(const glm::vec3 &loc, const glm::vec3 &norv) : Object(loc), normalVector(norv), number(++sum) {}
    Wall(std::istream &);
    //copy move destruct
    Wall(const Wall &) = default;
    Wall(Wall &&) = default;
    Wall &operator=(const Wall &) = default;
    Wall &operator=(Wall &&) = default;
    ~Wall() = default;

    //information
    glm::vec3 norm() const{ return normalVector; }
    unsigned int cnt() const { return 0; }
    Object_type type() const { return Object_type::WALL; }
    unsigned int num() const { return number; }

private:
    glm::vec3 normalVector;
    static unsigned int sum;
    unsigned int number = 0;
};
std::istream &operator>>(std::istream &, Wall &);
std::ostream &operator<<(std::ostream &, const Wall &);
std::ostream &operator<<(std::ostream &, const std::vector<std::shared_ptr<Wall>> &);



class Ball final : public Object //velocity,mass
{
    //friend io
    friend std::istream &operator>>(std::istream &, Ball &);
    friend std::ostream &operator<<(std::ostream &, const Ball &);

public:
    //construct
    Ball() : Ball(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f) {}
    Ball(glm::vec3 loc, glm::vec3 vel, float m, float r) : Object(loc), velocity(vel), mass(m), radius(r) ,number(++sum){}
    Ball(std::istream &);
    //copy move destruct
    Ball(const Ball &) = default;
    Ball(Ball &&) = default;
    Ball &operator=(const Ball &) = default;
    Ball &operator=(Ball &&) = default;
    ~Ball() = default;

    //information
    float r() const { return radius; }
    glm::vec3 vel() const { return velocity; }
    float m() const { return mass; }
    float ek() const { return 0.5f * mass * square(glm::length(velocity)); }
    unsigned int cnt() const { return count; }
    Object_type type() const { return Object_type::BALL; }
    unsigned int num() const { return number; }

    //action
    void move(float t) { location += velocity * t; }
    void reverse() { velocity = -velocity; }

    //predict
    float predict(const Wall &);
    float predict(const Ball &);
    float predict(const Container &);

    //bounce
    void bounce(Object &);
    void bounce(Wall &);
    void bounce(Ball &);
    void bounce(Container &);

    //examine
    bool examine(const Wall &);
    bool examine(const Ball &);
    bool examine(const Container &);
    bool back(const Ball &);

private:
    glm::vec3 velocity;
    float mass, radius;
    static unsigned int sum; //extern
    unsigned int number = 0, count = 0;
};

std::istream &operator>>(std::istream &, Ball &);
std::ostream &operator<<(std::ostream &, const Ball &);
std::ostream &operator<<(std::ostream &, const std::vector<std::shared_ptr<Ball>> &);

//Container++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Container final : public Object//private:a,b,c
{
    //friend io
    friend std::istream& operator>>(std::istream&, Container&);
    friend std::ostream& operator<<(std::ostream&, const Container&);
public:
    //construct
    Container() :Container(glm::vec3(0.0f),glm::vec3(1.0f)) { }
    Container(glm::vec3 loc, glm::vec3 len) : Object(loc), length(len) ,number(++sum) { }
    Container(std::istream&);

    //copy move destruct
    Container(const Container&) = default;
    Container(Container&&) = default;
    Container& operator=(const Container&) = default;
    Container& operator=(Container&&) = default;
    ~Container() = default;

    //information
    unsigned int cnt() const { return 0; }
    Object_type type() const { return Object_type::CONTAINER; }
    unsigned int num() const { return number; }
    float x_p() const { return location.x + length.x / 2; }//positive
    float x_n() const { return location.x - length.x / 2; }//negative
    float y_p() const { return location.y + length.y / 2; }
    float y_n() const { return location.y - length.y / 2; }
    float z_p() const { return location.z + length.z / 2; }
    float z_n() const { return location.z - length.z / 2; }

    glm::vec3 scale() { return length; }

private:
    glm::vec3 length;
    static unsigned int sum; //extern
    unsigned int number = 0, count = 0;//默认为0
};

std::istream& operator>>(std::istream&, Container&);
std::ostream& operator<<(std::ostream&, const Container&);
std::ostream& operator<<(std::ostream&, const std::vector<std::shared_ptr<Container>>&);
#endif