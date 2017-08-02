#include <iostream>
#include <geomc/linalg/Vec.h>
#include <geomc/linalg/AffineTransform.h>
#include <random>

#include "Image.h"
#include "RayHit.h"
#include "Sphere.h"

using namespace geom;
using namespace std;

bool sphere_intersection(Ray<double, 3> r, double sphereRadius, Vec3d center);
Vec3d calculate_surface_normal(Ray<double, 3> r, double sphereRadius, Vec3d center);
Vec3d calculate_surface_normal_on_sphere(Vec3d intersection, Ray<double, 3> r, double radius, Vec3d center);
Vec3d find_sphere_intersections(Ray<double, 3> r, double radius, Vec3d center);

std::default_random_engine generator(10);

inline Vec3d find_random_direction(Vec3d surfaceNormal) {
    // random distribution with mean 0 and variance 1
    std::normal_distribution<double> distribution(0.0, 1);

    double x = distribution(generator);
    double y = distribution(generator);
    double z = distribution(generator);

    Vec3d randomDirection = Vec3d(x, y, z);

    // normalize
    randomDirection = randomDirection.unit();

    return (randomDirection + surfaceNormal).unit();

}

// Trace function, returns a 3-dimensional vector representing the color of the pixel
inline Vec3d trace(Ray<double, 3> r, int depth) {
    // initialize the color as black
    Vec3d color = Vec3d(0.0, 0.0, 0.0);
    Vec3d sphere = Vec3d(0.0, 0.0, -4.0);
    double radius = 0.5;

    /*
     * If the ray hits something, calculate the surfance normal, then choose
     * a random direction for a new ray to fire off. Repeat this process until
     * the ray hits a light source.
     * Right now, the ray is only checking for a sphere. In the future, it will
     * check for (tiny) triangles.
     */
    if(sphere_intersection(r, radius, sphere) && depth < 15) {
        Vec3d P = find_sphere_intersections(r, radius, sphere);
        Vec3d surfaceNormal = calculate_surface_normal_on_sphere(P, r, 0.5, Vec3d(0.0, 0.0, -4.0));

        // Correct for floating point precision issue
        // add normal * .05
        P += surfaceNormal  * 0.05;

        Vec3d albedo = Vec3d(.9);

        Vec3d L = find_random_direction(surfaceNormal);

        double dotproduct = 1; //= surfaceNormal.dot(L);

        color = albedo * dotproduct * trace(Ray<double, 3>(P, L), depth + 1);

    }
    else {
        if(r.direction.x > 0.3) {
            // if the ray doesn't hit a sphere, it hits the sky
            color = Vec3d(1, 1, 1);
        }
        else {
            color = Vec3d(0, 0, 0);
        }
    }

    return color;

}

// Returns true if a ray r intersects with a sphere with a certain radius at
// location center from the origin.
bool sphere_intersection(Ray<double, 3> r, double sphereRadius, Vec3d center) {

    // calculating the a, b, and c values to plug into the quadratic equation
    double a = pow(r.direction.x,2) + pow(r.direction.y,2) + pow(r.direction.z,2);

    double b = 2 * ((r.origin.x - center.x)*(r.direction.x) + (r.origin.y - center.y)*(r.direction.y)
        + (r.origin.z - center.z)*(r.direction.z));

    double c = pow((r.origin.x - center.x), 2) + pow((r.origin.y - center.y), 2)
        + pow((r.origin.z - center.z), 2) - pow(sphereRadius, 2);

    if (pow(b, 2) - 4*a*c < 0 || a == 0) {
            return false;
    }

    double sPos = (-b + sqrt(pow(b, 2) - 4*a*c)) / 2*a;
    double sNeg = (-b - sqrt(pow(b, 2) - 4*a*c)) / 2*a;

    if(sPos < 0 && sNeg < 0) {
        return false;
    }
    else {
        return true;
    }

}

// Returns a Vec3d object of the location of the closest intercept to a sphere
Vec3d find_sphere_intersections(Ray<double, 3> r, double radius, Vec3d center){

    // calculating the a, b, and c values to plug into the quadratic equation
    double a = pow(r.direction.x,2) + pow(r.direction.y,2) + pow(r.direction.z,2);

    double b = 2 * ((r.origin.x - center.x)*(r.direction.y) + (r.origin.y - center.y)*(r.direction.y)
        + (r.origin.z - center.z)*(r.direction.z));

    double c = pow((r.origin.x - center.x), 2) + pow((r.origin.y - center.y), 2)
        + pow((r.origin.z - center.z), 2) - pow(radius, 2);

    double sPos = (-b + sqrt(pow(b, 2) - 4*a*c)) / 2*a;
    double sNeg = (-b - sqrt(pow(b, 2) - 4*a*c)) / 2*a;

    double s = 0.0;

    if (sPos < 0 && sNeg > 0)
        s = sNeg;
    else if (sNeg < 0 && sPos > 0)
        s = sPos;
    else
        s = min(sPos, sNeg);

    Vec3d intersection = r.atMultiple(s);

    return intersection;

}

Vec3d calculate_surface_normal_on_sphere(Vec3d intersection, Ray<double, 3> r, double radius, Vec3d center) {
    Vec3d surfaceNormal = intersection - center;

    return surfaceNormal.unit();
}

void render_image(Image<double,3>& img) {
    for (index_t y = 0; y < img.height; ++y) {
        for (index_t x = 0; x < img.width; ++x) {
            Vec2i xy = Vec2i(x,y);                // integer pixel coordinates
            Vec2d st = ((Vec2d)(xy)) / img.width; // floating point image coordinates
            st -= Vec2d(0.5, (0.5 * img.height) / img.width); // center on (0,0)

            Vec3d o = Vec3d(0.0,0.0,0.0);
            Vec3d d = Vec3d(st[0], st[1], -1.0);
            Ray<double,3> r = Ray<double,3>(o, d);

            Vec3d color = Vec3d(0.0, 0.0, 0.0);

            for (int i = 0; i < 10; i++) {
                color += trace(r, 0);
            }

            color /= 10;

            img[xy] = color;
        }
    }
}

int main(int argc, char** argv) {
    Image<double,3> img(1024, 512); // a 3-channel image.

    render_image(img);

    img.save_png("output/test.png");

    return 0;
}

/*
 * TODO: make a sphere object that takes in the center and radius
 * you can pass that sphere object a rayhit object
 * if it doesn't hit the sphere, the rayhit object has a bool value that you
 * set to false. if it does it, you store the location of the hit, the normal,
 * and generate a new random direction.
trace {
    for(int i = 0; i < 10; i++) {
        Sphere s = Sphere(radius, center);
        s.checkHit(rayHitObj);
        if rayHitObj.hit() {
            trace(rayHitObj);
            return colorFormula;
        }
        else {
            return skyColor
        }
    }
}
*/
