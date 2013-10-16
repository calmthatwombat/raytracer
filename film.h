#ifndef FILM_H
#ifndef FREEIMAGE_H
#define FILM_H
#define FREEIMAGE_H
#include <iostream>
#include "FreeImage.h"

/** Forward Declarations */
class Scene;

/** Class Declarations */
class Film;

Film{
 public:
  Film(int, int);
  FIBITMAP* bitmap;
  RGBQUAD color;
  int width, height; // passed in by scene
  void Film::setPixel(int x, int y);
  void Film::outputImage(string outputFileName);

}

/** Objct Declarations */
Film film;
Scene *scene;


#endif

