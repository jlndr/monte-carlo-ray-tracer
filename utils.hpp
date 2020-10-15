#include <random>
#pragma once

const float EPSILON = 0.01;

const int LAMBERTIAN = 0;
const int PERFECT_REFL = 1;
const int LIGHTSOURCE = 2;


static std::default_random_engine GENERATOR;
static std::uniform_real_distribution<double> DISTR(0.0, 1.0);

double uniformRand() {
    return DISTR(GENERATOR);
}

double randMinMax(const double &min, const double &max) {
    return min + uniformRand() * (max - min);
}