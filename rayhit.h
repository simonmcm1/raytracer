#ifndef RAYHIT_H
#define RAYHIT_H

#include "math.h"
#include "ray.h"
#include "material.h"
#include <memory>

class Material;

struct RayHit {
    float3 point;
    //normal faces agains the incoming ray
    float3 normal;
    float t;
    bool frontFace;
    float3 uv;
    std::shared_ptr<Material> material;

    inline void SetNormalAndFrontFace(const Ray &ray, const float3 &surfaceNormal) {
        frontFace = dot(ray.direction(), surfaceNormal) < 0;
        normal = frontFace ? surfaceNormal : -surfaceNormal;
    }
};

#endif