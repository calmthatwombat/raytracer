#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>

/** Forward Declarations */
class Camera;

/** Scene class, contains basically everything */
class Scene{
 public:
  Camera camera;
  int width, height;
  int maxDepth;
  std::string outputFile;
  Scene();
}

#endif
