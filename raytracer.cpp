#include "raytracer.h"

/** RayTracer constructor */
RayTracer::RayTracer(Camera *_c):
  cam(_c)
{}

/** Define Intersection constructor */
Intersection::Intersection():
  pos(3, 0.0f),
  normal(3, 0.0f),
  isExists(1)
{}

/** Trace given RAY recursively to return the resultant RGB color */
std::vector<float> RayTracer::trace(Ray &outRay, int depth) {
  //Triangle a = *triangles.at(0);
  std::vector<float> result(3, 0.0f);
  //intersecting shit for testing
  for (std::vector<Triangle *>::iterator t = triangles->begin(); 
       t != triangles->end(); ++t) {
    if ((*t)->intersect(outRay, *this).isExists) {
      result.at(0) = 1.0f;
    }
  }
  
  return result;
}

/** After finding an intersection, determine reflection ray at pos from input ray */
Ray RayTracer::createReflectRay(Ray &incRay, Intersection inters){
  float dot = (incRay.dir.at(0)*inters.normal.at(0) + 
	       incRay.dir.at(1)*inters.normal.at(1) + 
	       incRay.dir.at(2)*inters.normal.at(2));
  std::vector<float> result(3, 0.0f);
  result.at(0) = -incRay.dir.at(0) + 2*dot*inters.normal.at(0);
  result.at(1) = -incRay.dir.at(1) + 2*dot*inters.normal.at(1);
  result.at(2) = -incRay.dir.at(2) + 2*dot*inters.normal.at(2);
  normalizationizerificationator(result);
  return Ray(inters.pos, result);
}

/* DirectionalLight constructor */
DirectionalLight::DirectionalLight(float x, float y, float z, float r, float g, 
				   float b):
  lightDir(3, 0.0f),
  rgb(3, 0.0f)
{
  lightDir.at(0) = x;
  lightDir.at(1) = y;
  lightDir.at(2) = z;
  rgb.at(0) = r;
  rgb.at(1) = g;
  rgb.at(2) = b; 
}

/* PointLight constructor (pos) */
PointLight::PointLight(float x, float y, float z, float r, float g, float b):
  pos(3, 0.0f),
  rgb(3, 0.0f)
{
  pos.at(0) = x;
  pos.at(1) = y;
  pos.at(2) = z;
  rgb.at(0) = r;
  rgb.at(1) = g;
  rgb.at(2) = b;
}


/** Sphere constructor */
Sphere::Sphere(float x, float y, float z, float rad, BRDF b):
  center(3, 0.0f)
{
  center.at(0) = x;
  center.at(1) = y;
  center.at(2) = z;
  radius = rad;
  brdf = b;
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
  // Set x0 to closest positive parameter
  if (x0 < 1.0f && x1 < 1.0f) { //Prevents selfshadowing as well
    return 0;
  } else if (x0 < 1.0f) {
    std::swap(x0, x1);
  } else {
    if (x0 > x1 && x1 >= 1.0f) std::swap(x0, x1);
  }
  return 1;
}

/** Given an incoming ray, returns NULL if no intersection, otherwise returns
 *  resultant intersection */
Intersection Sphere::intersect(Ray &ray, RayTracer &rt) {
  // Roots of intersection
  float t0 = 0.0f;
  float t1 = 0.0f;
  // emc: e minus c (ray's startPos - sphere center)
  std::vector<float> emc(3, 0.0f);
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
  // Initialize intersection, currently isExists == 1
  Intersection inters;
  if (!quadratic(a, b, c, t0, t1)) {
    // SET isExists = 0
    inters.isExists = 0;
    return inters;
  }
  inters.t = t0;
  inters.sph = this;
  inters.pos.at(0) = ray.startPos.at(0) + t0*ray.dir.at(0);
  inters.pos.at(1) = ray.startPos.at(1) + t0*ray.dir.at(1);
  inters.pos.at(2) = ray.startPos.at(2) + t0*ray.dir.at(2);
  // NULL if point not inbounds
  if (!rt.isInBounds(inters.pos)) {
    // SET isExists = 0
    inters.isExists = 0;
    return inters;
  }
  inters.normal.at(0) = inters.pos.at(0) - center.at(0);
  inters.normal.at(1) = inters.pos.at(1) - center.at(1);
  inters.normal.at(2) = inters.pos.at(2) - center.at(2);
  normalizationizerificationator(inters.normal);
  // By this point, intersection is then valid.
  return inters;
}

