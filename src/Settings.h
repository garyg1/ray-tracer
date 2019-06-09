#ifndef SETTINGS_H_
#define SETTINGS_H_

#define TREE_STATS true

#define W 800
#define H 600

#define SHAPE_CONST 3

inline double min(double a, double b) {
    return a<b? a : b;
}

#define xaxis 0
#define yaxis 1
#define zaxis 2

#define USE_SPATIAL_SPLIT 0
#define USE_MEDIAN_SPLIT 0
#define USE_MEAN_SPLIT 1
#define USE_SAH_SPLIT 0

#define DEPTH_MAX 24


#endif
