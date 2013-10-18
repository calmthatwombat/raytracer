#ifndef SAMPLER_H
#define SAMPLER_H

#include "camera.h"
#include "raytracer.h"
#include "film.h"

/** Forward Declarations */
/**class Camera;
   class Raytracer;
   class Film;*/

/** Class Declarations */
class Sampler;

class Sampler {
 public:
  /* Constructor that creates Sampler object with a Camera, a Raytracer, and a Film. */
  Sampler(Camera &_camera, RayTracer &_raytracer, Film &_film);
  
  /* Reference to the Camera, Raytracer, and Film */
  Camera camera;
  RayTracer raytracer;
  Film film;

};


#endif
