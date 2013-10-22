#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "algebra3.h" //transf
#include "camera.h"

#include <vector>
#include <math.h>
#include <algorithm>
#include <stdexcept>

/** Class Declarations */
class RayTracer;
class Intersection;
class DirectionalLight;
class PointLight;
class Shape;
class Sphere;
class Triangle;
class TriNormal;
class BRDF;

class RayTracer {
 public:
  RayTracer(Camera *_c);
  /* Trace given RAY recursively to return the resultant color */
  std::vector<float> trace(Ray &outRay, int depth);
  Camera *cam;
  // Containers for iterators
  std::vector<Shape *> *shapes;
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
  Shape *sh;
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
  PointLight(float x, float y, float z, float r, float g, float b, float a1, 
	     float a2, float a3);
  std::vector<float> pos;
  std::vector<float> rgb;
  float att1, att2, att3;
};

/** BRDF class */
class BRDF {
 public:
  BRDF();
  std::vector<float> ka, kd, ks, kr, ke;
  float s;
};

/** Shape class */
class Shape {
 public:
  Shape(unsigned int n);
  Triangle *tri;
  Sphere *sph;
  TriNormal *tn;
  unsigned int num;
  BRDF brdf;
};

/** Sphere class */
class Sphere {
 public:
  Sphere();
  /* Sphere constructor */
  Sphere(float x, float y, float z, float rad, BRDF b, unsigned int n, mat4 transformMatrix);
  /* Given an incoming ray, isExists == 0 if no intersection, otherwise returns
   * resultant intersection */
  Intersection intersect(Ray &ray, RayTracer &rt);
  std::vector<float> center;
  BRDF brdf;
  float radius;
  unsigned int num;

  mat4 transMat;//transf
  mat4 inverse;//transf

};

/** Triangle class */
class Triangle {
 public:
  Triangle();
  /* Triangle constructor */
  Triangle(std::vector<float> *p0, std::vector<float> *p1, std::vector<float> *p2,
	   BRDF b, unsigned int n);
  /* Given an incoming ray, isExists == 0 if no intersection, otherwise returns
   * resultant intersection */
  Intersection intersect(Ray &ray, RayTracer &rt);
  // Vertices
  std::vector<float> *v0, *v1, *v2;
  // Normal (arbitrary direction, figure out appropriate one later)
  std::vector<float> normal;
  BRDF brdf;
  unsigned int num;
};

/** TriNormal class */
class TriNormal {
 public:
  TriNormal();
  /* Triangle constructor */
  TriNormal(std::vector<float> *p0, std::vector<float> *p1, std::vector<float> *p2,
	    std::vector<float> *l0, std::vector<float> *l1, std::vector<float> *l2,
	    BRDF b, unsigned int n);
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
  unsigned int num;
};

/** Method Declarations */
/** Quadratic solver, sets x0 and x1 to to the roots of quadratic function, given
 *  a, b, and c of the quadratic formula. */
int quadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

/** Finds the vector from pt1 to pt2 */
std::vector<float> vectorizer(std::vector<float> pt1, std::vector<float> pt2);

/** After finding an intersection, determine reflection ray at pos */
Ray createReflectRay(Ray &incRay, Intersection &inters);

/** Find reflect vector (not Ray) */
std::vector<float> getRefVector(std::vector<float> lightDir, 
				std::vector<float> surfNorm);

/** Get negative vector */
std::vector<float> getNegative(std::vector<float> v);

/** Ambient function */
std::vector<float> ambientify(std::vector<float> acolor, std::vector<float> icolor);

/** Diffuse function
 *  returns Rd, diffuse RGB value */
std::vector<float> diffusify(std::vector<float> dcolor, std::vector<float> icolor, 
			     std::vector<float> lightDir, 
			     std::vector<float> surfaceNormal);

/** Specular function
 * reflectedRay found using PARAMS lightDir and PARAMS surfaceNorm
 * input vectors MUST be normalized first
 * returns Rs, specular RGB value */
std::vector<float> specularify(std::vector<float> scolor, std::vector<float> icolor, 
			       std::vector<float> lightDir, 
			       std::vector<float> surfaceNorm, 
			       std::vector<float> dirToViewer, float pCoeff);

/** Combine and cap RGB values to find resultant RGB
 * PARAM rgbs : array of the RGBs
 * PARAM num : length of rgbs array (# of RGBs to be combined) */
std::vector<float> shAverager(std::vector<float> rgbs[], int num);

//takes in a vector<float> v and converts it to a vec3 object
vec3 convertToVec3(std::vector<float> v);

//takes in a vec3 object and converts it to a vector<float>
std::vector<float> convertToVectorFloat(vec3 v);

#endif
