#ifndef SCENE_H
#define SCENE_H

#include "raytracer.h"
#include "camera.h"
#include "film.h"
#include "sampler.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>

/** Class Declarationss */
class Sphere;
class Triangle;

/** Sphere class */
class Sphere {
  /* Sphere constructor */
  Sphere(float x, float y, float z, float rad);
  /* Given an incoming ray, returns NULL if no intersection, otherwise returns
   * resultant intersection */
  Intersection* intersect(Ray &ray);
  // Fields
  vector<float> center;
  float radius;
};

/** Triangle class */
class Triangle {
 public:
  /* Triangle constructor */
  Triangle(vector<float> p1, vector<float> p2, vector<float> p3);
  /* Given an incoming ray, returns NULL if no intersection, otherwise returns
   * resultant intersection */
  Intersection* intersect(Ray &ray);
  // Fields
  vector<float> v1, v2, v3;
  // Normal (arbitrary direction, figure out appropriate one later)
  vector<float> normal;
};

/** Method Declarations */
/** Quadratic solver, sets x0 and x1 to to the roots of quadratic function, given
 *  a, b, and c of the quadratic formula. */
int quadratic(const float &a, const float &b, const float &c, float &x0, float &x1);



#endif
