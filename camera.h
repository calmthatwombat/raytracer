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

//represents the ray ray(t) = pos + t*dir, where t_min <= t <= t_max
class Ray{
	public:
	vector<float> pos;
	vector<float> dir;
	float t_min;
	float t_max;
}

#endif
