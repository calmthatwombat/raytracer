#include "raytracer.h"

/** Define Intersection constructor */
Intersection::Intersection():
  pos(3, 0.0f),
  normal(3, 0.0f)
{}

/** Trace given RAY recursively to return the resultant color */
vector<float> RayTracer::trace(Ray &outRay, int depth) {
  vector<float> result(3, 0.0f);
}

/** After finding an intersection, determine reflection ray at pos from input ray */
Ray RayTracer::createReflectRay(Ray &incRay, Intersection inters){
  float dot = (incRay.dir.at(0)*inters.normal.at(0) + 
	       incRay.dir.at(1)*inters.normal.at(1) + 
	       incRay.dir.at(2)*inters.normal.at(2));
  vector<float> result(3, 0.0f);
  result.at(0) = -incRay.dir.at(0) + 2*dot*inters.normal.at(0);
  result.at(1) = -incRay.dir.at(1) + 2*dot*inters.normal.at(1);
  result.at(2) = -incRay.dir.at(2) + 2*dot*inters.normal.at(2);
  return Ray(inters.pos, normalizationizerificationator(result));
}

/** Given a RAY, normalizes its vector segment. */
void RayTracer::normalizationizerificationator(Ray &ray) {
  float mag = sqrt(pow(ray.dir.at(0), 2.0) + pow(ray.dir.at(1), 2.0) + 
		   pow(ray.dir.at(2), 2.0));
  ray.dir.at(0) = ray.dir.at(0)/mag;
  ray.dir.at(1) = ray.dir.at(1)/mag;
  ray.dir.at(2) = ray.dir.at(2)/mag;
}

/** Given three POINTs, return the NORMAL vector to their plane */
vector<float> crossifier(vector<float> p1, vector<float> p2, vector<float> p3) {
  vector<float> v1(3, 0.0f);
  vector<float> v2(3, 0.0f);
  vector<float> result(3, 0.0f);
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
  return normalizationizerificationator(result);
}

/** Check to see if a point, given a CAMERA, exists within our view */
int isInBounds(const vector<float> p, const Camera cam) {
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

