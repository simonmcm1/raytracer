#ifndef SPATIAL_H
#define SPATIAL_H

#include "math.h"
#include "geometry.h"

#include <memory>

class BVHNode : public Hittable {
    public:
        BVHNode() {}
        BVHNode(HittableList& contents) : BVHNode(contents.Objects(), 0, contents.Objects().size()) {}
        BVHNode(std::vector<shared_ptr<Hittable>> &objects, size_t start, size_t end);
        virtual ~BVHNode() {}

        virtual bool Intersects(const Ray& r, double tmin, double tmax, RayHit& hit) const;
        virtual bool GetBounds(Bounds &bounds) const {
            bounds = this->bounds;
            return true;
        }

    private:
        shared_ptr<Hittable> left;
        shared_ptr<Hittable> right;
        Bounds bounds;

};

inline bool boxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) {
    Bounds boundsA;
    Bounds boundsB;

    if (!a->GetBounds(boundsA) || !b->GetBounds(boundsB))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return boundsA.min().e[axis] < boundsB.min().e[axis];
}


inline bool boxCompareX(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return boxCompare(a, b, 0);
}

inline bool boxCompareY (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return boxCompare(a, b, 1);
}

inline bool boxCompareZ (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return boxCompare(a, b, 2);
}

#endif