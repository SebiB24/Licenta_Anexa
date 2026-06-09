//
// Created by sebyc on 04/03/2026.

// This is the general main header file


#ifndef IMAGE_FILE_GENERAL_H
#define IMAGE_FILE_GENERAL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>


// C++ Std Usings
using std::shared_ptr;
using std::make_shared;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions
inline double degrees_to_radians(double degrees){
    return degrees * pi / 180.0;
}

inline double random_double(){
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max-min) * random_double();
}

// Common Headers
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "interval.h"
#include "camera.h"
#include "material.h"
#include "quad.h"

#endif //IMAGE_FILE_GENERAL_H
