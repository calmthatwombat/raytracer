#include "camera.h"

Camera::Camera():
  lookFrom(3, 0.0f),
  lookAt(3, 0.0f),
  upDir(3, 0.0f)
{}

/**
 * PARAM *sample and PARAM *ray
 * Returns a Ray going from Camera location towards the sample,
 * probably calculated using fov */
Ray Camera::generateRay(Sample *sample) {
  
  //from class def: Create a ray starting from the camera that passes through the corresponding pixel (sample.x, sample.y) on the image plane
}
