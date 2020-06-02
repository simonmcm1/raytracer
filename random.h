#ifndef RANDOM_H
#define RANDOM_H

#include "math.h"

class Random {
    public:
        Random() : distribution(0.0, 1.0) {}

        float randomf() {
            return distribution(generator);
        }

        float randomf(float min, float max) {
            return min + (max-min)*randomf();
        }

        int randomi(int min, int max) {
            return round(min + (max-min)*randomf());
        }
        
        float3 randomVector() {
            return float3(randomf(), randomf(), randomf());
        }

        float3 randomVector(float min, float max) {
            return float3(randomf(min, max), randomf(min, max), randomf(min, max));
        }

        //TODO: use proper polar coord random
        float3 randomInUnitSphere() {
            while(true) {
                auto p = randomVector(-1, 1);
                if(p.squareMagnitude() >=1 ) {
                    continue;
                }
                return p;
            }
        }

        float3 randomOnUnitSphereSurface() {
            auto a = randomf(0, 2*PI);
            auto z = randomf(-1, 1);
            auto r = sqrt(1 - z*z);
            return float3(r*cos(a), r*sin(a), z);
        }

        float3 randomInUnitHemisphere(const float3& N) {
            float3 rand = randomInUnitSphere();
            if (dot(rand, N) > 0.0) 
                return rand;
            else
                return -rand;
        }

        float3 randomOnUnitDisk() {
            while (true) {
                auto p = float3(randomf(-1,1), randomf(-1,1), 0);
                if (p.squareMagnitude() >= 1) continue;
                return p;
            }
        }


    private:
        std::uniform_real_distribution<float> distribution;
        std::mt19937 generator;
};




#endif