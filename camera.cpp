#include "camera.h"

/** Define Ray empty constructor */
Ray::Ray():
  startPos(3, 0.0f), 
  dir(3, 0.0f)
{}

/** Define Ray constructor */
Ray::Ray(std::vector<float> start, std::vector<float> direction) {
  startPos = start;
  dir = direction;
}

/** Define Camera empty constructor */
Camera::Camera():
  w(0.0f),
  h(0.0f),
  fov(0.0f),
  d(0.0f),
  ul(3, 0.0f),
  right(3, 0.0f),
  down(3, 0.0f),
  lookFrom(3, 0.0f),
  lookAt(3, 0.0f),
  upDir(3, 0.0f)
{}

/** PARAM *sample and PARAM *ray
 *  Returns a Ray going from Camera location towards the sample,
 *  probably calculated using fov */
Ray Camera::generateRay(Sample &sample) {
  Ray result;
  // Result's ray point
  result.startPos.at(0) = lookFrom.at(0);
  result.startPos.at(1) = lookFrom.at(1);
  result.startPos.at(2) = lookFrom.at(2);
  // Result's ray vector
  result.dir.at(0) = sample.x - lookFrom.at(0);
  result.dir.at(1) = sample.y - lookFrom.at(1);
  result.dir.at(2) = sample.z - lookFrom.at(2);
  normalizationizerificationator(result.dir);
  return result;
}

//3D sample with (x, y, z)
Sample::Sample(float _x, float _y, float _z):
  //The coordinates of the sample (x, y, z):
  x(_x),
  y(_y),
  z(_z),

  // FINAL rgb value of this sample (to be passed to Film)
  rgb(3, 0.0f)
{}

/** Normalizes a vector. */
void normalizationizerificationator(std::vector<float> &vec) {
  float mag = sqrt(pow(vec.at(0), 2.0) + pow(vec.at(1), 2.0) + 
		   pow(vec.at(2), 2.0));
  vec.at(0) = vec.at(0)/mag;
  vec.at(1) = vec.at(1)/mag;
  vec.at(2) = vec.at(2)/mag;
}

/** Given a RAY, normalizes its vector segment. */
void normalizationizerificationator(Ray &ray) {
  float mag = sqrt(pow(ray.dir.at(0), 2.0) + pow(ray.dir.at(1), 2.0) + 
		   pow(ray.dir.at(2), 2.0));
  ray.dir.at(0) = ray.dir.at(0)/mag;
  ray.dir.at(1) = ray.dir.at(1)/mag;
  ray.dir.at(2) = ray.dir.at(2)/mag;
}

/** Given 2 three POINTs, return the NORMAL vector to their plane */
std::vector<float> crossifier(const std::vector<float> &p1, 
			      const std::vector<float> &p2,
			      const std::vector<float> &p3) {
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
  // use overloaded method
  return crossifier(v1, v2);
}

/** Given 2 VECTORS, return the NORMAL vector to their plane (v1 cross v2)*/
std::vector<float> crossifier(const std::vector<float> &v1, 
			      const std::vector<float> &v2) {
  std::vector<float> result(3, 0.0f);  
  result.at(0) = v1.at(1)*v2.at(2) - v1.at(2)*v2.at(1);
  result.at(1) = v1.at(2)*v2.at(0) - v1.at(0)*v2.at(2);
  result.at(2) = v1.at(0)*v2.at(1) - v1.at(1)*v2.at(0);
  normalizationizerificationator(result);
  return result;
}

/** Given 2 VECTORS, return v1 dot v2 */
float dot(std::vector<float> v1, std::vector<float> v2) {
  return v1.at(0) * v2.at(0) + v1.at(1) * v2.at(1) + v1.at(2) * v2.at(2);
}

/** Set d in a camera */
void Camera::setD() {
  d = h / (2*tan((fov/2)*(3.14159265/180)));
}

/** Given a camera with defined upDir and d, set its UL field, RIGHT, and DOWN */
void Camera::setULRD() {
  lookAt.at(0) = lookAt.at(0) - lookFrom.at(0);
  lookAt.at(1) = lookAt.at(1) - lookFrom.at(1);
  lookAt.at(2) = lookAt.at(2) - lookFrom.at(2);
  normalizationizerificationator(lookAt);

  // Set RIGHT and DOWN
  right = crossifier(lookAt, upDir);
  down = crossifier(lookAt, right);
  normalizationizerificationator(down);

  // Set UL
  for (int i = 0; i < 3; i++) {
    ul.at(i) = lookFrom.at(i) + lookAt.at(i) * d;
    ul.at(i) = ul.at(i) - right.at(i) * (w/2);
    ul.at(i) = ul.at(i) - down.at(i) * (h/2);
  }
}


