#ifndef CAMERA_H
#define CAMERA_H

#include

/** Forward Declarations */
class Sample;

/** Class Declarations */
class Ray;
class Camera;

/** Represents the ray RAY(t) = POS + t*DIR, where tMin <= t <= tMax */
Ray {
 public:
  vector<float> pos;
  vector<float> dir;
  float tMin;
  float tMax;
}

Camera {
 public:
  vector<float> lookFrom;
  vector<float> lookAt;
  vector<float> upDir;
  
  Ray generateRay(, Ray *ray);
}

#endif
