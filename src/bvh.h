#ifndef BVH_H_
#define BVH_H_

#include "Intersection.h"
#include "Ray.h"
#include "Vector.h"
#include "Shape.h"
#include "Intersectable.h"
#include "BoundingBox.h"
#include "ShapeContainer.h"
#include "Settings.h"
#include "util.h"

#include <vector>


struct BVHNode {
    BVHNode* left;
    BVHNode* right;
    BoundingBox* aabb;
    ShapeContainer* containedShapes;
};

class BVH: public Intersectable {
    BVHNode* root;

public:
    ~BVH();
    static BVH* ConstructBVH(std::vector<Shape*>&);
    inline Intersection* intersect(Ray& r) { return BVH::Intersect(r, root, 0); }
    inline Intersection* intersect(Ray& r, Shape* ignore) { return BVH::Intersect(r, root, ignore); }

private:
    BVH(BVHNode* root) : root(root) {}
    static Intersection* Intersect(Ray&, BVHNode*, int);
    static Intersection* Intersect(Ray&, BVHNode*, Shape*);
    static void PopulateBVH(std::vector<Shape*>&, BVHNode*, int);
};

#endif
