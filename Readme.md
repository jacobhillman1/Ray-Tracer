Ray Tracer
==========

This is a simple Ray Tracer implemented in C++. The boilerplate for this project was forked from Tim Babb's [lightbox](https://github.com/trbabb/lightbox). This boilerplate basically only provides an easy way of exporting the image as a png.
The project depends on a subset of [geomc](https://github.com/trbabb/geomc) (Tim Babb's own mathematical library) with most of the shapes and tracing functions removed. 

How it Works
============

Ray tracing is the technique of tracing the path of light and simulating the effects of its encounters with virtual objects. Basically, you create a virtual environment with virtual objects. You shoot one or more “rays” through every pixel of the image plane. When the ray hits one of your objects in the image plane, it reacts based on factors such as the surface normal, reflectivity, albedo (color), transparency, etc. Another ray is then shot from that hit location, and the process is repeated recursively until the ray hits either a source of light of goes out of bounds. In order to obtain the effects of everything the ray hits, ray tracers trace the ray from your eye to the light source, while in nature (the real world), the ray is moving from the light source to your eye.

My Implementation
=================

I’m able to create spherical objects in my plane and account for the effect of color and intensity. I have a trace function that takes each object and the ray that is being shot. Each object has a function that takes a ray as the parameter, and if it’s hit, returns the surface normal of the hit. Another ray is then shot from the hit location. Once the ray finds the light source or doesn’t hit anything, the surface normal of each hit is used to determine the “intensity” of the impact that each object has on the resulting color of the light. 

I have implemented Sphere and RayHit classes, and lightbox is where main is located. 

An example output obtained from this program can be seen [here](https://github.com/jacobhillman1/Ray-Tracer/tree/master/output/test.png).

Requirements
============

This project requires [libpng](http://www.libpng.org/pub/png/libpng.html), [zlib](http://www.zlib.net/) (for libpng), and [boost](http://www.boost.org/).

The Makefile is POSIX, but the code can be compiled on other platforms.
