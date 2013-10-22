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

/** Finds the vector from pt1 to pt2 */
std::vector<float> vectorizer(std::vector<float> pt1, std::vector<float> pt2) { // going from pt1 to pt2
  std::vector<float> result (3, 0.0f);
  result.at(0) = pt2.at(0)-pt1.at(0);
  result.at(1) = pt2.at(1)-pt1.at(1);
  result.at(2) = pt2.at(2)-pt1.at(2);
  normalizationizerificationator(result);
  return result;
}

/** After finding an intersection, determine reflection ray at pos from input ray */
Ray createReflectRay(Ray &incRay, Intersection &inters){
  float dp = dot(incRay.dir, inters.normal);
  std::vector<float> result(3, 0.0f);
  result.at(0) = incRay.dir.at(0) - 2*dp*inters.normal.at(0);
  result.at(1) = incRay.dir.at(1) - 2*dp*inters.normal.at(1);
  result.at(2) = incRay.dir.at(2) - 2*dp*inters.normal.at(2);
  normalizationizerificationator(result);
  return Ray(inters.pos, result);
}

/** Find reflect vector (not Ray) */
std::vector<float> getRefVector(std::vector<float> lightDir, 
			     std::vector<float> surfNorm) {
  float dot = (lightDir.at(0)*surfNorm.at(0) + lightDir.at(1)*surfNorm.at(1) + 
	       lightDir.at(2)*surfNorm.at(2));
  std::vector<float> result(3, 0.0f);
  result.at(0) = lightDir.at(0) - 2*dot*surfNorm.at(0);
  result.at(1) = lightDir.at(1) - 2*dot*surfNorm.at(1);
  result.at(2) = lightDir.at(2) - 2*dot*surfNorm.at(2);
  normalizationizerificationator(result);
  return result;
}

/** Get negative vector */
std::vector<float> getNegative(std::vector<float> v) {
  std::vector<float> result(3, 0.0f);
  result.at(0) = -v.at(0);
  result.at(1) = -v.at(1);
  result.at(2) = -v.at(2);
  return result;
}

// Ambient function
std::vector<float> ambientify(std::vector<float> acolor, std::vector<float> icolor) {
  std::vector<float> result(3, 0.0f);
  result.at(0) = acolor.at(0) * icolor.at(0);
  result.at(1) = acolor.at(1) * icolor.at(1);
  result.at(2) = acolor.at(2) * icolor.at(2);
  return result;
}

/** 
 * Diffuse function
 * returns Rd, diffuse RGB value */
std::vector<float> diffusify(std::vector<float> dcolor, std::vector<float> icolor, 
			     std::vector<float> lightDir, 
			     std::vector<float> surfaceNormal)  {
  // negative scalar due to the way we are interpreting our light rays
  float scalar = std::max(-lightDir.at(0)*surfaceNormal.at(0) +
			  -lightDir.at(1)*surfaceNormal.at(1) + 
			  -lightDir.at(2)*surfaceNormal.at(2),
			  0.0f);
  std::vector<float> result(3, 0.0f);
  result.at(0) = scalar*dcolor.at(0)* icolor.at(0);
  result.at(1) = scalar*dcolor.at(1)* icolor.at(1);
  result.at(2) = scalar*dcolor.at(2)* icolor.at(2);
  return result;
}

/** 
 * Specular function
 * reflectedRay found using PARAMS lightDir and PARAMS surfaceNorm
 * input vectors MUST be normalized first
 * returns Rs, specular RGB value */
std::vector<float> specularify(std::vector<float> scolor, std::vector<float> icolor, 
			       std::vector<float> lightDir, 
			       std::vector<float> surfaceNorm,
			       std::vector<float> dirToViewer, float pCoeff)  { 
  
  // find reflectedRay
  std::vector<float> reflectRay(3, 0.0f);
  reflectRay = getRefVector(lightDir, surfaceNorm); // getRefRay already normalizes.

  float scalar = std::max(-reflectRay.at(0)*dirToViewer.at(0) + 
			  -reflectRay.at(1)*dirToViewer.at(1) + 
			  -reflectRay.at(2)*dirToViewer.at(2),
			  0.0f);
  scalar = pow(scalar, pCoeff);

  std::vector<float> result(3, 0.0f);
  result.at(0) = scalar*scolor.at(0)* icolor.at(0);
  result.at(1) = scalar*scolor.at(1)* icolor.at(1);
  result.at(2) = scalar*scolor.at(2)* icolor.at(2);
  return result;
}

