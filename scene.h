#ifndef SCENE_H
#define SCENE_H

#include "sampler.h"

#include <sstream> //included this for loadscene

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <string>

/** Class Declarationss */

//NOTES: RENDER METHOD SHOULD BE CALLED AT SOME POINT IN SCENE.CPP MAIN (probably after all the parsing is done)
/* METHOD DESCRIPTION: begins for-looping through each pixel in window of size width * height.
   This method will generate a ray that will shoot to each pixel in the window, and get the
   RGB value to be passed to Film.*/
/* PARAM width, height : width and height of image plane
   PARAM depth : the depth of recursion
   PARAM outputFileName : the output file name that film needs to create the PNG output file.
   PARAM Sampler : needs Sampler object to get access to a Sample and the Camera */
void render(int width, int height, int depth, std::string outputFileName, 
	    Sampler &sampler);




#endif
