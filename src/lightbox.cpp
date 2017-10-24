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


// Trace function accepts two spheres which will be represented in the output image.
// Function returns the color that a pixel should be after the ray fired through 
// it interacts with the spheres. 
Vec3d traceWithObjects(Ray<double, 3> r, Sphere s1, Sphere s2, int depth) {
    RayHit hitObject = RayHit(r);
    s1.checkHit(hitObject);
    if(!hitObject.hitSomething()) {
        s2.checkHit(hitObject);
    }

    if(hitObject.hitSomething()) {
        hitObject.generateNewDirection();
        return hitObject.getAlbedo() * hitObject.getDotProduct()
                * traceWithObjects(Ray<double, 3>(hitObject.getHitPoint(), hitObject.getNewDirection()), s1, s2, depth + 1);
    }
    else {
        // the color that the "sky" will be
        return Vec3d(1, 1, 1);
    }
}

// Most of the image rendering was provided with the boilerplate
void render_image(Image<double,3>& img) {
	// Instantiate the sphere objects
    Sphere s1  = Sphere(Vec3d(0, 0.0, -4.0), 0.5, Vec3d(1, 1, 1));
    Sphere s2 = Sphere(Vec3d(0, -40.0, -4.0), 39.3, Vec3d(1, 0.0, 0.0));
    int numRays = 50; // number of rays fired through each pixel
    for (index_t y = 0; y < img.height; ++y) {
        for (index_t x = 0; x < img.width; ++x) {
            Vec2i xy = Vec2i(x,y);                // integer pixel coordinates
            Vec2d st = ((Vec2d)(xy)) / img.width; // floating point image coordinates
            st -= Vec2d(0.5, (0.5 * img.height) / img.width); // center on (0,0)

            Vec3d o = Vec3d(0.0,0.0,0.0);
            Vec3d d = Vec3d(st[0], st[1], -1.0);
            Ray<double,3> r = Ray<double,3>(o, d);

            Vec3d color = Vec3d(0.0, 0.0, 0.0);

            for (int i = 0; i < numRays; i++) {
                color += traceWithObjects(r, s1, s2, 0);
            }
            color /= numRays;

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
