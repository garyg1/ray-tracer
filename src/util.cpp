#include "util.h"

#if TREE_STATS
int totalDepth, totalTris, leafCount;
#endif

void sortShapesByAxis(std::vector<Shape*>& shapes, int axis) {
    if (axis == xaxis)
    std::sort(shapes.begin(), shapes.end(), [](const Shape* shape1, const Shape* shape2) {
        BoundingBox *a, *b;
        a = shape1->getBoundingBox();
        b = shape2->getBoundingBox();
        bool lessThan;
        lessThan = (a->p1->x < b->p1->x);
        delete a;
        delete b;
        return lessThan;
    });

    else if (axis == yaxis)
    std::sort(shapes.begin(), shapes.end(), [](const Shape* shape1, const Shape* shape2) {
        BoundingBox *a, *b;
        a = shape1->getBoundingBox();
        b = shape2->getBoundingBox();
        bool lessThan;
        lessThan = (a->p1->y < b->p1->y);
        delete a;
        delete b;
        return lessThan;
    });

    else if (axis == zaxis)
    std::sort(shapes.begin(), shapes.end(), [](const Shape* shape1, const Shape* shape2) {
        BoundingBox *a, *b;
        a = shape1->getBoundingBox();
        b = shape2->getBoundingBox();
        bool lessThan;
        lessThan = (a->p1->z < b->p1->z);
        delete a;
        delete b;
        return lessThan;
    });
}
