#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "camera.h"

#include <vector>
#include <math.h>
#include <algorithm>

/** Class Declarations */
class RayTracer;
class Intersection;
class DirectionalLight;
class PointLight;
class Sphere;
class Triangle;
class TriNormal;
class BRDF;

class RayTracer {
 public:
  RayTracer(Camera *_c);
  /* Trace given RAY recursively to return the resultant color */
  std::vector<float> trace(Ray &outRay, int depth);
  /* After finding an intersection, determine reflection ray at pos */
  Ray createReflectRay(Ray &incRay, Intersection inters);
  /** Check to see if a point, given a CAMERA, exists within our view */
  int isInBounds(const std::vector<float> pos);
  Camera *cam;
  // Containers for iterators
  std::vector<TriNormal *> *trinorms;
  std::vector<Triangle *> *triangles;
  std::vector<Sphere *> *spheres;
  std::vector<DirectionalLight *> *DLs;
  std::vector<PointLight *> *PLs;
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

/** DirectionalLight class */
class DirectionalLight {
 public:
  /* DirectionalLight constructor */
  DirectionalLight(float x, float y, float z, float r, float g, float b);
  std::vector<float> lightDir;
  std::vector<float> rgb;
};

/** PointLight class */
class PointLight {
 public:
  /* PointLight constructor (pos) */
  PointLight(float x, float y, float z, float r, float g, float b);
  std::vector<float> pos;
  std::vector<float> rgb;
};

/** BRDF class */
class BRDF {
 public:
  BRDF();
  std::vector<float> ka, kd, ks, kr, ke;
  float s;
};

/** Sphere class */
class Sphere {
 public:
  /* Sphere constructor */
  Sphere(float x, float y, float z, float rad, BRDF b);
  /* Given an incoming ray, isExists == 0 if no intersection, otherwise returns
   * resultant intersection */
  Intersection intersect(Ray &ray, RayTracer &rt);
  std::vector<float> center;
  BRDF brdf;
  float radius;
};

/** Triangle class */
class Triangle {
 public:
  /* Triangle constructor */
  Triangle(std::vector<float> *p0, std::vector<float> *p1, std::vector<float> *p2,
	   BRDF b);
  /* Given an incoming ray, isExists == 0 if no intersection, otherwise returns
   * resultant intersection */
  Intersection intersect(Ray &ray, RayTracer &rt);
  // Vertices
  std::vector<float> *v0, *v1, *v2;
  // Normal (arbitrary direction, figure out appropriate one later)
  std::vector<float> normal;
  BRDF brdf;
};

/** TriNormal class */
class TriNormal {
 public:
  /* Triangle constructor */
  TriNormal(std::vector<float> *p0, std::vector<float> *p1, std::vector<float> *p2,
	    std::vector<float> *l0, std::vector<float> *l1, std::vector<float> *l2,
	    BRDF b);
  /* Given an incoming ray, isExists == 0 if no intersection, otherwise returns
   * resultant intersection */
  Intersection intersect(Ray &ray, RayTracer &rt);
  /* Calculate adjusted normal */
  std::vector<float> trinormalFinder(Intersection &inters);
  // Vertices
  std::vector<float> *v0, *v1, *v2;
  // Normal (trinormal arbitraries)
  std::vector<float> *n0, *n1, *n2;
  BRDF brdf;
};

/** Method Declarations */
/** Quadratic solver, sets x0 and x1 to to the roots of quadratic function, given
 *  a, b, and c of the quadratic formula. */
int quadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

#endif
