#include "Sphere.h"

Sphere::Sphere(Vec3d center, float radius, Vec3d albedo) {
    this->center = center;
    this->radius = radius;
    this->albedo = albedo;
}

void Sphere::checkHit(RayHit &r) {
    // calculating the a, b, and c values to plug into the quadratic equation
    double a = pow(r.getRayDirection().x,2) + pow(r.getRayDirection().y,2) + pow(r.getRayDirection().z,2);

    double b = 2 * ((r.getRayOrigin().x - center.x)*(r.getRayDirection().x) + (r.getRayOrigin().y - center.y)*(r.getRayDirection().y)
        + (r.getRayOrigin().z - center.z)*(r.getRayDirection().z));

    double c = pow((r.getRayOrigin().x - center.x), 2) + pow((r.getRayOrigin().y - center.y), 2)
        + pow((r.getRayOrigin().z - center.z), 2) - pow(radius, 2);

    if (pow(b, 2) - 4*a*c < 0 || a == 0) {
            r.setIfHit(false);
    }
    else {
        double sPos = (-b + sqrt(pow(b, 2) - 4*a*c)) / 2*a;
        double sNeg = (-b - sqrt(pow(b, 2) - 4*a*c)) / 2*a;

        if(sPos < 0 && sNeg < 0) {
            r.setIfHit(false);
        }
        else {
            r.setIfHit(true);

            double s = 0.0;

            if (sPos < 0 && sNeg > 0)
                s = sNeg;
            else if (sNeg < 0 && sPos > 0)
                s = sPos;
            else
                s = fmin(sPos, sNeg);

            Vec3d hitPoint = r.getRay().atMultiple(s);
            // calculate the surface normal and pass it to the RayHit object
            r.setSurfaceNormal(calculateSurfaceNormal(hitPoint));

            // correct for floating point precision
            hitPoint += r.getSurfaceNormal() * 0.05;

            r.setHitPoint(hitPoint);
            r.setAlbedo(albedo); // give the RayHit object the color of the sphere
            //TODO: multi-colored objects?
        }
    }
}

Vec3d Sphere::calculateSurfaceNormal(Vec3d hitPoint) {
    Vec3d surfaceNormal = hitPoint - center;
    return surfaceNormal.unit();
}
