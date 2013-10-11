#include "camera.h"

/** Prepare Ray point and vector */
Camera::Ray():
  pos(3, 0.0f), 
  dir(3, 0.0f)
{}

/** Prepare Camera vectors */
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
  Ray result();
  // Result's ray point
  result.pos.at(0) = lookFrom.at(0);
  result.pos.at(1) = lookFrom.at(1);
  result.pos.at(2) = lookFrom.at(2);
  // Result's ray vector
  result.dir.at(0) = sample->x - lookFrom.at(0);
  result.dir.at(1) = sample->y - lookFrom.at(1);
  result.dir.at(2) = sample->z - lookFrom.at(2);
  
}
