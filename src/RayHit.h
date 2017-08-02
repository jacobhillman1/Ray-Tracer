#ifndef RAYHIT
#define RAYHIT

#include <geomc/linalg/Vec.h>
#include <geomc/linalg/AffineTransform.h>

using namespace geom;

class RayHit {
private:
    Ray<double, 3> r;
    bool isAHit;
    Vec3d hitPoint;
    Vec3d newDirection;
    Vec3d surfaceNormal; //every hit will have a surface normal, but it will depend on what it's hitting

public:
    RayHit(Vec3d);
    RayHit(Ray<double, 3>);
    void setHitPoint(Vec3d);
    void setIfHit(bool);
    void generateNewDirection();
    void setNewDirection(); // TODO: actually use this
    void setSurfaceNormal(Vec3d);
    Vec3d getHitPoint();
    Vec3d getNewDirection();
    Ray<double, 3> getRay();
    Vec3d getRayOrigin();
    Vec3d getRayDirection();

};

#endif
