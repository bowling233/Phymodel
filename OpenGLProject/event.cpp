#include "Event.h"

void Event::handle()
{
    if(this->type)//wall
    {
        this->ball1.handleCollision(this->wall);
    }
    else//ball
    {
        this->ball1.handleCollision(this->ball2);
    }
}



