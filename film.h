#ifndef FILM_H
#define FILM_H

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include "./FreeImage/FreeImage.h"

/** Forward Declarations */

/** Class Declarations */
class Film;

class Film {
 public:
  Film(int, int);
  FIBITMAP* bitmap;
  RGBQUAD color;
  int width, height; // passed in by scene
  void setPixel(int x, int y, std::vector<float> rgb);
  void outputImage(std::string outputFileName);

};


#endif /* FILM_H */

