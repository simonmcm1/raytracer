#ifndef RAY_H
#define RAY_H

#include "math.h"
#include "bounds.h"

class Ray {

    public:
        Ray() {}
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

        double intersectSphere(const float3 &center, float radius) const {
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

        inline bool IntersectsBounds(const Bounds& bounds, double tMin, double tMax) const {
            for (int a = 0; a < 3; a++) {
                auto invD = 1.0f / direction()[a];
                auto t0 = (bounds.min()[a] - origin()[a]) * invD;
                auto t1 = (bounds.max()[a] - origin()[a]) * invD;
                if (invD < 0.0f)
                    std::swap(t0, t1);
                tMin = t0 > tMin ? t0 : tMin;
                tMax = t1 < tMax ? t1 : tMax;
                if (tMax <= tMin)
                    return false;
            }
            return true;
        }
    private:
        float3 orig;
        float3 dir;

};

#endif