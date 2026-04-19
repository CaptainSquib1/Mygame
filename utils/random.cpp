#include "random.h"

#include "xoshiro256plus.h"
#include <random>
#include <numbers>

Xoshiro256Plus& get_rng() {
    thread_local Xoshiro256Plus generator = [] {
        std::random_device rd;
        uint64_t seed = (static_cast<uint64_t>(rd()) << 32) | rd();
        return Xoshiro256Plus(seed);
    }();
    return generator;
}
int randint(int min , int max ) {
    static std::random_device rd;
    static std::default_random_engine ran{rd()};
    return std::uniform_int_distribution<>{min, max}(ran);
}

float random_float() {
    return get_rng().random_float();
}

double random_double() {
    return get_rng().random_double();
}

Vec<float> random_vec() {
    uint64_t n = get_rng().next();
    float x = (n >> 40) * 0x1.0p-24f; // top 24 bits of upper float
    float y = ((n >> 16) & 0xFFFFFF) * 0x1.0p-24f; // top 24 bits of lower float
    return {x, y};
}

Vec<float> random_unit_vec() {
    float theta = 2.0f * std::numbers::pi_v<float> * random_float();
    return {std::cos(theta), std::sin(theta)};
}
