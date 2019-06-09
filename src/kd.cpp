#include "kd.h"


#if TREE_STATS
extern int totalDepth, totalTris, leafCount;
#endif 
KD* KD::ConstructKD(std::vector<Shape*>& shapes) {
    KDNode* root = new KDNode();
    
    root->aabb = shapes[0]->getBoundingBox();
    for (int i = 1; i < shapes.size(); i++) {
        BoundingBox* bb = shapes[i]->getBoundingBox();
        root->aabb->expand(bb);
        delete bb;
    }

    #if USE_SAH_SPLIT
    std::cout << "Using Surface Area Heuristic to split k-d tree." << std::endl
        << "This will take a few minutes." << std::endl;
    #endif
    KD::PopulateKD(shapes, root, 0);
    return new KD(root);
}

KD::~KD() {
    delete root;
}

void KD::PopulateKD(std::vector<Shape*>& shapes, KDNode* parent, int depth) {
    return KD::PopulateKD(shapes, parent, depth, -1);
}

double getSAH(std::vector<Shape*>& shapes, int i, BoundingBox* aabb, int a, int useMin) {
    double baseArea = 0.0;
    double minLen, maxLen;
    BoundingBox* shapeAABB = shapes[i]->getBoundingBox();
    double l1, l2;
    switch(a) {
    case xaxis:
        baseArea = (aabb->p2->y - aabb->p1->y) * (aabb->p2->z - aabb->p1->z);
        if (useMin) {
            minLen = shapeAABB->p1->x - aabb->p1->x;
            maxLen = aabb->p2->x - shapeAABB->p1->x;
        } else {
            minLen = shapeAABB->p2->x - aabb->p1->x;
            maxLen = aabb->p2->x - shapeAABB->p2->x;
        }

        l1 = (aabb->p2->y - aabb->p1->y);
        l2 = (aabb->p2->z - aabb->p1->z);
        break;
    case yaxis:
        baseArea = (aabb->p2->x - aabb->p1->x) * (aabb->p2->z - aabb->p1->z);
        if (useMin) {
            minLen = shapeAABB->p1->y - aabb->p1->y;
            maxLen = aabb->p2->y - shapeAABB->p1->y;
        } else {
            minLen = shapeAABB->p2->y - aabb->p1->y;
            maxLen = aabb->p2->y - shapeAABB->p2->y;
        }

        l1 = (aabb->p2->x - aabb->p1->x);
        l2 = (aabb->p2->z - aabb->p1->z);
        break;
    case zaxis:
        baseArea = (aabb->p2->y - aabb->p1->y) * (aabb->p2->x - aabb->p1->x);
        if (useMin) {
            minLen = shapeAABB->p1->z - aabb->p1->z;
            maxLen = aabb->p2->z - shapeAABB->p1->z;
        } else {
            minLen = shapeAABB->p2->z - aabb->p1->z;
            maxLen = aabb->p2->z - shapeAABB->p2->z;
        }

        l1 = (aabb->p2->x - aabb->p1->x);
        l2 = (aabb->p2->y - aabb->p1->y);
        break;
    }

    double mid = 0.0;
    switch(a) {
    case xaxis:
        if (useMin) mid = shapeAABB->p1->x;
        else mid = shapeAABB->p2->x;
        break;
    case yaxis:
        if (useMin) mid = shapeAABB->p1->y;
        else mid = shapeAABB->p2->y;
        break;
    case zaxis:
        if (useMin) mid = shapeAABB->p1->z;
        else mid = shapeAABB->p2->z;
        break;
    }

    int nl = 0, nr = 0;
    for (int i = 0; i < shapes.size(); i++) {
        BoundingBox* bb = shapes[i]->getBoundingBox();
        switch (a) {
        case xaxis:
            if (bb->p1->x < mid) nl++;
            if (bb->p2->x >= mid) nr++;
            break;
        case yaxis:
            if (bb->p1->y < mid) nl++;
            if (bb->p2->y >= mid) nr++;
            break;
        case zaxis:
            if (bb->p1->z < mid) nl++;
            if (bb->p2->z >= mid) nr++;
            break;
        }

        delete bb;
    }

    delete shapeAABB;
    return 2 * baseArea * (nl + nr) + 2 * nl * minLen * (l1 + l2) + 2 * nr * maxLen * (l1 + l2);
}

