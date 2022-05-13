/***************************************************************************
 * Vec2.h                                                                   *
 *                                                                          *
 * Vec2 is a trivial encapsulation of 2D floating-point coordinates.        *
 * It has all of the obvious operators defined as functions.         *
 *                                                                          *
 * History:                                                                 *
 *   10/04/2009  Updated for use in CS112.                                  *
 *   04/01/2003  Initial coding.                                            *
 *                                                                          *
 ***************************************************************************/
#ifndef __POLARCOORDS2_INCLUDED__
#define __POLARCOORDS2_INCLUDED__

#include <iostream>
#include <string>
#include <cmath>
#include "Vec2.h"

long double polarCoords2_PI = atan(1) * 4;

class polarCoords2
{
public:
    long double r, theta;

    void correct()
    {
        if (r < 0)
        {
            r *= -1;
            theta = theta + polarCoords2_PI;
        }

        if (theta < 0 || 2 * polarCoords2_PI < theta)
        {
            int nTurns = (int)(theta / (2 * polarCoords2_PI));
            theta = theta - nTurns * 2 * polarCoords2_PI;

            if (theta < 0)
            {
                theta += 2 * polarCoords2_PI;
            }
        }
    }

    inline polarCoords2()
    {
        r = 0;
        theta = 0;
        correct();
    }

    inline polarCoords2(long double rN, long double thetaN)
    {
        r = rN;
        theta = thetaN;
        correct();
    }

    inline polarCoords2(const polarCoords2 &other)
    {
        r = other.r;
        theta = other.theta;
        correct();
    }

    inline polarCoords2 operator=(const polarCoords2 &other)
    {
        r = other.r;
        theta = other.theta;
        correct();
        return *this;
    }

    inline polarCoords2 operator-()
    {
        theta = theta + polarCoords2_PI;
        correct();
        return *this;
    }

    inline polarCoords2 operator+(const polarCoords2 &other) const
    {
        polarCoords2 result(*this);
        result.r = sqrt(pow(r, 2) + 2 * r * other.r * cos(other.theta - theta) + pow(other.r, 2));
        result.theta = theta + atan2((other.r * sin(other.theta - theta)), (r + other.r * cos(other.theta - theta)));
        result.correct();
        return result;
    }

    inline polarCoords2 operator-(const polarCoords2 &other) const
    {
        return operator+(other * -1);
    }

    inline polarCoords2 operator*(long double scalar) const
    {
        return polarCoords2(r * scalar, theta);
    }

    inline polarCoords2 operator*(const polarCoords2 &other) const
    {
        return polarCoords2(r * other.r, theta + other.theta);
    }

    inline polarCoords2 operator/(long double scalar) const
    {
        return polarCoords2(r / scalar, theta);
    }

    inline polarCoords2 operator/(const polarCoords2 &other) const
    {
        return polarCoords2(r / other.r, theta - other.theta);
    }

    inline polarCoords2 operator+=(const polarCoords2 &other)
    {
        *this = *this + other;
        return *this;
    }

    inline polarCoords2 operator-=(const polarCoords2 &other)
    {
        *this = *this - other;
        return *this;
    }

    inline polarCoords2 operator*=(long double scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    inline polarCoords2 operator*=(const polarCoords2 &other)
    {
        *this = *this * other;
        return *this;
    }

    inline polarCoords2 operator/=(long double scalar)
    {
        *this = *this / scalar;
        return *this;
    }

    inline polarCoords2 operator/=(const polarCoords2 &other)
    {
        *this = *this / other;
        return *this;
    }

    inline bool operator==(const polarCoords2 &other) const
    {
        return (r == other.r) && (theta == other.theta);
    }

    inline bool operator!=(const polarCoords2 &other) const
    {
        return (r != other.r) || (theta != other.theta);
    }

    inline bool operator<(const polarCoords2 &other) const
    {
        return (r < other.r) || ((r == other.r) && (theta < other.theta));
    }

    inline bool operator>(const polarCoords2 &other) const
    {
        return (r > other.r) || ((r == other.r) && (theta > other.theta));
    }

    inline bool operator<=(const polarCoords2 &other) const
    {
        return (r <= other.r) || ((r == other.r) && (theta <= other.theta));
    }

    inline bool operator>=(const polarCoords2 &other) const
    {
        return (r >= other.r) || ((r == other.r) && (theta >= other.theta));
    }

    friend std::ostream &operator<<(std::ostream &out, const polarCoords2 &other);
};

std::ostream &operator<<(std::ostream &out, const polarCoords2 &other)
{
    out << "(" << other.r << "|" << other.theta * 180 / polarCoords2_PI << " o)";
    return out;
}

#endif
