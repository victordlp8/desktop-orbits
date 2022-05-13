/***************************************************************************
 * polarForm_2D.h                                                           *
 *                                                                          *
 * polarForm_2D is a trivial encapsulation of a segment and an angle.       *
 * It has all of the obvious operators defined as functions.                *
 *                                                                          *
 ***************************************************************************/
#ifndef __POLARFORM_2D_INCLUDED__
#define __POLARFORM_2D_INCLUDED__

#include <iostream>
#include <cmath>

long double polarForm_2D_PI = atan(1) * 4;

/**
 * @brief polarForm_2D is a trivial encapsulation of 2D floating-point coordinates.
 * 
 * @param r Is the length of the segment
 * @param theta Is the angle of the segment
 */
class polarForm_2D
{
public:
    long double r, theta;

    /**
     * @brief Gets rid of the minuses and ensures that the angle is between 0 and 2 PI
     */
    void correct()
    {
        if (r < 0)
        {
            r *= -1;
            theta = theta + polarForm_2D_PI;
        }

        if (theta < 0 || 2 * polarForm_2D_PI < theta)
        {
            int nTurns = (int)(theta / (2 * polarForm_2D_PI));
            theta = theta - nTurns * 2 * polarForm_2D_PI;

            if (theta < 0)
            {
                theta += 2 * polarForm_2D_PI;
            }
        }
    }

    /**
     * @brief Construct a new polarForm_2D object
     */
    inline polarForm_2D()
    {
        r = 0;
        theta = 0;
        correct();
    }

    /**
     * @brief Construct a new polarForm_2D object
     * 
     * @param r Is the length of the segment
     * @param theta Is the angle of the segment
     */
    inline polarForm_2D(long double rN, long double thetaN)
    {
        r = rN;
        theta = thetaN;
        correct();
    }

    /**
     * @brief Copy constructor from another polarForm_2D object
     * 
     * @param other The polarForm_2D object to copy from
     */
    inline polarForm_2D(const polarForm_2D &other)
    {
        r = other.r;
        theta = other.theta;
        correct();
    }

    inline polarForm_2D operator=(const polarForm_2D &other)
    {
        r = other.r;
        theta = other.theta;
        correct();
        return *this;
    }

    inline polarForm_2D operator-()
    {
        theta = theta + polarForm_2D_PI;
        correct();
        return *this;
    }

    inline polarForm_2D operator+(const polarForm_2D &other) const
    {
        polarForm_2D result(*this);
        result.r = sqrt(pow(r, 2) + 2 * r * other.r * cos(other.theta - theta) + pow(other.r, 2));
        result.theta = theta + atan2((other.r * sin(other.theta - theta)), (r + other.r * cos(other.theta - theta)));
        result.correct();
        return result;
    }

    inline polarForm_2D operator-(const polarForm_2D &other) const
    {
        return operator+(other * -1);
    }

    inline polarForm_2D operator*(long double scalar) const
    {
        return polarForm_2D(r * scalar, theta);
    }

    inline polarForm_2D operator*(const polarForm_2D &other) const
    {
        return polarForm_2D(r * other.r, theta + other.theta);
    }

    inline polarForm_2D operator/(long double scalar) const
    {
        return polarForm_2D(r / scalar, theta);
    }

    inline polarForm_2D operator/(const polarForm_2D &other) const
    {
        return polarForm_2D(r / other.r, theta - other.theta);
    }

    inline polarForm_2D operator+=(const polarForm_2D &other)
    {
        *this = *this + other;
        return *this;
    }

    inline polarForm_2D operator-=(const polarForm_2D &other)
    {
        *this = *this - other;
        return *this;
    }

    inline polarForm_2D operator*=(long double scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    inline polarForm_2D operator*=(const polarForm_2D &other)
    {
        *this = *this * other;
        return *this;
    }

    inline polarForm_2D operator/=(long double scalar)
    {
        *this = *this / scalar;
        return *this;
    }

    inline polarForm_2D operator/=(const polarForm_2D &other)
    {
        *this = *this / other;
        return *this;
    }

    inline bool operator==(const polarForm_2D &other) const
    {
        return (r == other.r) && (theta == other.theta);
    }

    inline bool operator!=(const polarForm_2D &other) const
    {
        return (r != other.r) || (theta != other.theta);
    }

    inline bool operator<(const polarForm_2D &other) const
    {
        return (r < other.r) || ((r == other.r) && (theta < other.theta));
    }

    inline bool operator>(const polarForm_2D &other) const
    {
        return (r > other.r) || ((r == other.r) && (theta > other.theta));
    }

    inline bool operator<=(const polarForm_2D &other) const
    {
        return (r <= other.r) || ((r == other.r) && (theta <= other.theta));
    }

    inline bool operator>=(const polarForm_2D &other) const
    {
        return (r >= other.r) || ((r == other.r) && (theta >= other.theta));
    }

    friend std::ostream &operator<<(std::ostream &out, const polarForm_2D &other);
};

std::ostream &operator<<(std::ostream &out, const polarForm_2D &other)
{
    out << "(" << other.r << "|" << other.theta * 180 / polarForm_2D_PI << " o)";
    return out;
}

#endif
