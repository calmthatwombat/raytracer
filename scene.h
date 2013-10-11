#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>

/** Forward Declarations */
class Camera;

/** Scence class, contains basically everything */
class Scene{
 public:
  Camera camera;
  int width, height;
  Scene();
}

/** Represents the ray RAY(t) = POS + t*DIR, where tMin <= t <= tMax */
class Ray{
 public:
  vector<float> pos;
  vector<float> dir;
  float tMin;
  float tMax;
}

#endif
