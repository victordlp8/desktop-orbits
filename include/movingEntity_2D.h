#include <iostream>
#include "json.hpp"
#include "polarForm_2D.h"
#include "vector_2D.h"

using namespace std;

class movingEntity_2D
{
public:
    polarForm_2D pos;
    polarForm_2D vel;

    long double mass; // mass of entity in KB

    movingEntity_2D()
    {
        pos = polarForm_2D(0, 0);
        vel = polarForm_2D(0, 0);
        mass = 0;
    }

    movingEntity_2D(polarForm_2D p)
    {
        pos = p;
        vel = polarForm_2D(0, 0);
        mass = 0;
    }

    movingEntity_2D(polarForm_2D p, polarForm_2D v, long double m)
    {
        pos = p;
        vel = v;
        mass = m;
    }

    movingEntity_2D(const movingEntity_2D &e)
    {
        pos = e.pos;
        vel = e.vel;
        mass = e.mass;
    }

    void setPos(const polarForm_2D &p)
    {
        pos = p;
    }

    void setVel(const polarForm_2D &v)
    {
        vel = v;
    }

    void pushAcceleration(polarForm_2D acceleration)
    {
        vel += acceleration;
    }

    void pushForce(polarForm_2D force)
    {
        vel += force / mass;
    }

    // long double bounceAngle(const movingEntity_2D &other) const
    // {
    //     long double aEntry = vel.angle();
    //     long double aBetween = pos.angle(other.pos);
    //     long double aExit = PI - aEntry + (PI / 2 - aBetween);

    //     return aExit;
    // }

    // void update(HWND hd, long double dt = 1)
    // {
    //     pos += vel * dt;
    //     ListView_SetItemPosition(hd, 1, (int)pos.x + vel.x + 1920, (int)pos.y + vel.y);
    // }
};