#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <math.h>

/** Forward Declarations */
class Sample;

/** Class Declarations */
class Ray;
class Camera;

/** Represents the ray RAY(t) = POS + t*DIR, where tMin <= t <= tMax */
class Ray {
 public:
  /* Empty constructor does nothing */
  Ray();
  /* Constructor initializing startPos and dir */
  Ray(vector<float> start, vector<float> direction);
  vector<float> startPos;
  vector<float> dir;
};

class Camera {
 public:
  /* Constructor taking in width, height, and fov */
  Camera(float x, float y, float fov);
  // Window size
  float w, h;
  // Orthogonal distance
  float d;
  // UL corner
  vector<float> ul;
  // DIRECTIONS for incremental sampling. NOTE: capital letter indicates POSITIVE.
  // must be x, X, y, Y, z, Z
  char right;
  char down;
  // Camera properties
  vector<float> lookFrom;
  vector<float> lookAt;
  vector<float> upDir;
  /* Returns a RAY generated from pointing the camera to the SAMPLE */
  Ray generateRay(Sample &sample);
};

/** Method Declarations */
/** Given a camera with defined upDir and d, set its UL field, RIGHT, and DOWN */
void setULRD(Camera cam);


#endif
