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

class RayTracer {
 public:
  /* Trace given RAY recursively to return the resultant color */
  vector<float> trace(Ray &outRay, int depth);
  /* After finding an intersection, determine reflection ray at pos */
  Ray createReflectRay(Ray &incRay, Intersection inters);
  /* Method that normalizes a ray */
  void normalizationizerificationator(Ray &ray);
};

/** Location and Normal of a given surface point in 3D space */
class Intersection {
 public:
  vector<float> pos; //intersection point
  vector<float> normal; //normal at intersection
  // Class only contains one of the following. Use null pointers to check which
  Triangle *tri;
  Sphere *sph;
  // Parameter root of intersecting RAY equation
  float t;
};

/** Method Declarations */
/** Given three POINTs, return the NORMAL vector to their plane */
vector<float> crossifier(vector<float> p1, vector<float> p2, vector<float> p3);

/** Check to see if a point, given a CAMERA, exists within our view */
int isInBounds(const vector<float> pos, const Camera cam);


#endif
