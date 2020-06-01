#ifndef BOUNDS_H
#define BOUNDS_H

#include "math.h"

class Bounds {
    public:
        Bounds() {}
        Bounds(const float3 &minPoint, const float3 &maxPoint) : _min(minPoint), _max(maxPoint) {}

        float3 max() const {return _max;}
        float3 min() const {return _min;}

        void Encapsulate(const Bounds& other) {
            float3 newMin(fmin(min().x(), other.min().x()),
                            fmin(min().y(), other.min().y()),
                            fmin(min().z(), other.min().z()));

            float3 newMax(fmax(max().x(), other.max().x()),
                            fmax(max().y(), other.max().y()),
                            fmax(max().z(), other.max().z()));
            _min = newMin;
            _max = newMax;
        }

    private:
        float3 _min;
        float3 _max;


};

#endif