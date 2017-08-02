#ifndef RAYHIT
#define RAYHIT

#include <geomc/linalg/Vec.h>
#include <geomc/linalg/AffineTransform.h>
#include <random> //needed for normal distribution

using namespace geom;
using namespace std;

class RayHit {
private:
    Ray<double, 3> r;
    bool isAHit;
    Vec3d hitPoint;
    Vec3d newDirection;
    Vec3d surfaceNormal; //every hit will have a surface normal, but it will depend on what it's hitting
    Vec3d albedo; //store the albedo of whatever was hit

    void generateNewDirection();

public:
    RayHit(Vec3d);
    RayHit(Ray<double, 3>);

    void setHitPoint(Vec3d);
    void setIfHit(bool);
    void setNewDirection(); // TODO: actually use this
    void setSurfaceNormal(Vec3d);
    void setAlbedo(Vec3d);

    Vec3d getSurfaceNormal();
    Vec3d getHitPoint();
    Vec3d getNewDirection();
    Ray<double, 3> getRay();
    Vec3d getRayOrigin();
    Vec3d getRayDirection();
    Vec3d getAlbedo();
    float getDotProduct();
    bool hitSomething();

};

#endif
