# Ray Tracer w/ K-D Tree & BVH Implementation

Take a look at the [report](https://github.com/garyg1/ray-tracer/blob/master/report/report.pdf).

## Environment
This program was developed and tested on Ubuntu 16.04.

## Compiling
Run `make`. This should output an executable `raytracer`.

The different partitioning algorithms are done at compile-time. They can be adjusted in `src/Settings.h`:

```
#define USE_SPATIAL_SPLIT 0
#define USE_MEDIAN_SPLIT 0
#define USE_MEAN_SPLIT 1
#define USE_SAH_SPLIT 0
```

## Running
```
./raytracer [modelPath] [useColors] [structure]
```

#### Examples:
```
./raytracer data/galleon.obj false bvh
./raytracer data/porsche.obj true kdtree
./raytracer data/porsche.obj true bvh
```
