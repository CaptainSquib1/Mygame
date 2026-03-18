#pragma once

#include <cstdint>

class Xoshiro256Plus {
private:
    uint64_t s[4];

    static inline uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

public:
    // seed using SplitMix64 to choose initial s[4]
    explicit Xoshiro256Plus(uint64_t seed) {
        uint64_t z = seed;
        for (int i = 0; i < 4; i++) {
            z += 0x9e3779b97f4a7c15;
            uint64_t tmp = z;
            tmp = (tmp ^ (tmp >> 30)) * 0xbf58476d1ce4e5b9;
            tmp = (tmp ^ (tmp >> 27)) * 0x94d049bb133111eb;
            s[i] = tmp ^ (tmp >> 31);
        }
    }

    uint64_t next() {
        const uint64_t result = s[0] + s[3];
        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }

    // double in [0, 1), uses top 53 bits
    double random_double() {
        return (next() >> 11) * 0x1.0p-53;
    }

    // float in [0, 1), uses top 24 bits
    float random_float() {
        return (next() >> 40) * 0x1.0p-24f;
    }
};
