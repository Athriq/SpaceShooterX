#pragma once

#include <random>

namespace RandomUtils
{
    // Uniform Mersenne-Twister random number generator
    static double random(double range_from, double range_to) {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_real_distribution<> distr(range_from, range_to);
        return distr(generator);
    }
}