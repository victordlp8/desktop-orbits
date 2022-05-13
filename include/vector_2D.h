/***************************************************************************
 * vector_2D.h                                                                   *
 *                                                                          *
 * vector_2D is a trivial encapsulation of 2D floating-point coordinates.        *
 * It has all of the obvious operators defined as functions.         *
 *                                                                          *
 * History:                                                                 *
 *   10/04/2009  Updated for use in CS112.                                  *
 *   04/01/2003  Initial coding.                                            *
 *                                                                          *
 ***************************************************************************/
#ifndef __VECTOR_2D_INCLUDED__
#define __VECTOR_2D_INCLUDED__

#include <iostream>
#include <cmath>
#include "polarForm_2D.h"

long double vector_2D_PI = atan(1) * 4;

// vector_2D allows public access to its two data members: x and y.
class vector_2D
{
public:
    long double x, y;

    inline vector_2D()
    {
        x = 0;
        y = 0;
    }

    inline vector_2D(long double xN, long double yN)
    {
        x = xN;
        y = yN;
    }

    // Squared Euclidean length.
    inline long double lengthSquared(const vector_2D &other) const
    {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }

    // The Euclidean length.
    inline long double length(const vector_2D &other) const
    {
        return sqrt(lengthSquared(other));
    }

    inline long double module() const
    {
        return sqrt(x * x + y * y);
    }

    inline long double angle() const
    {
        return atan2(y, x);
    }

    inline long double angle(const vector_2D &other) const
    {
        vector_2D temp(other.x - x, other.y - y);
        return temp.angle();
    }

    inline vector_2D operator=(const vector_2D &other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    // Vector addition.
    inline vector_2D operator+(const vector_2D &other) const
    {
        vector_2D result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    // Vector subtraction.
    inline vector_2D operator-(const vector_2D &other) const
    {
        vector_2D result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    // Unary minus.
    inline vector_2D operator-() const
    {
        vector_2D result;
        result.x = -x;
        result.y = -y;
        return result;
    }

    // Scalar multiplication.
    inline vector_2D operator*(long double scalar) const
    {
        vector_2D result;
        result.x = x * scalar;
        result.y = y * scalar;
        return result;
    }

    // Division by a scalar.
    inline vector_2D operator/(long double scalar) const
    {
        vector_2D result;
        result.x = x / scalar;
        result.y = y / scalar;
        return result;
    }

    // // Compute the cross product of the 3D vectors formed
    // // from other and B by appending a zero Z coordinate.  Return
    // // only the Z coord of the result (as both X and Y will
    // // be zero anyway).
    // double operator^(const vector_2D &other, const vector_2D &B)
    // {
    //     return other.x * B.y - other.y * B.x;
    // }

    // Increment one vector by another.
    inline vector_2D &operator+=(const vector_2D &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Decrement one vector by another.
    inline vector_2D &operator-=(const vector_2D &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // // Returns normalized version of vector.
    // vector_2D Unit(const vector_2D &other)
    // {
    //     double d = lengthSquared(other);
    //     return d > 0.0 ? other / sqrt(d) : vector_2D(0, 0);
    // }

    // // Send vector to an output stream.
    // std::ostream &operator<<(std::ostream &out, const vector_2D &other)
    // {
    //     out << "(" << other.x << ", " << other.y << ") ";
    //     return out;
    // }

    friend std::ostream &operator<<(std::ostream &out, const vector_2D &other);
};

std::ostream &operator<<(std::ostream &out, const vector_2D &other)
{
    out << "(" << other.x << ", " << other.y << ") ";
    return out;
}

#endif
