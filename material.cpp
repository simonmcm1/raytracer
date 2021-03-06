#include "material.h"
#include "random.h"

bool LambertMat::Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const {
    float3 scatterDir = rand.randomInUnitHemisphere(hit.normal);
    rOut = Ray(hit.point, scatterDir);
    attenuation = this->alb->Read(hit.uv.x(), hit.uv.y(), hit.point);
    return true;
}

bool MetalMat::Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const {
    float3 reflectDir = reflect(normalized(rIn.direction()), hit.normal);
    rOut = Ray(hit.point, reflectDir + this->rough * rand.randomInUnitSphere());
    attenuation = this->alb->Read(hit.uv.x(), hit.uv.y(), hit.point);
    return (dot(rOut.direction(), hit.normal) > 0);
}

bool DielectricMat::Scatter(const Ray& rIn, Random &rand, const RayHit& hit, float3& attenuation, Ray& rOut) const {
    attenuation = color(1.0, 1.0, 1.0);
    double eta;
    if (hit.frontFace) {
        eta = 1.0 / refraction;
    } else {
        eta = refraction;
    }

    float3 inDir = normalized(rIn.direction());
    float cost = fmin(dot(-inDir, hit.normal), 1.0);
    float sint = sqrt(1.0 - cost*cost);
    if (eta * sint > 1.0 ) {
        float3 reflected = reflect(inDir, hit.normal);
        rOut = Ray(hit.point, reflected);
        return true;
    }

    float reflectProbability = schlick(cost, eta);
    if(rand.randomf() < reflectProbability) {
        float3 reflected = reflect(inDir, hit.normal);
        rOut = Ray(hit.point, reflected);
        return true;
    }

    float3 refracted = refract(inDir, hit.normal, eta);
    rOut = Ray(hit.point, refracted);
    return true;
}
