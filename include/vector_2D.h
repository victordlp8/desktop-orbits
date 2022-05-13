/***************************************************************************
 * vector_2D.h                                                              *
 *                                                                          *
 * vector_2D is a trivial encapsulation of 2D floating-point coordinates.   *
 * It has all of the obvious operators defined as functions.                *
 *                                                                          *
 ***************************************************************************/
#ifndef __VECTOR_2D_INCLUDED__
#define __VECTOR_2D_INCLUDED__

#include <iostream>
#include <cmath>

long double vector_2D_PI = atan(1) * 4;

/**
 * @brief vector_2D is a trivial encapsulation of 2D floating-point coordinates.
 *
 * @param x Is the X coordinate of the vector
 * @param y Is the Y coordinate of the vector
 */
class vector_2D
{
public:
    long double x, y;

    /**
     * @brief Construct a new vector_2D object
     */
    inline vector_2D()
    {
        x = 0;
        y = 0;
    }

    /**
     * @brief Construct a new vector_2D object
     *
     * @param x Is the X coordinate of the vector
     * @param y Is the Y coordinate of the vector
     */
    inline vector_2D(long double x, long double y)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief Returns the module of the vector
     *
     * @return long double
     */
    inline long double module() const
    {
        return sqrt(x * x + y * y);
    }

    /**
     * @brief Returns the distance between two vectors
     *
     * @param other The second vector to compare with
     * @return long double
     */
    inline long double distance(const vector_2D &other) const
    {
        return (other - *this).module();
    }

    /**
     * @brief Returns the angle of the vector
     *
     * @return long double
     */
    inline long double angle() const
    {
        return atan2(y, x);
    }

    /**
     * @brief Returns the angle between two vectors
     *
     * @param other The second vector to compare with
     * @return long double
     */
    inline long double angle(const vector_2D &other) const
    {
        return (other - *this).angle();
    }

    inline vector_2D operator=(const vector_2D &other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    inline vector_2D operator+(const vector_2D &other) const
    {
        vector_2D result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    inline vector_2D operator-(const vector_2D &other) const
    {
        vector_2D result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    inline vector_2D operator-() const
    {
        vector_2D result;
        result.x = -x;
        result.y = -y;
        return result;
    }

    inline vector_2D operator*(long double scalar) const
    {
        vector_2D result;
        result.x = x * scalar;
        result.y = y * scalar;
        return result;
    }

    inline vector_2D operator/(long double scalar) const
    {
        vector_2D result;
        result.x = x / scalar;
        result.y = y / scalar;
        return result;
    }

    inline vector_2D &operator+=(const vector_2D &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline vector_2D &operator-=(const vector_2D &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const vector_2D &other);
};

std::ostream &operator<<(std::ostream &out, const vector_2D &other)
{
    out << "(" << other.x << ", " << other.y << ") ";
    return out;
}

#endif
