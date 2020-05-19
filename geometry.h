#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ray.h"
#include "math.h"
#include <vector>
#include <memory>

using namespace std;

struct RayHit {
    float3 point;
    //normal faces agains the incoming ray
    float3 normal;
    float t;
    bool frontFace;

    inline void SetNormalAndFrontFace(const Ray &ray, const float3 &surfaceNormal) {
        frontFace = dot(ray.direction(), surfaceNormal) < 0;
        normal = frontFace ? surfaceNormal : -surfaceNormal;
    }
};

class Hittable {
    public:
        virtual bool Intersects(const Ray& r, double tmin, double tmax, RayHit& hit) const = 0;

};

class HittableList : public Hittable {
    public:
        HittableList() {}
        virtual bool Intersects(const Ray& r, double tmin, double tmax, RayHit& hit) const;
        void Add(std::shared_ptr<Hittable> obj) {
            objects.push_back(obj);
        }
        void Clear() {
            objects.clear();
        }

    private:
        vector<shared_ptr<Hittable>> objects;
};

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(const float3& center, float radius) : c(center), r(radius) {}
        virtual bool Intersects(const Ray& ray, double tmin, double tmax, RayHit& hit) const;
        virtual ~Sphere() {}
    private:
        float3 c;
        float r;
};


#endif