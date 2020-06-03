#ifndef TEXTURE_H
#define TEXTURE_H

#include "math.h"
#include <cmath>

class Texture {
    public:
        virtual float3 Read(float u, float v, const float3& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor() {}
        SolidColor(float3 c) : color(c) {}

        SolidColor(float r, float g, float b) : SolidColor(float3(r,g,b)) {}
        virtual ~SolidColor() {}
        virtual float3 Read(float u, float v, const float3& p) const {
            return color;
        }

    private:
        float3 color;
};

class WorldSpaceChecker : public Texture {
    public:
        WorldSpaceChecker(float3 c1, float3 c2, float scale) : color1(c1), color2(c2), worldSpaceScale(scale) {}
        virtual ~WorldSpaceChecker() {}
        virtual float3 Read(float u, float v, const float3 &p) const {
            float temp;
            float x = abs(modff(p.x() * worldSpaceScale, &temp));
            float y = abs(modff(p.y() * worldSpaceScale, &temp));
            float z = abs(modff(p.z() * worldSpaceScale, &temp));
            int check = 0;
            if(x > 0.5) check++;
            if(y > 0.5) check++;
            if(z > 0.5) check++;
            
            return check % 2 == 0 ? color1 : color2;
        }
    private:
        float3 color1;
        float3 color2;
        float worldSpaceScale;
};

#endif