#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

//Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Util Functions
inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

//Common Headers
#include "Math/interval.h"
#include "ray.h"
#include "Math/vec3.h"
