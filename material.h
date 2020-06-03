#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "rayhit.h"
#include "math.h"
#include "random.h"
#include "texture.h"

using namespace std;

struct RayHit;

class Material {
    public:
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const = 0;
        virtual float3 Emit(float u, float v, const float3 &p) const {
            return float3(0,0,0);
        }
};

class LambertMat : public Material {
    public:
        LambertMat() {}
        LambertMat(shared_ptr<Texture> albedo) : alb(albedo) {}
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const;
        virtual ~LambertMat() {}
    public:
        shared_ptr<Texture> alb;
};

class MetalMat : public Material {
    public:
        MetalMat() {}
        MetalMat(shared_ptr<Texture> albedo, float roughness) : alb(albedo), rough(saturate(roughness)) {}
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const;
        virtual ~MetalMat() {}
    public:
        shared_ptr<Texture> alb;
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

class EmissiveMat : public Material  {
    public:
        EmissiveMat(shared_ptr<Texture> emit) : emission(emit) {}
        virtual ~EmissiveMat() {}
        virtual bool Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const {
            return false;
        }

        virtual color Emit(float u, float v, const float3 &p) const {
            return emission->Read(u, v, p);
        }

    public:
        shared_ptr<Texture> emission;
};

#endif