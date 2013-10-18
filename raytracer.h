#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "camera.h"

#include <vector>
#include <math.h>
#include <algorithm>

/** Forward Declarations */
class Triangle;
class Sphere;

/** Class Declarations */
class RayTracer;
class Intersection;
class Sphere;
class Triangle;
class BRDF;

class RayTracer {
 public:
  RayTracer(Camera &_c);
  /* Trace given RAY recursively to return the resultant color */
  std::vector<float> trace(Ray &outRay, int depth);
  /* After finding an intersection, determine reflection ray at pos */
  Ray createReflectRay(Ray &incRay, Intersection inters);
  /** Check to see if a point, given a CAMERA, exists within our view */
  int isInBounds(const std::vector<float> pos);
  Camera cam;
};

/** Location and Normal of a given surface point in 3D space */
class Intersection {
 public:
  Intersection();
  std::vector<float> pos; //intersection point
  std::vector<float> normal; //normal at intersection
  // Class only contains one of the following. Use null pointers to check which
  Triangle *tri;
  Sphere *sph;
  // Parameter root of intersecting RAY equation
  float t;
  // isExists field (false if intersection does not exist)
  int isExists;
};

/** Sphere class */
class Sphere {
 public:
  /* Sphere constructor */
  Sphere(float x, float y, float z, float rad);
  /* Given an incoming ray, isExists == 0 if no intersection, otherwise returns
   * resultant intersection */
  Intersection intersect(Ray &ray, RayTracer &rt);
  // Fields
  std::vector<float> center;
  float radius;
};

/** Triangle class */
class Triangle {
 public:
  /* Triangle constructor */
  Triangle(std::vector<float> p1, std::vector<float> p2, std::vector<float> p3);
  /* Given an incoming ray, isExists == 0 if no intersection, otherwise returns
   * resultant intersection */
  Intersection intersect(Ray &ray, RayTracer &rt);
  // Fields
  std::vector<float> v1, v2, v3;
  // Normal (arbitrary direction, figure out appropriate one later)
  std::vector<float> normal;
};

/** BRDF class */
class BRDF {
 public:
  BRDF(std::vector<float> _ka, std::vector<float> _kd, std::vector<float> _ks, std::vector<float> _kr, float _s);
  std::vector<float> ka, kd, ks, kr;
  float s;
};

/** Method Declarations */
/** Quadratic solver, sets x0 and x1 to to the roots of quadratic function, given
 *  a, b, and c of the quadratic formula. */
int quadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

/** Given three POINTs, return the NORMAL vector to their plane */
std::vector<float> crossifier(std::vector<float> p1, std::vector<float> p2, 
			      std::vector<float> p3);

#endif
