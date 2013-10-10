#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>

#endif

class Scene{
	public:
		//blah


}

//represents the ray ray(t) = pos + t*dir, where t_min <= t <= t_max
class Ray{
	public:
	vector<float> pos;
	vector<float> dir;
	float t_min;
	float t_max;
}
