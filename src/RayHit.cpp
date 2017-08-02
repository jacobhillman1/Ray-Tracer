#include "RayHit.h"

RayHit::RayHit(Ray<double, 3> r) {
    this->r = r;
}

void RayHit::setIfHit(bool hit) {
    this->isAHit = hit;
}

/* Probably won't need this */
RayHit::RayHit(Vec3d hitPoint) {
    this->hitPoint = hitPoint;
}

void RayHit::setHitPoint(Vec3d hitPoint) {
    this->hitPoint = hitPoint;
}

Ray<double, 3> RayHit::getRay() {
    return r;
}

Vec3d RayHit::getRayOrigin() {
    return r.origin;
}

Vec3d RayHit::getRayDirection() {
    return r.direction;
}

void RayHit::generateNewDirection() {

}

void RayHit::setSurfaceNormal(Vec3d surfaceNormal) {
    this->surfaceNormal = surfaceNormal;
}
