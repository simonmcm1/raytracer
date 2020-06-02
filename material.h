#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "rayhit.h"
#include "math.h"
#include "random.h"

struct RayHit;

class Material {
    public:
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const = 0;
};

class LambertMat : public Material {
    public:
        LambertMat() {}
        LambertMat(const float3& albedo) : alb(albedo) {}
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const;
        virtual ~LambertMat() {}
    public:
        float3 alb;
};

class MetalMat : public Material {
    public:
        MetalMat() {}
        MetalMat(const float3& albedo, float roughness) : alb(albedo), rough(saturate(roughness)) {}
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const;
        virtual ~MetalMat() {}
    public:
        float3 alb;
        float rough;
};

class DielectricMat : public Material {
    public:
        DielectricMat() {}
        DielectricMat(float refractionIndex) : refraction(refractionIndex) {}
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const;
        virtual ~DielectricMat() {}
    public:
        float refraction;
};

#endif