/** 
 * Combine and cap RGB values to find resultant RGB
 * PARAM rgbs : array of the RGBs
 * PARAM num : length of rgbs array (# of RGBs to be combined) */
std::vector<float> shAverager(std::vector<float> rgbs[], int num) {
  std::vector<float> result(3, 0.0f);
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  int i;
  for (i = 0; i < num; i++) {
    r = r + rgbs[i].at(0);
    g = g + rgbs[i].at(1);
    b = b + rgbs[i].at(2);
  }
  // Cap when overflow
  if (r > 1.0f)
    r = 1.0f;
  if (g > 1.0f)
    g = 1.0f;
  if (b > 1.0f)
    b = 1.0f;
  result.at(0) = r;
  result.at(1) = g;
  result.at(2) = b;
  return result;
}

/** Trace given RAY recursively to return the resultant RGB color */
std::vector<float> RayTracer::trace(Ray &outRay, int depth) {
  outRay.startPos.at(0) += outRay.dir.at(0) * 0.001;
  outRay.startPos.at(1) += outRay.dir.at(1) * 0.001;
  outRay.startPos.at(2) += outRay.dir.at(2) * 0.001;
  std::vector<float> black(3, 0.0f);
  std::vector<float> red(3, 0.0f);
  red.at(0) = 1.0f;
  std::vector<Intersection> intersections;
  Intersection inters;
  std::vector<Shape *>::iterator sh = shapes->begin();
  for (int i = 0; sh != shapes->end(); ++sh, i++) {
    
    
    if ((*sh)->tri != NULL) {
      
      
      inters = (*sh)->tri->intersect(outRay, *this);
      if (inters.isExists) {

	  
	intersections.push_back(inters);
      }
    } else if ((*sh)->sph != NULL) {
      inters = (*sh)->sph->intersect(outRay, *this);
      if (inters.isExists)
	intersections.push_back(inters);
    } else if ((*sh)->tn != NULL) {
      inters = (*sh)->tn->intersect(outRay, *this);
      if (inters.isExists)
	intersections.push_back(inters);
    } else {
      printf("No type of shape?");
    }
  }
  float closestT = -1;
  Intersection firstHit;
  // Find firsthit
  for (unsigned int i = 0; i < intersections.size(); i++) {
    // Self-shadowing handled here
    if (intersections[i].isExists) {
      if (closestT < 0.0f || intersections[i].t < closestT) {
	closestT = intersections[i].t;
	firstHit = intersections[i];
      }
    }
  }
  // No intersections
  if (closestT == -1) {
    return black;
  }
  
  // We now know exact Intersection.
  // Create rgb array (+2 for emission and reflection)
  std::vector<float> rgbs[DLs->size() + PLs->size() + 2];

  // Look through DLs
  for (unsigned int i = 0; i < DLs->size(); i++) {
    Intersection block;
    int blocked = 0;
    Ray cRay(firstHit.pos, getNegative(DLs->at(i)->lightDir));
    // Check if collision is on correct side
    if (dot(cRay.dir, firstHit.normal) <= 0.0f)
      blocked = 1;
    std::vector<Shape *>::iterator sh = shapes->begin();
    // Search for obstructions (self-shadowing handled here)
    // Self-shadowing handled by disallowing obstruction to be itself
    for (int j = 0; sh != shapes->end(); ++sh, j++) {
      if (blocked)
	break;
      if ((*sh)->tri != NULL) {
	block = (*sh)->tri->intersect(cRay, *this);
	if (block.isExists) {
	  if (block.sh != firstHit.sh) {
	    blocked = 1;
	  }
	}
      } else if ((*sh)->sph != NULL) {
	block = (*sh)->sph->intersect(cRay, *this);
	if (block.isExists) {
	  if (block.sh != firstHit.sh) {
	    blocked = 1;
	  }
	}
      } else if ((*sh)->tn != NULL) {
	block = (*sh)->tn->intersect(cRay, *this);
	if (block.isExists) {
	  if (block.sh != firstHit.sh) {
	    blocked = 1;
	  }
	}
      } else {
	printf("No type of shape?");
      }
    }
    // Collision handling
    if (!blocked) {
      std::vector<float> dlresult[3];
      dlresult[0] = ambientify(firstHit.sh->brdf.ka, DLs->at(i)->rgb);
      dlresult[1] = diffusify(firstHit.sh->brdf.kd, DLs->at(i)->rgb,
			      DLs->at(i)->lightDir, firstHit.normal);
      dlresult[2] = specularify(firstHit.sh->brdf.ks, DLs->at(i)->rgb, 
				DLs->at(i)->lightDir, firstHit.normal,
				getNegative(outRay.dir), firstHit.sh->brdf.s);
      rgbs[i] = shAverager(dlresult, 3);
    } else {
      rgbs[i] = black;
    }
  }
  // Loop through PLs
  for (unsigned int i = 0; i < PLs->size(); i++) {
    Intersection block;
    int blocked = 0;
    float exactT = 0.0f;
    Ray cRay(firstHit.pos, vectorizer(firstHit.pos, PLs->at(i)->pos));
    // Check if collision is on correct side
    if (dot(cRay.dir, firstHit.normal) <= 0.0f)
      blocked = 1;
    // Make a ray that goes from PL to p, which is backwards
    Ray backwardscRay(PLs->at(i)->pos, vectorizer(PLs->at(i)->pos, firstHit.pos));
    std::vector<Intersection> intersections1;
    Intersection inters1;
    sh = shapes->begin();
    // Search for collisions
    for (int j = 0; sh != shapes->end(); ++sh, j++) {
      if (blocked)
	break;
      if ((*sh)->tri != NULL) {
	inters1 = (*sh)->tri->intersect(backwardscRay, *this);
	if (inters1.isExists) {
	  intersections1.push_back(inters1);
	  if (inters1.sh == firstHit.sh)
	    exactT = inters1.t;
	}
      } else if ((*sh)->sph != NULL) {
	inters1 = (*sh)->sph->intersect(backwardscRay, *this);
	if (inters1.isExists) {
	  intersections1.push_back(inters1);
	  if (inters1.sh == firstHit.sh)
	    exactT = inters1.t;
	}
      } else if ((*sh)->tn != NULL) {
	inters1 = (*sh)->tn->intersect(backwardscRay, *this);
	if (inters1.isExists) {
	  intersections1.push_back(inters1);
	  if (inters1.sh == firstHit.sh)
	    exactT = inters1.t;
	}
      } else {
	printf("No type of shape?");
      }
    }
    for (unsigned int j = 0; !blocked && j < intersections1.size(); j++) {
      // Find closest intersection (overlap handling is needed for PLs)
      // Handle overlaps by allowing exact to take priority
      if (intersections1[i].isExists) {
	if (intersections1[i].t < exactT) {
	  rgbs[DLs->size() + i] = black;
	  blocked = 1;
	  break;
	}
      }
    }
    // Collision handling
    if (!blocked) {
      std::vector<float> plresult[3];
      plresult[0] = ambientify(firstHit.sh->brdf.ka, PLs->at(i)->rgb);
      plresult[1] = diffusify(firstHit.sh->brdf.kd, PLs->at(i)->rgb,
			      vectorizer(PLs->at(i)->pos, firstHit.pos), 
			      firstHit.normal);
      plresult[2] = specularify(firstHit.sh->brdf.ks, PLs->at(i)->rgb, 
				vectorizer(PLs->at(i)->pos, firstHit.pos), 
				firstHit.normal,
				getNegative(outRay.dir), firstHit.sh->brdf.s);
      rgbs[DLs->size() + i] = shAverager(plresult, 3);
    }
  }
  std::vector<float> ke(3, 0.0f);
  ke.at(0) = firstHit.sh->brdf.ke.at(0);
  ke.at(1) = firstHit.sh->brdf.ke.at(1);
  ke.at(2) = firstHit.sh->brdf.ke.at(2);
  rgbs[DLs->size() + PLs->size()] = ke;
  Ray recRay = createReflectRay(outRay, firstHit);
  if (depth > 0) {
    // Determine reflection
    if (!firstHit.sh->brdf.s) {
      // No reflection necessary
      rgbs[DLs->size() + PLs->size() + 1] = black;
    } else {
      std::vector<float> icolor = trace(recRay, depth - 1);
      rgbs[DLs->size() + 
	   PLs->size() + 
	   1] = specularify(firstHit.sh->brdf.ks, icolor, 
			    getNegative(recRay.dir), firstHit.normal,
			    getNegative(outRay.dir), firstHit.sh->brdf.s);
    }
  } else {
    rgbs[DLs->size() + PLs->size() + 1] = black;
  }
  return shAverager(rgbs, PLs->size() + DLs->size() + 2);
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
PointLight::PointLight(float x, float y, float z, float r, float g, float b, float a1,
		       float a2, float a3):
  pos(3, 0.0f),
  rgb(3, 0.0f),
  att1(a1),
  att2(a2),
  att3(a3)
{
  pos.at(0) = x;
  pos.at(1) = y;
  pos.at(2) = z;
  rgb.at(0) = r;
  rgb.at(1) = g;
  rgb.at(2) = b;
}

/** Shape constructor */
Shape::Shape(unsigned int n) {
  num = n;
  tri = NULL;
  sph = NULL;
  tn = NULL;
}

/** Sphere constructor */
Sphere::Sphere(){}
Sphere::Sphere(float x, float y, float z, float rad, BRDF b, unsigned int n, mat4 transformMatrix):
  center(3, 0.0f),
  num(n)
{
  center.at(0) = x;
  center.at(1) = y;
  center.at(2) = z;
  radius = rad;
  brdf = b;
  transMat = transformMatrix;//matrix used to keep track of transformation
  inverse = transMat.inverse();//getting the inverse of the transformation inverse (used for intersections)
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
  if (x0 <= 0.0f && x1 <= 0.0f) {
    return 0;
  } else if (x0 <= 0.0f) {
    std::swap(x0, x1);
  } else {
    if (x0 > x1 && x1 > 0.0f) std::swap(x0, x1);
  }
  return 1;
}

/** Given an incoming ray, returns NULL if no intersection, otherwise returns
 *  resultant intersection */
Intersection Sphere::intersect(Ray &ray, RayTracer &rt) {


  //////stuff needed for transformation:
  
  vec3 rayStartPos = convertToVec3(ray.startPos);
  vec3 rayDir = convertToVec3(ray.dir);

  //getting the transformed ray position and transformed direction
  vec3 transRayPos = vec3(inverse * vec4(rayStartPos, 1));
  vec3 transRayDir = vec3(inverse * vec4(rayDir, 0), 3).normalize();

  //The ray with the transformation applied:
  Ray transRay = Ray(convertToVectorFloat(transRayPos), convertToVectorFloat(transRayDir));


  //////////////////original below:

  // Roots of intersection
  float t0 = 0.0f;
  float t1 = 0.0f;
  // emc: e minus c (ray's startPos - sphere center)
  std::vector<float> emc(3, 0.0f);
  emc.at(0) = transRay.startPos.at(0) - center.at(0);
  emc.at(1) = transRay.startPos.at(1) - center.at(1);
  emc.at(2) = transRay.startPos.at(2) - center.at(2);
  // a: d dot d     (ray's dir dotproduct itself) EQUALS 1 for unit d
  float a = (transRay.dir.at(0)*transRay.dir.at(0) + transRay.dir.at(1)*transRay.dir.at(1) + 
	     transRay.dir.at(2)*transRay.dir.at(2));
  // b: 2* (d dot ((e-c)dot(e-c)))    (2 times (d dot (emc dotproduct itself)))
  float b = 2 * (transRay.dir.at(0)*emc.at(0) + transRay.dir.at(1)*emc.at(1) +
		 transRay.dir.at(2)*emc.at(2));
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
  inters.sh = rt.shapes->at(this->num);
  /////////// more stuff for transformation:
  vec3 transfRayStartPos = convertToVec3(transRay.startPos);
  vec3 transfRayDir = convertToVec3(transRay.dir);
  vec3 intersPos = transfRayStartPos + t0 * transfRayDir;
  intersPos = vec3(transMat * vec4(intersPos));

  //setting inters.pos to the intersPos we found above, after the transformation:
  inters.pos = convertToVectorFloat(intersPos);

  //Note: to calculate normal of an ellipsoid, transform normal by m.inverse.transpose (Shirley)
  //vec3 vec3Center = convertToVec3(center); //deleted
  vec3 normal = vec3(inverse * intersPos) - convertToVec3(center);
  // note, this is an algebra3 constructor: vec3(const vec4& v, int dropAxis); <- casts vec4 to vec3
  normal = vec3(inverse.transpose() * vec4(normal, 0), 3);
  normal.normalize();

  //setting inters.normal to the normal calculated above:
  inters.normal = convertToVectorFloat(normal);

  // By this point, intersection is then valid.
  return inters;

  //originally:
  // inters.pos.at(0) = transRay.startPos.at(0) + t0*transRay.dir.at(0);
  // inters.pos.at(1) = transRay.startPos.at(1) + t0*transRay.dir.at(1);
  // inters.pos.at(2) = transRay.startPos.at(2) + t0*transRay.dir.at(2);
  // inters.normal.at(0) = inters.pos.at(0) - center.at(0);
  // inters.normal.at(1) = inters.pos.at(1) - center.at(1);
  // inters.normal.at(2) = inters.pos.at(2) - center.at(2);
  // normalizationizerificationator(inters.normal);
  // return inters;



  //kevin's original code:
  // // Roots of intersection
  // float t0 = 0.0f;
  // float t1 = 0.0f;
  // // emc: e minus c (ray's startPos - sphere center)
  // std::vector<float> emc(3, 0.0f);
  // emc.at(0) = ray.startPos.at(0) - center.at(0);
  // emc.at(1) = ray.startPos.at(1) - center.at(1);
  // emc.at(2) = ray.startPos.at(2) - center.at(2);
  // // a: d dot d     (ray's dir dotproduct itself) EQUALS 1 for unit d
  // float a = (ray.dir.at(0)*ray.dir.at(0) + ray.dir.at(1)*ray.dir.at(1) + 
  //      ray.dir.at(2)*ray.dir.at(2));
  // // b: 2* (d dot ((e-c)dot(e-c)))    (2 times (d dot (emc dotproduct itself)))
  // float b = 2 * (ray.dir.at(0)*emc.at(0) + ray.dir.at(1)*emc.at(1) +
  //    ray.dir.at(2)*emc.at(2));
  // // c: emc dot emc - r^2        (emc dot emc - sphere's radius^2)
  // float c = (emc.at(0)*emc.at(0) + emc.at(1)*emc.at(1) + emc.at(2)*emc.at(2) -
  //      radius*radius);
  // // Initialize intersection, currently isExists == 1
  // Intersection inters;
  // if (!quadratic(a, b, c, t0, t1)) {
  //   // SET isExists = 0
  //   inters.isExists = 0;
  //   return inters;
  // }
  // inters.t = t0;
  // inters.sh = rt.shapes->at(this->num);
  // inters.pos.at(0) = ray.startPos.at(0) + t0*ray.dir.at(0);
  // inters.pos.at(1) = ray.startPos.at(1) + t0*ray.dir.at(1);
  // inters.pos.at(2) = ray.startPos.at(2) + t0*ray.dir.at(2);
  // inters.normal.at(0) = inters.pos.at(0) - center.at(0);
  // inters.normal.at(1) = inters.pos.at(1) - center.at(1);
  // inters.normal.at(2) = inters.pos.at(2) - center.at(2);
  // normalizationizerificationator(inters.normal);
  // // By this point, intersection is then valid.
  // return inters;
}

vec3 convertToVec3(std::vector<float> v) {
  return vec3(v.at(0), v.at(1), v.at(2));
}

std::vector<float> convertToVectorFloat(vec3 v) {
  std::vector<float> result(3, 0.0f);
  result.at(0) = v[0];
  result.at(1) = v[1];
  result.at(2) = v[2];
  return result;
} 

/** Triangle constructor */
Triangle::Triangle(){}
Triangle::Triangle(std::vector<float> *p0, std::vector<float> *p1, 
		   std::vector<float> *p2, BRDF b, unsigned int n) {
  v0 = p0;
  v1 = p1;
  v2 = p2;
  brdf = b;
  num = n;
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

  //checking if the triangle is already on or behind the ray:
  if (t <= 0.0f) {
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

  //At this point, we know the intersection point p we found is the ray-triangle intersection!
  
  inters.sh = rt.shapes->at(this->num);
  inters.t = t;
  inters.normal.at(0) = normal.at(1);
  inters.normal.at(1) = normal.at(1);
  inters.normal.at(2) = normal.at(2);
  normalizationizerificationator(inters.normal);
  // By this point, intersection is then valid.
  return inters;
}

/** TriNormal constructor */
TriNormal::TriNormal(){}
TriNormal::TriNormal(std::vector<float> *p0, std::vector<float> *p1, 
		     std::vector<float> *p2, std::vector<float> *l0,
		     std::vector<float> *l1, std::vector<float> *l2, BRDF b, 
		     unsigned int n) {
  v0 = p0;
  v1 = p1;
  v2 = p2;
  n0 = l0;
  n1 = l1;
  n2 = l2;
  brdf = b;
  num = n;
}

/* Calculate adjusted normal */
std::vector<float> TriNormal::trinormalFinder(Intersection &inters) {
  return std::vector<float>(3, 0.0f);
}

/* Find TriNormal Intersection */
Intersection TriNormal::intersect(Ray &ray, RayTracer &rt) {
  return Intersection();
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


