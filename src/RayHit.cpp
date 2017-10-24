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
    return newDirection;
}

void RayHit::generateNewDirection() {
    std::normal_distribution<double> distribution(0.0, 1);
    bool valid = false;

    while(!valid) {
        double x = distribution(generator);
        double y = distribution(generator);
        double z = distribution(generator);

        newDirection = Vec3d(x, y, z);

        // normalize (make magnitude = 1)
        newDirection = newDirection.unit();

        // make sure the new direction is not pointing into the object
        if(newDirection.dot(surfaceNormal) > 0) {
            valid = true;
        }
    }

    newDirection = (newDirection + surfaceNormal).unit();

}

void RayHit::setSurfaceNormal(Vec3d surfaceNormal) {
    this->surfaceNormal = surfaceNormal;
}

void RayHit::setAlbedo(Vec3d albedo) {
    this->albedo = albedo;
}

bool RayHit::hitSomething() {
    return isAHit;
}

// dot product between the surface normal and new direction
float RayHit::getDotProduct() {
    return newDirection.dot(surfaceNormal); //surfaceNormal.dot(newDirection)
}
