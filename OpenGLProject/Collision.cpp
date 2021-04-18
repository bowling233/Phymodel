#include "Collision.h"

void Event::handle()
{
    switch (type)
    {
    case event_type::FB:
    {
        FixedBall &obj_ref = dynamic_cast<FixedBall &>(object);
        ball.bounceOff(obj_ref);
        break;
    }
    case event_type::W:
    {
        Wall &obj_ref = dynamic_cast<Wall &>(object);
        ball.bounceOff(obj_ref);
        break;
    }
    case event_type::B:
    {
        Ball &obj_ref = dynamic_cast<Ball &>(object);
        ball.bounceOff(obj_ref);
        break;
    }
    default:
        break;
    }
    type = event_type::U;
}
