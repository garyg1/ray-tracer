#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <algorithm>

#include "Intersection.h"
#include "Ray.h"
#include "Vector.h"
#include "Shape.h"
#include "Intersectable.h"
#include "BoundingBox.h"
#include "ShapeContainer.h"
#include "Settings.h"

void sortShapesByAxis(std::vector<Shape*>& shapes, int axis);

#endif
