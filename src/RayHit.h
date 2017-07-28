#ifndef RAYHIT
#define RAYHIT

#include <geomc/linalg/Vec.h>

using namespace geom;

class RayHit {
private:
    Vec3d hitPoint;
    Vec3d newDirection;

public:
    RayHit(Vec3d);
    Vec3d getHitPoint();
    Vec3d getNewDirection();

};

#endif
