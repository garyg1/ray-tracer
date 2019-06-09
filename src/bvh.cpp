#include "bvh.h"

#if TREE_STATS
extern int totalDepth, totalTris, leafCount;
#endif

BVH* BVH::ConstructBVH(std::vector<Shape*>& shapes) {
    BVHNode* root = new BVHNode();
    BVH::PopulateBVH(shapes, root, 0);
    return new BVH(root);
}

BVH::~BVH() {
    delete root;
}

void BVH::PopulateBVH(std::vector<Shape*>& shapes, BVHNode* parent, int depth) {
    // std::cout << depth << " " << shapes.size() << std::endl;
    
    switch (shapes.size()) {
    case 0:
        #if TREE_STATS
        totalDepth += depth;
        totalTris += shapes.size();
        leafCount++;
        #endif 

        parent->aabb = new BoundingBox(new Vector(0, 0, 0), new Vector(0, 0, 0));
        parent->containedShapes = new ShapeContainer(shapes);
        return;
    case 1:
        #if TREE_STATS
        totalDepth += depth;
        totalTris += shapes.size();
        leafCount++;
        #endif 

        parent->aabb = shapes[0]->getBoundingBox();
        parent->containedShapes = new ShapeContainer(shapes);
        return;
    }

    parent->aabb = shapes[0]->getBoundingBox();
    for (int i = 1; i < shapes.size(); i++) {
        BoundingBox* bb = shapes[i]->getBoundingBox();
        parent->aabb->expand(bb);
        delete bb;
    }

    if (shapes.size() < SHAPE_CONST) {
        #if TREE_STATS
        totalDepth += depth;
        totalTris += shapes.size();
        leafCount++;
        #endif 

        parent->containedShapes = new ShapeContainer(shapes);
        return;
    }

    int longestAxis = parent->aabb->getLongestAxis();

    double mid = 0.0;
    #if USE_MEAN_SPLIT
    for (int i = 0; i < shapes.size(); i++) {
        Vector m = shapes[i]->getMidpoint();
        if (longestAxis == xaxis) {
            mid += m.x;
        } else if (longestAxis == yaxis) {
            mid += m.y;
        } else {
            mid += m.z;
        }
    }

    mid /= shapes.size();

    #elif USE_MEDIAN_SPLIT
    sortShapesByAxis(shapes, longestAxis);
    Vector* m = shapes[shapes.size() / 2]->getMidpointPtr();
    if (longestAxis == xaxis) {
        mid += m->x;
    } else if (longestAxis == yaxis) {
        mid += m->y;
    } else {
        mid += m->z;
    }

    delete m;

    #elif USE_SPATIAL_SPLIT
    switch (longestAxis) {
    case xaxis:
        mid = (parent->aabb->p1->x + parent->aabb->p2->x)/2.0;
        break;
    case yaxis:
        mid = (parent->aabb->p1->y + parent->aabb->p2->y)/2.0;
        break;
    case zaxis:
        mid = (parent->aabb->p1->z + parent->aabb->p2->z)/2.0;
    }
    #endif

    
    std::vector<Shape*> mins;
    std::vector<Shape*> maxs;
    for (int i = 0; i < shapes.size(); i++) {
        switch (longestAxis) {
        case xaxis:
            if (shapes[i]->getMidpoint().x < mid) mins.push_back(shapes[i]);
            else maxs.push_back(shapes[i]);
            break;
        case yaxis:
            if (shapes[i]->getMidpoint().y < mid) mins.push_back(shapes[i]);
            else maxs.push_back(shapes[i]);
            break;
        case zaxis:
            if (shapes[i]->getMidpoint().z < mid) mins.push_back(shapes[i]);
            else maxs.push_back(shapes[i]);
            break;
        }
    }

    if (mins.size() == 0 || maxs.size() == 0) {
        #if TREE_STATS
        totalDepth += depth;
        totalTris += shapes.size();
        leafCount++;
        #endif 

        parent->containedShapes = new ShapeContainer(shapes);
        return;
    }

    parent->left = new BVHNode();
    parent->right = new BVHNode();
    
    BVH::PopulateBVH(mins, parent->left, depth + 1);
    BVH::PopulateBVH(maxs, parent->right, depth + 1);
}

Intersection* BVH::Intersect(Ray& r, BVHNode* curr, int d) {
    if (curr == nullptr) return nullptr;
    if (curr->aabb->intersect(r) == nullptr) {
        return nullptr;
    }
    if (curr->containedShapes != nullptr) {
        return curr->containedShapes->intersect(r);
    }
    
    Intersection* left = BVH::Intersect(r, curr->left, d + 1);
    Intersection* right = BVH::Intersect(r, curr->right, d + 1);
    
    if (left == nullptr && right == nullptr) return nullptr;
    if (left == nullptr) return right;
    if (right == nullptr) return left;

    Intersection* min = Intersection::Min(left, right);

    if (min == left) {
        delete left;
        return right;
    } else {
        delete right;
        return left;
    }
}

Intersection* BVH::Intersect(Ray& r, BVHNode* curr, Shape* ignore) {
    return nullptr;
    if (curr == nullptr) return nullptr;
    if (curr->aabb->intersect(r) == nullptr) {
        return nullptr;
    }
    if (curr->containedShapes != nullptr) {
        return curr->containedShapes->intersect(r, ignore);
    }
    
    Intersection* left = BVH::Intersect(r, curr->left, ignore);
    Intersection* right = BVH::Intersect(r, curr->right, ignore);

    if (left == nullptr && right == nullptr) return nullptr;
    if (left == nullptr) return right;
    if (right == nullptr) return left;
    return left;
}

