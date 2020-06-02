#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"
#include "ray.h"

class Camera {
    public:
        Camera(float fov, float aspectRatio, float3 position, float3 forward, float3 up, float aperture, float focusDistance) {
            auto theta = fov * DEG_TO_RAD;
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspectRatio * viewport_height;
            auto focal_length = 1.0;

            w = normalized(-forward);
            u = normalized(cross(up, w));
            v = cross(w, u);
            this->aperture = aperture;

            origin = position;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            bottomLeft = origin - horizontal/2 - vertical/2 - focusDistance * w;
        }

        Ray GetRay(Random &rand, float s, float t) const {
            float3 offsetAmount = rand.randomOnUnitDisk() * aperture/2;
            float3 offset = u*offsetAmount.x() + v*offsetAmount.y();
            return Ray(origin + offset, bottomLeft + s*horizontal + t*vertical - origin - offset);
        }

    private:
        float3 origin;
        float3 horizontal;
        float3 vertical;
        float3 bottomLeft;
        float3 u, v, w;
        float aperture;

};

#endif