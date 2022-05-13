/***************************************************************************
 * movingEntity_2D.h                                                        *
 *                                                                          *
 * movingEntity_2D is a class that represents a moving entity in 2D space.  *
 *                                                                          *
 ***************************************************************************/
#ifndef __MOVINGENTITY_2D_INCLUDED__
#define __MOVINGENTITY_2D_INCLUDED__

#include <iostream>
#include "json.hpp"
#include "polarForm_2D.h"
#include "vector_2D.h"

using namespace std;

/**
 * @brief is a class that represents a moving entity in 2D space.
 *
 * @param pos is the position of the entity
 * @param vel is the velocity of the entity
 * @param mass is the mass of the entity
 */
class movingEntity_2D
{
public:
    polarForm_2D pos;
    polarForm_2D vel;

    long double mass;

    /**
     * @brief Construct a new movingEntity_2D object
     */
    movingEntity_2D()
    {
        pos = polarForm_2D(0, 0);
        vel = polarForm_2D(0, 0);
        mass = 0;
    }

    /**
     * @brief Construct a new movingEntity_2D object
     *
     * @param p is the position of the entity
     */
    movingEntity_2D(polarForm_2D p)
    {
        pos = p;
        vel = polarForm_2D(0, 0);
        mass = 0;
    }

    /**
     * @brief Construct a new movingEntity_2D object
     *
     * @param p is the position of the entity
     * @param v is the velocity of the entity
     * @param m is the mass of the entity
     */
    movingEntity_2D(polarForm_2D p, polarForm_2D v, long double m)
    {
        pos = p;
        vel = v;
        mass = m;
    }

    /**
     * @brief Copy constructor from another polarForm_2D object
     *
     * @param other The movingEntity_2D object to copy from
     */
    movingEntity_2D(const movingEntity_2D &e)
    {
        pos = e.pos;
        vel = e.vel;
        mass = e.mass;
    }

    /**
     * @brief Addes the acceleration to the current velocity
     *
     * @param acceleration The acceleration to add
     */
    void pushAcceleration(polarForm_2D acceleration)
    {
        vel += acceleration;
    }

    /**
     * @brief Applies a force to the entity
     *
     * @param force The force to apply
     */
    void pushForce(polarForm_2D force)
    {
        vel += force / mass;
    }
};

#endif
