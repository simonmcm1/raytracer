#ifndef RAY_H
#define RAY_H

#include "math.h"

class Ray {

    public:
        Ray(const float3 &origin, const float3 &direction) : orig(origin), dir(direction) {

        }

        float3 origin() const {
            return orig;
        }

        float3 direction() const {
            return dir;
        }

        float3 at(float t) const {
            return orig + t*dir;
        }

        double intersectSphere(const float3 &center, float radius) {
            float3 oc = origin() - center;
            auto a = dir.squareMagnitude();
            auto halfB = dot(oc, direction());
            auto c = oc.squareMagnitude() - radius*radius;
            auto discriminant = halfB*halfB - a*c;

            if(discriminant < 0) {
                return -1;
            } else {
                return (-halfB - sqrt(discriminant) ) / a;
            }
        }
    private:
        float3 orig;
        float3 dir;

};

#endif