void KD::PopulateKD(std::vector<Shape*>& shapes, KDNode* parent, int depth, int axis) {
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

    // if out of axes or too few shapes or too deep, just give up
    if (axis > 2 || shapes.size() < SHAPE_CONST || depth >= DEPTH_MAX) {
        #if TREE_STATS
        totalDepth += depth;
        totalTris += shapes.size();
        leafCount++;
        #endif 

        parent->containedShapes = new ShapeContainer(shapes);
        return;
    }

    int longestAxis = axis;
    if (axis == -1) {
        longestAxis = parent->aabb->getLongestAxis();
    }
    
    double mid = 0.0;
    #if USE_MEAN_SPLIT
    for (int i = 0; i < shapes.size(); i++) {
        Vector* m = shapes[i]->getMidpointPtr();
        if (longestAxis == xaxis) {
            mid += m->x;
        } else if (longestAxis == yaxis) {
            mid += m->y;
        } else {
            mid += m->z;
        }

        delete m;
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

    #elif USE_SAH_SPLIT
    double minSAH = __DBL_MAX__;
    BoundingBox* midpoint = nullptr;
    int doUseMin = 0;
    for (int a = 0; a < 3; a++) {
        if (axis != -1) a = axis;
        for (int i = 0; i < shapes.size(); i++) {
            std::cout << "Checking " << i + a*shapes.size() << "/" << 3*shapes.size() 
                << " possible splits                           \r" << std::flush;
            for (int useMin = 0; useMin < 2; useMin++) {
                double sah = getSAH(shapes, i, parent->aabb, a, useMin);
                if (sah < minSAH) {
                    longestAxis = a;
                    delete midpoint;
                    midpoint = shapes[i]->getBoundingBox();
                    doUseMin = useMin;
                    minSAH = sah;
                }
            }
        }

        if (axis != -1) break;
    }

    std::cout << "Checked " << shapes.size()*3 << " possible splits                                " << std::endl;
    std::cout << "Min SAH: " << minSAH << std::endl;

    switch(longestAxis) {
    case xaxis:
        if (doUseMin) mid = midpoint->p1->x;
        else mid = midpoint->p2->x;
        break;
    case yaxis:
        if (doUseMin) mid = midpoint->p1->y;
        else mid = midpoint->p2->y;
        break;
    case zaxis:
        if (doUseMin) mid = midpoint->p1->z;
        else mid = midpoint->p2->z;
        break;
    }

    delete midpoint;
    #endif
    
    std::vector<Shape*> mins;
    std::vector<Shape*> maxs;

    int duplicateCount = 0;
    for (int i = 0; i < shapes.size(); i++) {
        BoundingBox* bb = shapes[i]->getBoundingBox();
        switch (longestAxis) {
        case xaxis:
            if (bb->p1->x < mid && bb->p2->x >= mid) duplicateCount++;
            if (bb->p1->x < mid) mins.push_back(shapes[i]);
            if (bb->p2->x >= mid) maxs.push_back(shapes[i]);
            break;
        case yaxis:
            if (bb->p1->y < mid && bb->p2->y >= mid) duplicateCount++;
            if (bb->p1->y < mid) mins.push_back(shapes[i]);
            if (bb->p2->y >= mid) maxs.push_back(shapes[i]);
            break;
        case zaxis:
            if (bb->p1->z < mid && bb->p2->z >= mid) duplicateCount++;
            if (bb->p1->z < mid) mins.push_back(shapes[i]);
            if (bb->p2->z >= mid) maxs.push_back(shapes[i]);
            break;
        }

        delete bb;
    }

    // if no change, cycle through axes
    if (shapes.size() == mins.size()) {
        #if TREE_STATS
        totalDepth += depth;
        totalTris += shapes.size();
        leafCount++;
        #endif         
        
        KD::PopulateKD(mins, parent, depth + 1, axis + 1);
        return;
    }
    if (shapes.size() == maxs.size()) {
        #if TREE_STATS
        totalDepth += depth;
        totalTris += shapes.size();
        leafCount++;
        #endif 

        KD::PopulateKD(maxs, parent, depth + 1, axis + 1);
        return;
    }

    if (duplicateCount >= shapes.size()/2 || mins.size() == 0 || maxs.size() == 0) {
        parent->containedShapes = new ShapeContainer(shapes);
        return;
    }

    parent->left = new KDNode();
    parent->right = new KDNode();

    parent->left->aabb = parent->aabb->split(mid, false, longestAxis);
    parent->right->aabb = parent->aabb->split(mid, true, longestAxis);
    
    KD::PopulateKD(mins, parent->left, depth + 1, -1);
    KD::PopulateKD(maxs, parent->right, depth + 1, -1);
}


double intersectHelper(Ray& r, KDNode* curr) {
    if (curr == nullptr) return __DBL_MIN__;
    if (curr->aabb->intersect(r) == nullptr) {
        return __DBL_MIN__;
    }

    return curr->aabb->intersect(r)->ray_t;
}

Intersection* KD::Intersect(Ray& r, KDNode* curr, int d) {
    if (curr == nullptr) return nullptr;
    if (curr->aabb->intersect(r) == nullptr) {
        return nullptr;
    }
    if (curr->containedShapes != nullptr) {
        return curr->containedShapes->intersect(r);
    }

    
    Intersection* left = nullptr;
    Intersection *right = nullptr;
    if (intersectHelper(r, curr->left) < intersectHelper(r, curr->right)) {
        left = KD::Intersect(r, curr->left, d + 1);
        if (left == nullptr) {
            return KD::Intersect(r, curr->right, d + 1);
        } else {
            return left;
        }
    } else {
        right = KD::Intersect(r, curr->right, d + 1);
        if (right == nullptr) {
            return KD::Intersect(r, curr->left, d + 1);
        } else {
            return right;
        }
    }
}

Intersection* KD::Intersect(Ray& r, KDNode* curr, Shape* ignore) {
    return nullptr;
    if (curr == nullptr) return nullptr;
    if (curr->aabb->intersect(r) == nullptr) {
        return nullptr;
    }
    if (curr->containedShapes != nullptr) {
        return curr->containedShapes->intersect(r, ignore);
    }
    
    Intersection* left = nullptr;
    Intersection* right = nullptr;
    if (intersectHelper(r, curr->left) < intersectHelper(r, curr->right)) {
        left = KD::Intersect(r, curr->left, ignore);
        if (left == nullptr) {
            return KD::Intersect(r, curr->right, ignore);
        } else {
            return left;
        }
    } else {
        right = KD::Intersect(r, curr->right, ignore);
        if (right == nullptr) {
            return KD::Intersect(r, curr->left, ignore);
        } else {
            return right;
        }
    }
}

