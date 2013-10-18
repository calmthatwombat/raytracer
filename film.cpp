#include "film.h"

#define BPP 24 //since we're outputting three 8 bit RGB values

/** Film constructor receives image plane width & height from scene */
Film::Film(int _width, int _height) {
  width = _width;
  height = _height;
  bitmap = FreeImage_Allocate(width, height, BPP);
  //might possibly need to set color.rgbREd, color.rgbBlue...to 0?
  FreeImage_Initialise();
}

/* sets a pixel (x,y), and saves it into the corresponding position on the bitmap. 
 * @PARAM: Takes in the x coordinate of the sample, and the y coordinate of the sample.
   @PARAM: RGB color of the pixel to be saved in bitmap. */
void Film::setPixel(int x, int y, std::vector<float> rgb){
  //FreeImage_Initialise();
  if(!bitmap) {
    std::cout << "No bitmap!" << std::endl;
    exit(1);
  }

  color.rgbRed = (double) rgb.at(0) * 255;
  color.rgbGreen = (double) rgb.at(1) * 255;
  color.rgbBlue = (double) rgb.at(2) * 255;

  FreeImage_SetPixelColor(bitmap, x, y, &color);//passing a pointer to the color struct

}

/*outputs the final image.
 * @PARAM: given the outputFileName, outputs a PNG image with that outputFileName. */
void Film::outputImage(std::string outputFileName){
  // convert outputFileName to char*
  /*
    char *output = new char[outputFileName.size() + 1];
  output[outputFileName.size()] = 0;
  memcpy(output, outputFileName.c_str(), outputFileName.size());*/
  
  if (FreeImage_Save(FIF_PNG, bitmap, outputFileName.c_str(), 0)) //outputting the file
    std::cout << "Image successfully saved!!!" << std::endl;

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

//}
