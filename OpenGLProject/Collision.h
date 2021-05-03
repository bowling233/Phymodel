#ifndef COLLISION_H
#define COLLISION_H

#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <GLFW\glfw3.h>
constexpr auto DELTATIME = 0.01f;
extern int sumbounce;


class Object; 
class MovableObject;
class FixedBall; 
class Wall;
class Ball;

class CollisionSystem;


class CollisionSystem
{
    friend void display(GLFWwindow *window, double currentTime, CollisionSystem &system);
    friend std::istream &operator>>(std::istream &, CollisionSystem &);
    friend std::ostream &operator<<(std::ostream &, CollisionSystem &);

public:
    //construct and destruct
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    CollisionSystem(std::istream &is) : CollisionSystem() { is >> *this; }

    //action
    void run(float);
    void reverse();
    std::vector<std::shared_ptr<Ball>>& b() { return balls; }
    std::vector<std::shared_ptr<FixedBall>>& fb() { return fixedBalls; }
    std::vector<std::shared_ptr<Wall>>& w() { return walls; }

private:
    void init();
    void move(float);
    std::vector<std::shared_ptr<Ball>> balls;
    std::vector<std::shared_ptr<FixedBall>> fixedBalls;
    std::vector<std::shared_ptr<Wall>> walls;
    float currentTime = 0;
};
std::istream &operator>>(std::istream &, CollisionSystem &);
std::ostream &operator<<(std::ostream &, CollisionSystem &);

#endif