#include "scene.h"

/** Sphere constructor */
Sphere::Sphere(float x, float y, float z, float rad) {
  center(3, 0.0f);
  center.at(0) = x;
  center.at(1) = y;
  center.at(2) = z;
  radius = rad;
}

/** Quadratic solver, sets x0 and x1 to to the roots of quadratic function, given
 *  a, b, and c of the quadratic formula.
 *  Return 0 if roots don't exist, else return 1 */
int quadratic(const float &a, const float &b, const float &c, float &x0, float &x1) {
  float discr = b * b - 4 * a * c;
  if (discr < 0) {
    return 0;
  }
  else if (discr == 0) {
    x0 = x1 = - 0.5 * b / a;
  } else {
    float q = (b > 0) ?
      -0.5 * (b + sqrt(discr)):
      -0.5 * (b - sqrt(discr));
    x0 = q / a;
    x1 = c / q;
  }
  if (x0 > x1) std::swap(x0, x1);
  return 1;
}

/** Given an incoming ray, returns NULL if no intersection, otherwise returns
 *  resultant intersection */
Intersection* Sphere::intersect(Ray &ray) {
  // Roots of intersection
  float t0 = 0f;
  float t1 = 0f;
  // emc: e minus c (ray's startPos - sphere center)
  vector<float> emc(3, 0.0f);
  emc.at(0) = ray.startPos.at(0) - center.at(0);
  emc.at(1) = ray.startPos.at(1) - center.at(1);
  emc.at(2) = ray.startPos.at(2) - center.at(2);
  // a: d dot d     (ray's dir dotproduct itself) EQUALS 1 for unit d
  float a = (ray.dir.at(0)*ray.dir.at(0) + ray.dir.at(1)*ray.dir.at(1) + 
	     ray.dir.at(2)*ray.dir.at(2));
  // b: 2* (d dot ((e-c)dot(e-c)))    (2 times (d dot (emc dotproduct itself)))
  float b = 2 * (ray.dir.at(0)*emc.at(0) + ray.dir.at(1)*emc.at(1) +
		 ray.dir.at(2)*emc.at(2));
  // c: emc dot emc - r^2        (emc dot emc - sphere's radius^2)
  float c = (emc.at(0)*emc.at(0) + emc.at(1)*emc.at(1) + emc.at(2)*emc.at(2) -
	     radius*radius);
  if (!quadratic(a, b, c, t0, t1))
    return NULL;
  // Prevent self-intersections.
  if (t0 < 1.0f)
    return NULL;
  // Intersection now known to exist, create it.
  Intersection inters();
  inters.t = t0;
  inters.sphere = &this;
  inters.pos.at(0) = ray.startPos.at(0) + t0*ray.dir.at(0);
  inters.pos.at(1) = ray.startPos.at(1) + t0*ray.dir.at(1);
  inters.pos.at(2) = ray.startPos.at(2) + t0*ray.dir.at(2);
  inters.normal.at(0) = inters.pos.at(0) - center.at(0);
  inters.normal.at(1) = inters.pos.at(1) - center.at(1);
  inters.normal.at(2) = inters.pos.at(2) - center.at(2);
  normalizationizerificationator(inters.normal);
  // NULL if point not inbounds
  if (!isInBounds(inters.pos, AAAAAAAAAAAAAAAA))
    return NULL;
  // By this point, intersection is then valid.
  return &inters;
}

/** Triangle constructor */
Triangle::Triangle(vector<float> p1, vector<float> p2, vector<float> p3) {
  v1 = p1;
  v2 = p2;
  v3 = p3;
  normal = crossifier(v1, v2, v3);
}

/** Given an incoming ray, returns NULL if no intersection, otherwise returns
 *  resultant intersection */
Intersection* Triangle::intersect(Ray &ray) {
  
}
