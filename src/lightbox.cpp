#include <iostream>
#include <geomc/linalg/Vec.h>
#include <geomc/linalg/AffineTransform.h>

#include "Image.h"

using namespace geom;
using namespace std;

bool sphere_intersection(Ray<double, 3> r, double sphereRadius, Vec3d center);
Vec3d calculate_surface_normal(Ray<double, 3> r, double sphereRadius, Vec3d center);
Vec3d calculate_surface_normal_on_sphere(Ray<double, 3> r, double radius, Vec3d center);

const int x = 0;
const int y = 1;
const int z = 2;

// Produce a pixel color based on the image coordinates x,y.
// The image center will be (0,0), and x will range from -0.5 to 0.5.
inline Vec3d render_pixel(Vec2d xy) {
    double phz = xy.x * M_PI * 12;

    //here's some expression which makes a pixel color value:
    Vec3d px = Vec3d(
        cos(phz)/2 + 0.5,      // red
        cos(phz*1.1)/2 + 0.5,  // green
        cos(phz*1.2)/2 + 0.5); // blue

    return px;
}

inline Vec3d render_pixel_surface_normal(Vec2d xy) {
    Vec3d o = Vec3d(0.0,0.0,0.0);
    Vec3d d = Vec3d(xy[0], xy[1], 1.0);
    Ray<double,3> r = Ray<double,3>(o, d);

    Vec3d px = Vec3d(0, .5, 1);

    if(sphere_intersection(r, 0.5, Vec3d(0.0, 0.0, 4.0))) {
        Vec3d surfaceNormal = calculate_surface_normal_on_sphere(r, 0.5, Vec3d(0.0, 0.0, 4.0));
        for (int i = 0; i < 3; i ++) {
            if (surfaceNormal[i] < 0)
                surfaceNormal[i] = -surfaceNormal[i];
        }
        px = surfaceNormal;
    }

    return px;

}

// Returns true if a ray r intersects with a sphere with a certain radius at
// location c from the origin.
bool sphere_intersection(Ray<double, 3> r, double sphereRadius, Vec3d center) {

    // calculating the a, b, and c values to plug into the quadratic equation
    double a = pow(r.direction[x],2) + pow(r.direction[y],2) + pow(r.direction[z],2);

    double b = 2 * ((r.origin[x] - center[x])*(r.direction[x]) + (r.origin[y] - center[y])*(r.direction[y])
        + (r.origin[z] - center[z])*(r.direction[z]));

    double c = pow((r.origin[x] - center[x]), 2) + pow((r.origin[y] - center[y]), 2)
        + pow((r.origin[z] - center[z]), 2) - pow(sphereRadius, 2);

    if (pow(b, 2) - 4*a*c >= 0 && a != 0)
        return true;
    else
        return false;

}

// Returns a Vec3d object of the location of the closest intercept to a sphere
Vec3d find_sphere_intersections(Ray<double, 3> r, double radius, Vec3d center){

    // calculating the a, b, and c values to plug into the quadratic equation
    double a = pow(r.direction[x],2) + pow(r.direction[y],2) + pow(r.direction[z],2);

    double b = 2 * ((r.origin[x] - center[x])*(r.direction[x]) + (r.origin[y] - center[y])*(r.direction[y])
        + (r.origin[z] - center[z])*(r.direction[z]));

    double c = pow((r.origin[x] - center[x]), 2) + pow((r.origin[y] - center[y]), 2)
        + pow((r.origin[z] - center[z]), 2) - pow(radius, 2);

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

Vec3d calculate_surface_normal_on_sphere(Ray<double, 3> r, double radius, Vec3d center) {
    Vec3d intersection = find_sphere_intersections(r, radius, center);

    Vec3d surfaceNormal = Vec3d((intersection[x] - center[x]) / radius, (intersection[y] - center[y]) / radius,
        (intersection[z] - center[z]) / radius);

    return surfaceNormal;

}

void render_image(Image<double,3>& img) {
    for (index_t y = 0; y < img.height; ++y) {
        for (index_t x = 0; x < img.width; ++x) {
            Vec2i xy = Vec2i(x,y);                // integer pixel coordinates
            Vec2d st = ((Vec2d)(xy)) / img.width; // floating point image coordinates
            st -= Vec2d(0.5, (0.5 * img.height) / img.width); // center on (0,0)

            img[xy] = render_pixel_surface_normal(st);
        }
    }
}


int main(int argc, char** argv) {

    Image<double,3> img(1024, 512); // a 3-channel image.

    render_image(img);

    img.save_png("output/test.png");

    return 0;
}
