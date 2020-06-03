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

void GetSphereUV(const float3& p, float& u, float& v) {
    auto phi = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u = 1-(phi + PI) / (2*PI);
    v = (theta + PI/2) / PI;
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
        float u,v;
        GetSphereUV(hit.point, u, v);
        hit.uv= float3(u, v, 0);
        return true;
    }
    temp = (-halfB + root) / a;
    if (temp < tmax && temp > tmin) {
        hit.t = temp;
        hit.point = ray.at(hit.t);
        auto surfaceNormal = (hit.point - this->c) / r;
        hit.SetNormalAndFrontFace(ray, surfaceNormal);
        hit.material = this->material;
        float u,v;
        GetSphereUV(hit.point, u, v);
        hit.uv= float3(u, v, 0);
        return true;
    }
    return false;
}

bool Sphere::GetBounds(Bounds &bounds) const {
    bounds = Bounds(c - float3(r,r,r), c + float3(r,r,r));
    return true;
}


bool XYRect::Intersects(const Ray& ray, double tmin, double tmax, RayHit& hit) const {
    auto t = (k-ray.origin().z()) / ray.direction().z();

    if (t < tmin || t > tmax) {
        return false;
    }

    auto x = ray.origin().x() + t*ray.direction().x();
    auto y = ray.origin().y() + t*ray.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }

    float u = (x-x0)/(x1-x0);
    float v = (y-y0)/(y1-y0);
    hit.uv = float3(u,v, 0);
    hit.t = t;
    auto normal = vec3(0, 0, 1);
    hit.SetNormalAndFrontFace(ray, normal);
    hit.material = material;
    hit.point = ray.at(t);
    return true;
}

bool XYRect::GetBounds(Bounds &bounds) const {
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount.
    bounds = Bounds(float3(x0,y0, k-0.0001), float3(x1, y1, k+0.0001));
    return true;
}


