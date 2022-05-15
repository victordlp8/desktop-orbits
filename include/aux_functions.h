#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include "json.hpp"
#include "polarForm_2D.h"
#include "vector_2D.h"

using json = nlohmann::json;
using namespace std;

/**
 * @brief Funtion that transforms a vector_2D object into a polarForm_2D object
 *
 * @param other
 * @return polarForm_2D - the polarForm_2D object that is the result of the transformation
 */
inline polarForm_2D toPolarCoords(const vector_2D &other)
{
    return polarForm_2D(other.module(), other.angle());
}

/**
 * @brief Function that transforms a polarForm_2D object into a vector_2D object
 *
 * @param other this is the polarForm_2D object to be transformed
 * @return vector_2D - the vector_2D object that is the result of the transformation
 */
inline vector_2D tovector_2D(const polarForm_2D &other)
{
    return vector_2D(other.r * cos(other.theta), other.r * sin(other.theta));
}

/**
 * @brief Loads a json file and returns a json object
 *
 * @param json_data in/out json object
 * @param filePath name of the file that contains the json data
 * @return true/false depending if the reading is successful
 */
bool loadJson(json &json_data, const char *filePath)
{
    ifstream json_file(filePath);
    if (json_file.is_open())
    {
        json_file >> json_data;
        return true;
    }
    else
    {
        return false;
    }
}

random_device rd;  // Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
/**
 * @brief A random number generator with long double precision and min-max capabilities
 *
 * @param minMax - vector_2D object that contains the min and max values of the random number
 * @return long double - the random number
 */
long double random(vector_2D minMax)
{
    uniform_real_distribution<long double> dis(minMax.x, minMax.y);
    gen.seed(time(NULL) + rand());
    return dis(gen);
}