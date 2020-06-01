#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ray.h"
#include "math.h"
#include "rayhit.h"
#include "bounds.h"
#include <vector>
#include <memory>

class Bounds;

using namespace std;

class Hittable {
    public:
        virtual bool Intersects(const Ray& r, double tmin, double tmax, RayHit& hit) const = 0;
        virtual bool GetBounds(Bounds &bounds) const = 0;
};

class HittableList : public Hittable {
    public:
        HittableList() {}
        virtual bool Intersects(const Ray& r, double tmin, double tmax, RayHit& hit) const;
        virtual bool GetBounds(Bounds &bounds) const;
        void Add(std::shared_ptr<Hittable> obj) {
            objects.push_back(obj);
        }
        void Clear() {
            objects.clear();
        }
        vector<shared_ptr<Hittable>>& Objects() { return objects;}

    private:
        vector<shared_ptr<Hittable>> objects;
};

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(const float3& center, float radius, shared_ptr<Material> mat) : c(center), r(radius), material(mat) {}
        virtual bool Intersects(const Ray& ray, double tmin, double tmax, RayHit& hit) const;
        virtual bool GetBounds(Bounds &bounds) const;
        virtual ~Sphere() {}
    private:
        float3 c;
        float r;
        shared_ptr<Material> material;
};


#endif