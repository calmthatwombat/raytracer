#include "film.h"
#include "FreeImage.h"

#define BPP 24 //since we're outputting three 8 bit RGB values

using namespace std;

// scene s;
// int width = s.width;
// int height = s.height;
//string outputFile = s.outputFile;

// FIBITMAP* bitmap = FreeImage_Allocate();
// RGBQUAD color;


/** Film constructor receives image plane width/height from scene */
Film::Film(int _width, int _height) {
  width = _width;
  height = _height;
  bitmap = FreeImage_Allocate(width, height, BPP);
}


/* sets a pixel (x,y), and saves it into the corresponding position on the bitmap. 
 * @PARAM: Takes in the x coordinate of the sample, and the y coordinate of the sample.*/
void Film::setPixel(float x, float y){
  FreeImage_Initialise();
  if(!bitmap)
    exit(1);

  color.rgbRed = 0;///////
  color.rgbGreen = (double) x * 255.0;/////
  color.rgbBlue = (double) y * 255.0;/////

  FreeImage_SetPixelColor(bitmap, x, y, &color);//passing a pointer to the color struct

}

/*outputs the final image.
 * @PARAM: given the outputFileName, outputs a PNG image with that outputFileName. */
void Film::outputImage(string outputFileName){
  if (FreeImage_Save(FIF_PNG, bitmap, outputFileName, 0)) //outputting the file
    cout << "Image successfully saved!!!" <<endl;

  FreeImage_DeInitialise(); //cleanup.
}


//original code:

// int main(int argc, char *argv[]){
//   scene s;
//   //saving the width and height of the image
//   int width = s.width; ////////should they be floats? or ints okay?
//   int height = s.height;
//   FreeImage_Initialise();

//   FIBITMAP* bitmap = FreeImage_Allocate(width, height, BPP);
//   RGBQUAD color;

//   if(!bitmap)
//     exit(1);
//   //Draws a gradient from blue to green
//   for (int i=0; i<width; i++) {
//     for (int j=0; j<height; j++) {
//       //for now, this is painting a gradient. but we will later need assign the write colors:
//       color.rgbRed = 0;///////
//       color.rgbGreen = (double) i/width * 255.0;/////
//       color.rgbBlue = (double) j/height * 255.0;/////

//       FreeImage_SetPixelColor(bitmap, i, j, &color);//passing a pointer to the color struct
//     }
//   }
  
//   if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0))
//     cout << "Image successfully saved!!!" <<endl;

//   FreeImage_DeInitialise(); //cleanup.

}