/** Triangle constructor */
Triangle::Triangle(std::vector<float> *p0, std::vector<float> *p1, 
		   std::vector<float> *p2, BRDF b) {
  v0 = p0;
  v1 = p1;
  v2 = p2;
  brdf = b;
  normal = crossifier(*v0, *v1, *v2);
}

/** Given an incoming ray, returns NULL if no intersection, otherwise returns
 *  resultant intersection */
Intersection Triangle::intersect(Ray &ray, RayTracer &rt) {

  // Initialize intersection, currently isExists == 1
  Intersection inters;

  //getting the dotproduct of: the triangle normal dot ray.dir
  float nDotRayDir = dot(normal, ray.dir);
  //if the ray is parallel to triangle plane, there is no intersection
  if(nDotRayDir == 0) {
    inters.isExists = 0;
    return inters;
  }

  /** First determine point at which entire triangle plane intersects ray
   *  To do that, first find triangle's full plane equation. */
  // calculating D (from the eqation: Ax + By + Cz + D = 0), where D is the distance 
  // from the O(0,0,0) to the plane. We can find D by using any of the 3 triangle 
  // vertices, but I will use v1.
  // To find D, I'm doing: normal dot v0
  float d = dot(normal, *v0);

  // finding the value of t in P = O + tD, where P is the intersection point
  //of the ray and the plane
  //t = -(dot(normal, ray.startPos) + D) / dot(normal, ray.dir)
  float t = -(dot(normal, ray.startPos) + d) / nDotRayDir;

  //checking if the triangle is behind the ray:
  if (t<0) {
    inters.isExists = 0;
    return inters;
  }

  //computing the intersection point, p, of the ray to the plane, given by P = O + tD
  std::vector<float> p(3, 0.0f);
  p.at(0) = ray.startPos.at(0) + t * ray.dir.at(0);
  p.at(1) = ray.startPos.at(1) + t * ray.dir.at(1);
  p.at(2) = ray.startPos.at(2) + t * ray.dir.at(2);

  ////////////////////
  //Now, I'm going to check whether this point p lies inside or outside triangle:
  ////////////////////

  //edge 0, defined by v1-v0:
  std::vector<float> edge0(3, 0.0f);
  edge0.at(0) = v1->at(0) - v0->at(0);
  edge0.at(1) = v1->at(1) - v0->at(1);
  edge0.at(2) = v1->at(2) - v0->at(2);

  //Finding the vector going from vectex v0 of the triangle to the point p (the intersection of the
  //ray with the plane)
  std::vector<float> VP0(3, 0.0f);
  VP0.at(0) = p.at(0) - v0->at(0);
  VP0.at(1) = p.at(1) - v0->at(1);
  VP0.at(2) = p.at(2) - v0->at(2);

  //vector norm0 that is perpendicular to the triangle plane
  std::vector<float> norm0 = crossifier(edge0, VP0);

  //finding the dot product of normal dot norm0:
  float nDotNorm = dot(normal, norm0);
  if (nDotNorm < 0) {
    //point p is outside triangle
    inters.isExists = 0;
    return inters;
  }

  //edge 1, defined by v2 - v1:
  std::vector<float>edge1(3, 0.0f);
  edge1.at(0) = v2->at(0) - v1->at(0);
  edge1.at(1) = v2->at(1) - v1->at(1);
  edge1.at(2) = v2->at(2) - v1->at(2);

  //Finding the vector going from vectex v1 of the triangle to the point p (the intersection of the
  //ray with the plane)
  std::vector<float> VP1(3, 0.0f);
  VP1.at(0) = p.at(0) - v1->at(0);
  VP1.at(1) = p.at(1) - v1->at(1);
  VP1.at(2) = p.at(2) - v1->at(2);

  //vector norm1 that is perpendicular to the triangle plane
  std::vector<float> norm1 = crossifier(edge1, VP1);

  //finding the dot product of normal dot norm1:
  nDotNorm = dot(normal, norm1);
  if (nDotNorm < 0) {
    //point p is outside triangle
    inters.isExists = 0;
    return inters;
  }

  //edge 2, defined by v0 - v2:
  std::vector<float>edge2(3, 0.0f);
  edge2.at(0) = v0->at(0) - v2->at(0);
  edge2.at(1) = v0->at(1) - v2->at(1);
  edge2.at(2) = v0->at(2) - v2->at(2);

  //Finding the vector going from vectex v2 of the triangle to the point p (the intersection of the
  //ray with the plane)
  std::vector<float> VP2(3, 0.0f);
  VP2.at(0) = p.at(0) - v2->at(0);
  VP2.at(1) = p.at(1) - v2->at(1);
  VP2.at(2) = p.at(2) - v2->at(2);

  //vector norm2 that is perpendicular to the triangle plane
  std::vector<float> norm2 = crossifier(edge2, VP2);

  //finding the dot product of normal dot norm1:
  nDotNorm = dot(normal, norm2);
  if (nDotNorm < 0) {
    //point p is outside triangle
    inters.isExists = 0;
    return inters;
  }

  //The last check: going to check if the intersection.pos isInBounds:
  inters.pos.at(0) = p.at(0);
  inters.pos.at(1) = p.at(1);
  inters.pos.at(2) = p.at(2);

  // NULL if point not inbounds
  if (!rt.isInBounds(inters.pos)) {
    // SET isExists = 0
    inters.isExists = 0;
    return inters;
  }

  //At this point, we know the intersection point p we found is the ray-triangle intersection!
  inters.tri = this;
  inters.t = t;
  inters.normal.at(0) = normal.at(1);
  inters.normal.at(1) = normal.at(1);
  inters.normal.at(2) = normal.at(2);
  normalizationizerificationator(inters.normal);
  // By this point, intersection is then valid.
  return inters;
}

