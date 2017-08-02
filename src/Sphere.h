#ifndef SPHERE
#define SPHERE

#include <geomc/linalg/Vec.h>
#include "RayHit.h"

using namespace geom;

class Sphere {
private:
    Vec3d center;
    float radius;
    Vec3d albedo; // the color of the sphere

    Vec3d calculateSurfaceNormal(Vec3d); // will only be called if there's a hit

public:
    Sphere(Vec3d, float, Vec3d); // constructor that includes albedo

    void checkHit(RayHit &r);

};

#endif
