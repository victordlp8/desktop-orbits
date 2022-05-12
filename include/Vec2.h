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
#ifndef __VEC2_INCLUDED__
#define __VEC2_INCLUDED__

#include <iostream>
#include <cmath>
#include "polarCoords2.h"

long double Vec2_PI = atan(1) * 4;

// Vec2 allows public access to its two data members: x and y.
class Vec2
{
public:
    long double x, y;

    inline Vec2()
    {
        x = 0;
        y = 0;
    }

    inline Vec2(long double xN, long double yN)
    {
        x = xN;
        y = yN;
    }

    // Squared Euclidean length.
    inline long double lengthSquared(const Vec2 &other) const
    {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }

    // The Euclidean length.
    inline long double length(const Vec2 &other) const
    {
        return sqrt(lengthSquared(other));
    }

    inline long double module() const
    {
        return sqrt(x * x + y * y);
    }

    inline long double angle() const
    {
        if (x == 0 && y == 0)
        {
            return 0;
        }
        else if (x > 0 && y == 0)
        {
            return 0;
        }
        else if (x < 0 && y == 0)
        {
            return Vec2_PI;
        }
        else if (x == 0 && y > 0)
        {
            return Vec2_PI / 2;
        }
        else if (x == 0 && y < 0)
        {
            return 3 * Vec2_PI / 2;
        }
        else if (x > 0 && y > 0)
        {
            return atan2(y, x);
        }
    }

    inline long double angle(const Vec2 &other) const
    {
        Vec2 temp(other.x - x, other.y - y);
        return temp.angle();
    }

    inline Vec2 operator=(const Vec2 &other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    // Vector addition.
    inline Vec2 operator+(const Vec2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Vector subtraction.
    inline Vec2 operator-(const Vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Unary minus.
    inline Vec2 operator-()
    {
        return Vec2(-x, -y);
    }

    // Scalar multiplication.
    inline Vec2 operator*(long double n)
    {
        return Vec2(x * n, y * n);
    }

    // Division by a scalar.
    inline Vec2 operator/(long double n)
    {
        return Vec2(x / n, y / n);
    }

    // // Compute the cross product of the 3D vectors formed
    // // from other and B by appending a zero Z coordinate.  Return
    // // only the Z coord of the result (as both X and Y will
    // // be zero anyway).
    // double operator^(const Vec2 &other, const Vec2 &B)
    // {
    //     return other.x * B.y - other.y * B.x;
    // }

    // Increment one vector by another.
    inline Vec2 &operator+=(const Vec2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Decrement one vector by another.
    inline Vec2 &operator-=(const Vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // // Returns normalized version of vector.
    // Vec2 Unit(const Vec2 &other)
    // {
    //     double d = lengthSquared(other);
    //     return d > 0.0 ? other / sqrt(d) : Vec2(0, 0);
    // }

    // // Send vector to an output stream.
    // std::ostream &operator<<(std::ostream &out, const Vec2 &other)
    // {
    //     out << "(" << other.x << ", " << other.y << ") ";
    //     return out;
    // }

    friend std::ostream &operator<<(std::ostream &out, const Vec2 &other);
};

std::ostream &operator<<(std::ostream &out, const Vec2 &other)
{
    out << "(" << other.x << ", " << other.y << ") ";
    return out;
}

#endif
