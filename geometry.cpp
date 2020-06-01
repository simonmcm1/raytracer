#include "geometry.h"
#include "rayhit.h"
#include "bounds.h"

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

bool HittableList::GetBounds(Bounds &bounds) const{
    if(objects.empty()) {
        return false;
    }
    Bounds b;
    bool hasBounds = objects.front()->GetBounds(b);

    for(auto obj : objects) {
        Bounds other;
        if(obj->GetBounds(other)) {
            if(hasBounds) {
                b.Encapsulate(other);
            } else {
                b = other;
                hasBounds = true;
            }
        }
    }
    bounds = b;
    return hasBounds;
    
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
        hit.material = this->material;
        return true;
    }
    temp = (-halfB + root) / a;
    if (temp < tmax && temp > tmin) {
        hit.t = temp;
        hit.point = ray.at(hit.t);
        auto surfaceNormal = (hit.point - this->c) / r;
        hit.SetNormalAndFrontFace(ray, surfaceNormal);
        hit.material = this->material;
        return true;
    }
    return false;
}

bool Sphere::GetBounds(Bounds &bounds) const {
    bounds = Bounds(c - float3(r,r,r), c + float3(r,r,r));
    return true;
}