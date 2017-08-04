#include <iostream>
#include <geomc/linalg/Vec.h>
#include <geomc/linalg/AffineTransform.h>

#include "Image.h"
#include "RayHit.h"
#include "Sphere.h"

using namespace geom;
using namespace std;

bool sphere_intersection(Ray<double, 3> r, double sphereRadius, Vec3d center);
Vec3d calculate_surface_normal(Ray<double, 3> r, double sphereRadius, Vec3d center);
Vec3d calculate_surface_normal_on_sphere(Vec3d intersection, Ray<double, 3> r, double radius, Vec3d center);
Vec3d find_sphere_intersections(Ray<double, 3> r, double radius, Vec3d center);


// TODO: template a "shape" class
Vec3d traceWithObjects(Ray<double, 3> r, Sphere s1, Sphere s2, int depth) {
    RayHit hitObject = RayHit(r);
    s1.checkHit(hitObject);
    if(!hitObject.hitSomething()) {
        s2.checkHit(hitObject);
    }
    if(hitObject.hitSomething()) {
         return hitObject.getAlbedo() * hitObject.getDotProduct()
                * traceWithObjects(Ray<double, 3>(hitObject.getHitPoint(), hitObject.getNewDirection()), s1, s2, depth + 1);
    }
    else {
        return Vec3d(1, 1, 1);
    }
}

void render_image(Image<double,3>& img) {
    Sphere s1  = Sphere(Vec3d(0.0, 0.0, -4.0), 0.5, Vec3d(0.0, 0.0, 1.0));
    Sphere s2 = Sphere(Vec3d(1.0, 0.0, -4.0), 0.3, Vec3d(1.0, 0.0, 0.0));
    for (index_t y = 0; y < img.height; ++y) {
        for (index_t x = 0; x < img.width; ++x) {
            Vec2i xy = Vec2i(x,y);                // integer pixel coordinates
            Vec2d st = ((Vec2d)(xy)) / img.width; // floating point image coordinates
            st -= Vec2d(0.5, (0.5 * img.height) / img.width); // center on (0,0)

            Vec3d o = Vec3d(0.0,0.0,0.0);
            Vec3d d = Vec3d(st[0], st[1], -1.0);
            Ray<double,3> r = Ray<double,3>(o, d);

            Vec3d color = Vec3d(0.0, 0.0, 0.0);

            for (int i = 0; i < 50; i++) {
                color += traceWithObjects(r, s1, s2, 0);
            }

            color /= 50;

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
