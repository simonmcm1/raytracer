#include "spatial.h"
#include <algorithm>

int randomAxis() {
    static std::mt19937 generator;
    static std::uniform_int_distribution<> dis(0, 2);
    return dis(generator);
}

BVHNode::BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) {
    int axis = randomAxis();
    auto comparator = (axis == 0) ? boxCompareX
                    : (axis == 1) ? boxCompareY
                                  : boxCompareZ;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
        left = make_shared<BVHNode>(objects, start, mid);
        right = make_shared<BVHNode>(objects, mid, end);
    }

    Bounds boundsLeft, boundsRight;

    if (  !left->GetBounds (boundsLeft)
       || !right->GetBounds(boundsRight)
    )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    bounds = Bounds(boundsLeft.min(), boundsLeft.max());
    bounds.Encapsulate(boundsRight);
}

bool BVHNode::Intersects(const Ray& r, double tmin, double tmax, RayHit& hit) const {
    if(!r.IntersectsBounds(bounds, tmin, tmax)) {
        return false;
    }

    bool leftHit = left->Intersects(r, tmin, tmax, hit) ;
    bool rightHit = right->Intersects(r, tmin, leftHit ? hit.t : tmax, hit);

    return leftHit || rightHit;
}