#include "raytracer.h"

/** RayTracer constructor */
RayTracer::RayTracer(Camera &_c):
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
  std::vector<float> result(3, 0.0f);
  //intersecting shit for testing
  Sphere sph(0, 0, -50, 50);
  if (sph.intersect(outRay, *this).isExists)
    result.at(0) = 1.0f;
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

/** Sphere constructor */
Sphere::Sphere(float x, float y, float z, float rad):
  center(3, 0.0f)
{
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
Triangle::Triangle(std::vector<float> p1, std::vector<float> p2, 
		   std::vector<float> p3) {
  v1 = p1;
  v2 = p2;
  v3 = p3;
  normal = crossifier(v1, v2, v3);
}

/** Given an incoming ray, returns NULL if no intersection, otherwise returns
 *  resultant intersection */
Intersection Triangle::intersect(Ray &ray, RayTracer &rt) {
  return Intersection();
}

/** BRDF constructor */
BRDF::BRDF(std::vector<float> _ka, std::vector<float> _kd, std::vector<float> _ks, 
	   std::vector<float> _kr, float _s):
  ka(_ka),
  kd(_kd),
  ks(_ks),
  kr(_kr),
  s(_s)
{}

/** Given three POINTs, return the NORMAL vector to their plane */
std::vector<float> crossifier(std::vector<float> p1, std::vector<float> p2,
			      std::vector<float> p3) {
  std::vector<float> v1(3, 0.0f);
  std::vector<float> v2(3, 0.0f);
  std::vector<float> result(3, 0.0f);
  // vector1
  v1.at(0) = p2.at(0) - p1.at(0);
  v1.at(1) = p2.at(1) - p1.at(1);
  v1.at(2) = p2.at(2) - p1.at(2);
  // vector2
  v2.at(0) = p3.at(0) - p1.at(0);
  v2.at(1) = p3.at(1) - p1.at(1);
  v2.at(2) = p3.at(2) - p1.at(2);
  // X product YEEEEEE
  result.at(0) = v1.at(1)*v2.at(2) - v1.at(2)*v2.at(1);
  result.at(1) = v1.at(0)*v2.at(2) - v1.at(2)*v2.at(0);
  result.at(2) = v1.at(0)*v2.at(1) - v1.at(1)*v2.at(0);
  normalizationizerificationator(result);
  return result;
}

/** Check to see if a point, given a CAMERA, exists within our view */
int RayTracer::isInBounds(const std::vector<float> p) {
  // Distance ratio between camera and point
  float dr;
  float xMin, xMax, yMin, yMax, zMin, zMax;
  
  // lulz
  
  // dir: +x
  if (cam.lookAt.at(0) == 1) {
    dr = (-cam.lookFrom.at(0) + p.at(0))/cam.d;
    if (cam.upDir.at(1) == 1) {
      // up: +y
      yMin = cam.ul.at(1) - (cam.h/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr - 1);
    } else if (cam.upDir.at(1) == -1) {
      // up: -y
      yMin = cam.ul.at(1) - (cam.h/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr + 1);
    } else if (cam.upDir.at(2) == 1) {
      // up: +z
      yMin = cam.ul.at(1) - (cam.w/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr + 1);
    } else if (cam.upDir.at(2) == -1) {
      // up: -z
      yMin = cam.ul.at(1) - (cam.w/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(1) >= yMin && p.at(1) <= yMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
    
    // dir: -x
  } else if (cam.lookAt.at(0) == -1) {
    dr = (cam.lookFrom.at(0) - p.at(0))/cam.d;
    if (cam.upDir.at(1) == 1) {
      // up: +y
      yMin = cam.ul.at(1) - (cam.h/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr + 1);
    } else if (cam.upDir.at(1) == -1) {
      // up: -y
      yMin = cam.ul.at(1) - (cam.h/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr - 1);
    } else if (cam.upDir.at(2) == 1) {
      // up: +z
      yMin = cam.ul.at(1) - (cam.w/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr + 1);
    } else if (cam.upDir.at(2) == -1) {
      // up: -z
      yMin = cam.ul.at(1) - (cam.w/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(1) >= yMin && p.at(1) <= yMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
    
    // dir: +y
  } else if (cam.lookAt.at(1) == 1) {
    dr = (-cam.lookFrom.at(1) + p.at(1))/cam.d;
    if (cam.upDir.at(0) == 1) {
      // up: +x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr + 1);
    } else if (cam.upDir.at(0) == -1) {
      // up: -x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr - 1);
    } else if (cam.upDir.at(2) == 1) {
      // up: +z
      xMin = cam.ul.at(0) - (cam.w/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr + 1);
    } else if (cam.upDir.at(2) == -1) {
      // up: -z
      xMin = cam.ul.at(0) - (cam.w/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(0) >= xMin && p.at(0) <= xMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
  
    // dir: -y
  } else if (cam.lookAt.at(1) == -1) {
    dr = (cam.lookFrom.at(1) - p.at(1))/cam.d;
    if (cam.upDir.at(0) == 1) {
      // up: +x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr - 1);
    } else if (cam.upDir.at(0) == -1) {
      // up: -x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.w/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.w/2) * (dr + 1);
    } else if (cam.upDir.at(2) == 1) {
      // up: +z
      xMin = cam.ul.at(0) - (cam.w/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr - 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr - 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr + 1);
    } else if (cam.upDir.at(2) == -1) {
      // up: -z
      xMin = cam.ul.at(0) - (cam.w/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr + 1);
      zMax = cam.ul.at(2) + (cam.h/2) * (dr + 1);
      zMin = cam.ul.at(2) - (cam.h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(0) >= xMin && p.at(0) <= xMax && p.at(2) <= zMax && p.at(2) >= zMin)
      return 1;
    return 0;
    
    // dir: +z
  } else if (cam.lookAt.at(2) == 1) {
    dr = (-cam.lookFrom.at(2) + p.at(2))/cam.d;
    if (cam.upDir.at(0) == 1) {
      // up: +x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr + 1);
      yMin = cam.ul.at(1) - (cam.w/2) * (dr - 1);
    } else if (cam.upDir.at(0) == -1) {
      // up: -x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr - 1);
      yMin = cam.ul.at(1) - (cam.w/2) * (dr + 1);
    } else if (cam.upDir.at(1) == 1) {
      // up: +y
      xMin = cam.ul.at(0) - (cam.w/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr - 1);
      yMin = cam.ul.at(1) - (cam.h/2) * (dr + 1);
    } else if (cam.upDir.at(1) == -1) {
      // up: -y
      xMin = cam.ul.at(0) - (cam.w/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr + 1);
      yMin = cam.ul.at(1) - (cam.h/2) * (dr - 1);
    } else {
      std::cout << " inBounds has an error!!!! " << std::endl;
      exit(1);
    }
    if (p.at(0) >= xMin && p.at(0) <= xMax && p.at(1) <= yMax && p.at(1) >= yMin)
      return 1;
    return 0;
    
    // dir: -z
  } else if (cam.lookAt.at(2) == -1) {
    dr = (cam.lookFrom.at(2) - p.at(2))/cam.d;
    if (cam.upDir.at(0) == 1) {
      // up: +x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr - 1);
      yMin = cam.ul.at(1) - (cam.w/2) * (dr + 1);
    } else if (cam.upDir.at(0) == -1) {
      // up: -x
      xMin = cam.ul.at(0) - (cam.h/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.h/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.w/2) * (dr + 1);
      yMin = cam.ul.at(1) - (cam.w/2) * (dr - 1);
    } else if (cam.upDir.at(1) == 1) {
      // up: +y
      xMin = cam.ul.at(0) - (cam.w/2) * (dr - 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr + 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr - 1);
      yMin = cam.ul.at(1) - (cam.h/2) * (dr + 1);
    } else if (cam.upDir.at(1) == -1) {
      // up: -y
      xMin = cam.ul.at(0) - (cam.w/2) * (dr + 1);
      xMax = cam.ul.at(0) + (cam.w/2) * (dr - 1);
      yMax = cam.ul.at(1) + (cam.h/2) * (dr + 1);
      yMin = cam.ul.at(1) - (cam.h/2) * (dr - 1);
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

