#include "film.h"
#include "scene.h"
#include "FreeImage.h"

#define BPP 24 //since we're outputting three 8 bit RGB values

using namespace std;

int main(int argc, char *argv[]){
  scene s;
  //saving the width and height of the image
  int width = scene.width; ////////should they be floats? or ints okay?
  int height = scene.height;
  FreeImage_Initialise();

  FIBITMAP* bitmap = FreeImage_Allocate(width, height, BPP);
  RGBQUAD color;

  if(!bitmap)
    exit(1);
  //Draws a gradient from blue to green
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) {
      /*for now, this is painting a gradient. but we will later need assign the write colors:*/
      color.rgbRed = 0;///////
      color.rgbGreen = (double) i/width * 255.0;/////
      color.rgbBlue = (double) j/height * 255.0;/////

      FreeImage_SetPixelColor(bitmap, i, j, &color);//passing a pointer to the color struct
    }
  }
  
  if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0))
    cout << "Image successfully saved!!!" <<endl;

  FreeImage_DeInitialise(); //cleanup.

}
