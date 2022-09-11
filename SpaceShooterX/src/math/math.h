#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <random>

class Math
{
public:
    static float ConvertToRadians(float p_degrees)
    {
        return p_degrees * (float)(M_PI / 180.0f);
    }

    static float Randf()
    {
        return (float)rand() / (float)RAND_MAX;
    }
};