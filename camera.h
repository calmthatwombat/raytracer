#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>


/** Forward Declarations */

/** Class Declarations */
class Ray;
class Camera;
class Sample;

/** Represents the ray RAY(t) = POS + t*DIR, where tMin <= t <= tMax */
class Ray {
 public:
  /* Empty constructor does nothing */
  Ray();
  /* Constructor initializing startPos and dir */
  Ray(std::vector<float> start, std::vector<float> direction);
  std::vector<float> startPos;
  std::vector<float> dir;
};

class Camera {
 public:
  /* Constructor taking in width, height, and fov */
  Camera();
  // Window size
  float w, h;
  // Window fov
  float fov;
  // Orthogonal distance
  float d;
  // UL corner
  std::vector<float> ul;
  // DIRECTIONS for incremental sampling. NOTE: capital letter indicates POSITIVE.
  // must be x, X, y, Y, z, Z
  char right;
  char down;
  // Camera properties
  std::vector<float> lookFrom;
  std::vector<float> lookAt;
  std::vector<float> upDir;
  /* Returns a RAY generated from pointing the camera to the SAMPLE */
  Ray generateRay(Sample &sample);

  /** Given a camera with defined upDir and d, set its UL field, RIGHT, and DOWN */
  void setULRD();

  /** Set d in a camera */
  void setD();
};

//3D sample represented by (x, y, z)
/**
 * Represents a 3D Sample on the window (x, y, z) <- this was Kevin's class description*/
class Sample {
 public:
  Sample(float x, float y, float z);
 	
  //The coordinates of the sample (x, y, z):
  float x, y, z;

  //The rgb value of this sample (to be passed to Film)
  std::vector<float> rgb;
};

/** Method Declarations */

/** Method that normalizes a ray */
void normalizationizerificationator(Ray &ray);

/** Method that normalizes a vector */
void normalizationizerificationator(std::vector<float> &vec);


#endif
