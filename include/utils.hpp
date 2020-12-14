#pragma once
#include <random>

const float EPSILON = 0.0001;

const int LAMBERTIAN = 0;
const int PERFECT_REFL = 1;
const int LIGHTSOURCE = 2;
const int TRANSPARENT = 3;
const int OREN_NAYAR = 4;


static std::default_random_engine GENERATOR;
static std::uniform_real_distribution<double> DISTR(0.0, 1.0);

static double uniformRand() {
    return DISTR(GENERATOR);
}

static double randMinMax(const double &min, const double &max) {
    return min + uniformRand() * (max - min);
}