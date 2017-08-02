#ifndef SPHERE
#define SPHERE

#include <geomc/linalg/Vec.h>
#include "RayHit.h"

using namespace geom;

class Sphere {
private:
    Vec3d center;
    float radius;
    Vec3d calculateSurfaceNormal(Vec3d); // will only be called if there's a hit

public:
    Sphere(Vec3d, float);
    void checkHit(RayHit);

};

#endif