/** TriNormal constructor */
TriNormal::TriNormal(std::vector<float> *p0, std::vector<float> *p1, 
		     std::vector<float> *p2, std::vector<float> *l0,
		     std::vector<float> *l1, std::vector<float> *l2, BRDF b) {
  v0 = p0;
  v1 = p1;
  v2 = p2;
  n0 = l0;
  n1 = l1;
  n2 = l2;
  brdf = b;
}

/* Calculate adjusted normal */
std::vector<float> TriNormal::trinormalFinder(Intersection &inters) {
  return std::vector<float>(3, 0.0f);
}

/** BRDF constructor */
BRDF::BRDF():
  ka(3, 0.2f),			// default value = 0.2f
  kd(3, 0.0f),
  ks(3, 0.0f),
  kr(3, 0.0f),
  ke(3, 0.0f),
  s(0.0f)
{}









/** Check to see if a point, given a CAMERA, exists within our view */
int RayTracer::isInBounds(const std::vector<float> p) {
  // Distance ratio between camera and point
  float dr;
  float xMin, xMax, yMin, yMax, zMin, zMax;
  
  // lulz
  
  // dir: +x
  if (cam->lookAt.at(0) == 1) {
    dr = (-cam->lookFrom.at(0) + p.at(0))/cam->d;
    if (cam->upDir.at(1) == 1) {
      // up: +y
      yMin = cam->ul.at(1) - (cam->h/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr - 1);
    } else if (cam->upDir.at(1) == -1) {
      // up: -y
      yMin = cam->ul.at(1) - (cam->h/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr + 1);
    } else if (cam->upDir.at(2) == 1) {
      // up: +z
      yMin = cam->ul.at(1) - (cam->w/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr + 1);
    } else if (cam->upDir.at(2) == -1) {
      // up: -z
      yMin = cam->ul.at(1) - (cam->w/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(1) >= yMin && p.at(1) <= yMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
    
    // dir: -x
  } else if (cam->lookAt.at(0) == -1) {
    dr = (cam->lookFrom.at(0) - p.at(0))/cam->d;
    if (cam->upDir.at(1) == 1) {
      // up: +y
      yMin = cam->ul.at(1) - (cam->h/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr + 1);
    } else if (cam->upDir.at(1) == -1) {
      // up: -y
      yMin = cam->ul.at(1) - (cam->h/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr - 1);
    } else if (cam->upDir.at(2) == 1) {
      // up: +z
      yMin = cam->ul.at(1) - (cam->w/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr + 1);
    } else if (cam->upDir.at(2) == -1) {
      // up: -z
      yMin = cam->ul.at(1) - (cam->w/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(1) >= yMin && p.at(1) <= yMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
    
    // dir: +y
  } else if (cam->lookAt.at(1) == 1) {
    dr = (-cam->lookFrom.at(1) + p.at(1))/cam->d;
    if (cam->upDir.at(0) == 1) {
      // up: +x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr + 1);
    } else if (cam->upDir.at(0) == -1) {
      // up: -x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr - 1);
    } else if (cam->upDir.at(2) == 1) {
      // up: +z
      xMin = cam->ul.at(0) - (cam->w/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr + 1);
    } else if (cam->upDir.at(2) == -1) {
      // up: -z
      xMin = cam->ul.at(0) - (cam->w/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(0) >= xMin && p.at(0) <= xMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
  
    // dir: -y
  } else if (cam->lookAt.at(1) == -1) {
    dr = (cam->lookFrom.at(1) - p.at(1))/cam->d;
    if (cam->upDir.at(0) == 1) {
      // up: +x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr - 1);
    } else if (cam->upDir.at(0) == -1) {
      // up: -x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->w/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->w/2) * (dr + 1);
    } else if (cam->upDir.at(2) == 1) {
      // up: +z
      xMin = cam->ul.at(0) - (cam->w/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr - 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr - 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr + 1);
    } else if (cam->upDir.at(2) == -1) {
      // up: -z
      xMin = cam->ul.at(0) - (cam->w/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr + 1);
      zMax = cam->ul.at(2) + (cam->h/2) * (dr + 1);
      zMin = cam->ul.at(2) - (cam->h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(0) >= xMin && p.at(0) <= xMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
    
    // dir: +z
  } else if (cam->lookAt.at(2) == 1) {
    dr = (-cam->lookFrom.at(2) + p.at(2))/cam->d;
    if (cam->upDir.at(0) == 1) {
      // up: +x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr + 1);
      yMin = cam->ul.at(1) - (cam->w/2) * (dr - 1);
    } else if (cam->upDir.at(0) == -1) {
      // up: -x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr - 1);
      yMin = cam->ul.at(1) - (cam->w/2) * (dr + 1);
    } else if (cam->upDir.at(1) == 1) {
      // up: +y
      xMin = cam->ul.at(0) - (cam->w/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr - 1);
      yMin = cam->ul.at(1) - (cam->h/2) * (dr + 1);
    } else if (cam->upDir.at(1) == -1) {
      // up: -y
      xMin = cam->ul.at(0) - (cam->w/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr + 1);
      yMin = cam->ul.at(1) - (cam->h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(0) >= xMin && p.at(0) <= xMax && p.at(1) <= yMax && p.at(1) >= yMin)
      return 1;
    return 0;
    
    // dir: -z
  } else if (cam->lookAt.at(2) == -1) {
    dr = (cam->lookFrom.at(2) - p.at(2))/cam->d;
    if (cam->upDir.at(0) == 1) {
      // up: +x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr - 1);
      yMin = cam->ul.at(1) - (cam->w/2) * (dr + 1);
    } else if (cam->upDir.at(0) == -1) {
      // up: -x
      xMin = cam->ul.at(0) - (cam->h/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->h/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->w/2) * (dr + 1);
      yMin = cam->ul.at(1) - (cam->w/2) * (dr - 1);
    } else if (cam->upDir.at(1) == 1) {
      // up: +y
      xMin = cam->ul.at(0) - (cam->w/2) * (dr - 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr + 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr - 1);
      yMin = cam->ul.at(1) - (cam->h/2) * (dr + 1);
    } else if (cam->upDir.at(1) == -1) {
      // up: -y
      xMin = cam->ul.at(0) - (cam->w/2) * (dr + 1);
      xMax = cam->ul.at(0) + (cam->w/2) * (dr - 1);
      yMax = cam->ul.at(1) + (cam->h/2) * (dr + 1);
      yMin = cam->ul.at(1) - (cam->h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(0) >= xMin && p.at(0) <= xMax && p.at(1) <= yMax && p.at(1) >= yMin)
      return 1;
    return 0;
    
  } else {
    std::cout << " inBounds has an error!!!! " << std::endl;
    exit(1);
  }
}

