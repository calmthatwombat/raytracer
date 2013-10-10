#ifndef CAMERA_H
#define CAMERA_H

#include 

class Camera;
class Sample;
class Ray;

class Camera {
 public:
  vector<float> lookFrom;
  vector<float> lookAt;
  vector<float> upDir;
  float fovVert;
  float fovHori;
  Ray generateRay(Sample *sample, Ray *ray);
}

#endif
