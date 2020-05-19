#include "geometry.h"

bool HittableList::Intersects(const Ray& ray, double tmin, double tmax, RayHit& hit) const {
    RayHit tempHit;
    float closest = tmax;
    bool hitSomething = false;

    for(const auto& obj : objects) {
        if(obj->Intersects(ray, tmin, closest, tempHit)) {
            closest = tempHit.t;
            hitSomething = true;
            hit = tempHit;
        }
    }

    return hitSomething;
}

bool Sphere::Intersects(const Ray& ray, double tmin, double tmax, RayHit& hit) const {
    float3 oc = ray.origin() - this->c;
    auto a = ray.direction().squareMagnitude();
    auto halfB = dot(oc, ray.direction());
    auto c = oc.squareMagnitude() - r*r;
    auto discriminant = halfB*halfB - a*c;

    auto root = sqrt(discriminant);
    auto temp = (-halfB - root)/a;
    if (temp < tmax && temp > tmin) {
        hit.t = temp;
        hit.point = ray.at(hit.t);
        auto surfaceNormal = (hit.point - this->c) / r;
        hit.SetNormalAndFrontFace(ray, surfaceNormal);
        return true;
    }
    temp = (-halfB + root) / a;
    if (temp < tmax && temp > tmin) {
        hit.t = temp;
        hit.point = ray.at(hit.t);
        auto surfaceNormal = (hit.point - this->c) / r;
        hit.SetNormalAndFrontFace(ray, surfaceNormal);
        return true;
    }
    return false;
}