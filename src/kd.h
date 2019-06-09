#ifndef KD_H_
#define KD_H_

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
#include <algorithm>


struct KDNode {
    KDNode* left;
    KDNode* right;
    BoundingBox* aabb;
    ShapeContainer* containedShapes;
};

class KD: public Intersectable {
    KDNode* root;

public:
    ~KD();
    static KD* ConstructKD(std::vector<Shape*>&);
    inline Intersection* intersect(Ray& r) { return KD::Intersect(r, root, 0); }
    inline Intersection* intersect(Ray& r, Shape* ignore) { return KD::Intersect(r, root, ignore); }

private:
    KD(KDNode* root) : root(root) {}
    static Intersection* Intersect(Ray&, KDNode*, int);
    static Intersection* Intersect(Ray&, KDNode*, Shape*);
    static void PopulateKD(std::vector<Shape*>&, KDNode*, int);
    static void PopulateKD(std::vector<Shape*>&, KDNode*, int, int);
};

#endif
