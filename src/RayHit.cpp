#include "RayHit.h"

std::default_random_engine generator(10);

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

Vec3d RayHit::getHitPoint() {
    return hitPoint;
}

Vec3d RayHit::getSurfaceNormal() {
    return surfaceNormal;
}

Vec3d RayHit::getAlbedo() {
    return albedo;
}

Vec3d RayHit::getNewDirection() {
    generateNewDirection();
    return newDirection;
}

void RayHit::generateNewDirection() {
    std::normal_distribution<double> distribution(0.0, 1);

    double x = distribution(generator);
    double y = distribution(generator);
    double z = distribution(generator);

    newDirection = Vec3d(x, y, z);

    // normalize
    newDirection = newDirection.unit();

    newDirection = (newDirection + surfaceNormal).unit(); //TODO: add .unit()

}


void RayHit::setSurfaceNormal(Vec3d surfaceNormal) {
    this->surfaceNormal = surfaceNormal;
}

void RayHit::setAlbedo(Vec3d albedo) {
    this->albedo = albedo;
}

/* Return true if something has been hit */
bool RayHit::hitSomething() {
    return isAHit;
}

float RayHit::getDotProduct() {
    return 1.0; //TODO: follow up on why this is 1
}
