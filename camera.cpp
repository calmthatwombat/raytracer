#include "camera.h"

/** Define Ray empty constructor */
Ray::Ray():
  startPos(3, 0.0f), 
  dir(3, 0.0f)
{}

/** Define Ray constructor */
Ray::Ray(std::vector<float> start, std::vector<float> direction) {
  startPos = start;
  dir = direction;
}

/** Define Camera empty constructor */
Camera::Camera():
  w(0.0f),
  h(0.0f),
  fov(0.0f),
  d(0.0f),
  ul(3, 0.0f),
  right(3, 0.0f),
  down(3, 0.0f),
  lookFrom(3, 0.0f),
  lookAt(3, 0.0f),
  upDir(3, 0.0f)
{}

/** PARAM *sample and PARAM *ray
 *  Returns a Ray going from Camera location towards the sample,
 *  probably calculated using fov */
Ray Camera::generateRay(Sample &sample) {
  Ray result;
  // Result's ray point
  result.startPos.at(0) = lookFrom.at(0);
  result.startPos.at(1) = lookFrom.at(1);
  result.startPos.at(2) = lookFrom.at(2);
  // Result's ray vector
  result.dir.at(0) = sample.x - lookFrom.at(0);
  result.dir.at(1) = sample.y - lookFrom.at(1);
  result.dir.at(2) = sample.z - lookFrom.at(2);
  normalizationizerificationator(result.dir);
  return result;
}

//3D sample with (x, y, z)
Sample::Sample(float _x, float _y, float _z):
  //The coordinates of the sample (x, y, z):
  x(_x),
  y(_y),
  z(_z),

  // FINAL rgb value of this sample (to be passed to Film)
  rgb(3, 0.0f)
{}

/** Normalizes a vector. */
void normalizationizerificationator(std::vector<float> &vec) {
  float mag = sqrt(pow(vec.at(0), 2.0) + pow(vec.at(1), 2.0) + 
		   pow(vec.at(2), 2.0));
  vec.at(0) = vec.at(0)/mag;
  vec.at(1) = vec.at(1)/mag;
  vec.at(2) = vec.at(2)/mag;
}

/** Given a RAY, normalizes its vector segment. */
void normalizationizerificationator(Ray &ray) {
  float mag = sqrt(pow(ray.dir.at(0), 2.0) + pow(ray.dir.at(1), 2.0) + 
		   pow(ray.dir.at(2), 2.0));
  ray.dir.at(0) = ray.dir.at(0)/mag;
  ray.dir.at(1) = ray.dir.at(1)/mag;
  ray.dir.at(2) = ray.dir.at(2)/mag;
}

/** Given 2 three POINTs, return the NORMAL vector to their plane */
std::vector<float> crossifier(const std::vector<float> &p1, 
			      const std::vector<float> &p2,
			      const std::vector<float> &p3) {
  std::vector<float> v1(3, 0.0f);
  std::vector<float> v2(3, 0.0f);
  std::vector<float> result(3, 0.0f);
  // vector1
  v1.at(0) = p2.at(0) - p1.at(0);
  v1.at(1) = p2.at(1) - p1.at(1);
  v1.at(2) = p2.at(2) - p1.at(2);
  // vector2
  v2.at(0) = p3.at(0) - p1.at(0);
  v2.at(1) = p3.at(1) - p1.at(1);
  v2.at(2) = p3.at(2) - p1.at(2);
  // use overloaded method
  return crossifier(v1, v2);
}

/** Given 2 VECTORS, return the NORMAL vector to their plane (v1 cross v2)*/
std::vector<float> crossifier(const std::vector<float> &v1, 
			      const std::vector<float> &v2) {
  std::vector<float> result(3, 0.0f);  
  result.at(0) = v1.at(1)*v2.at(2) - v1.at(2)*v2.at(1);
  result.at(1) = v1.at(0)*v2.at(2) - v1.at(2)*v2.at(0);
  result.at(2) = v1.at(0)*v2.at(1) - v1.at(1)*v2.at(0);
  normalizationizerificationator(result);
  return result;
}

/** Given 2 VECTORS, return v1 dot v2 */
float dot(std::vector<float> v1, std::vector<float> v2) {
  return v1.at(0) * v2.at(0) + v1.at(1) * v2.at(1) + v1.at(2) * v2.at(2);
}

/** Set d in a camera */
void Camera::setD() {
  d = h / (2*tan((fov/2)*(3.14159265/180)));
}

/** Given a camera with defined upDir and d, set its UL field, RIGHT, and DOWN */
void Camera::setULRD() {
  lookAt.at(0) = lookAt.at(0) - lookFrom.at(0);
  lookAt.at(1) = lookAt.at(1) - lookFrom.at(1);
  lookAt.at(2) = lookAt.at(2) - lookFrom.at(2);
  normalizationizerificationator(lookAt);
  
  // Set RIGHT and DOWN
  right = crossifier(lookAt, upDir);
  down.at(0) = upDir.at(0);
  down.at(1) = upDir.at(1);
  down.at(2) = upDir.at(2);
  normalizationizerificationator(down);

  // Set UL
  ul.at(0) = lookFrom.at(0) + 1/
















    
  /**
  std::vector<float> ULPos(3, 0.0f);
  // up: +y
  if (upDir.at(1) == 1) {
    if (lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = lookFrom.at(0) + (w/2);
      ULPos.at(1) = lookFrom.at(1) + (h/2);
      ULPos.at(2) = lookFrom.at(2) + d;
    } else if (lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = lookFrom.at(0) - (w/2);
      ULPos.at(1) = lookFrom.at(1) + (h/2);
      ULPos.at(2) = lookFrom.at(2) - d;
    } else if (lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = lookFrom.at(0) + d;
      ULPos.at(1) = lookFrom.at(1) + (h/2);
      ULPos.at(2) = lookFrom.at(2) - (w/2);	
    } else if (lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = lookFrom.at(0) - d;
      ULPos.at(1) = lookFrom.at(1) + (h/2);
      ULPos.at(2) = lookFrom.at(2) + (w/2);	
    } else {
      std::cout << "ULPOS was not set for +y upDir" << std::endl;
    }
    // up: -y
  } else if (upDir.at(1) == -1) {
    if (lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = lookFrom.at(0) - (w/2);
      ULPos.at(1) = lookFrom.at(1) - (h/2);
      ULPos.at(2) = lookFrom.at(2) + d;
    } else if (lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = lookFrom.at(0) + (w/2);
      ULPos.at(1) = lookFrom.at(1) - (h/2);
      ULPos.at(2) = lookFrom.at(2) - d;
    } else if (lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = lookFrom.at(0) + d;
      ULPos.at(1) = lookFrom.at(1) - (h/2);
      ULPos.at(2) = lookFrom.at(2) + (w/2);	
    } else if (lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = lookFrom.at(0) - d;
      ULPos.at(1) = lookFrom.at(1) - (h/2);
      ULPos.at(2) = lookFrom.at(2) - (w/2);	
    } else {
      std::cout << "ULPOS was not set for -y upDir" << std::endl;
    }
    // up: +x
  } else if (upDir.at(0) == 1) {
    if (lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = lookFrom.at(0) + (h/2);
      ULPos.at(1) = lookFrom.at(1) - (w/2);
      ULPos.at(2) = lookFrom.at(2) + d;
    } else if (lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = lookFrom.at(0) + (h/2);
      ULPos.at(1) = lookFrom.at(1) + (w/2);
      ULPos.at(2) = lookFrom.at(2) - d;
    } else if (lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = lookFrom.at(0) + (h/2);
      ULPos.at(1) = lookFrom.at(1) + d;
      ULPos.at(2) = lookFrom.at(2) + (w/2);
    } else if (lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = lookFrom.at(0) + (h/2);
      ULPos.at(1) = lookFrom.at(1) - d;
      ULPos.at(2) = lookFrom.at(2) - (w/2);
    } else {
      std::cout << "ULPOS was not set for +x upDir" << std::endl;
    }
    // up: -x
  } else if (upDir.at(0) == -1) {
    if (lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = lookFrom.at(0) - (h/2);
      ULPos.at(1) = lookFrom.at(1) + (w/2);
      ULPos.at(2) = lookFrom.at(2) + d;
    } else if (lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = lookFrom.at(0) - (h/2);
      ULPos.at(1) = lookFrom.at(1) - (w/2);
      ULPos.at(2) = lookFrom.at(2) - d;
    } else if (lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = lookFrom.at(0) - (h/2);
      ULPos.at(1) = lookFrom.at(1) + d;
      ULPos.at(2) = lookFrom.at(2) - (w/2);
    } else if (lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = lookFrom.at(0) - (h/2);
      ULPos.at(1) = lookFrom.at(1) - d;
      ULPos.at(2) = lookFrom.at(2) + (w/2);
    } else {
      std::cout << "ULPOS was not set for -x upDir" << std::endl;
    }
    // up: +z
  } else if (upDir.at(2) == 1) {
    if (lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = lookFrom.at(0) + d;
      ULPos.at(1) = lookFrom.at(1) + (w/2);
      ULPos.at(2) = lookFrom.at(2) + (h/2);
    } else if (lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = lookFrom.at(0) - d;
      ULPos.at(1) = lookFrom.at(1) - (w/2);
      ULPos.at(2) = lookFrom.at(2) + (h/2);
    } else if (lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = lookFrom.at(0) - (w/2);
      ULPos.at(1) = lookFrom.at(1) + d;
      ULPos.at(2) = lookFrom.at(2) + (h/2);
    } else if (lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = lookFrom.at(0) + (w/2);
      ULPos.at(1) = lookFrom.at(1) - d;
      ULPos.at(2) = lookFrom.at(2) + (h/2);
    } else {
      std::cout << "ULPOS was not set for +z upDir" << std::endl;
    }
    // up: -z
  } else if (upDir.at(2) == -1) {
    if (lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = lookFrom.at(0) + d;
      ULPos.at(1) = lookFrom.at(1) - (w/2);
      ULPos.at(2) = lookFrom.at(2) - (h/2);
    } else if (lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = lookFrom.at(0) - d;
      ULPos.at(1) = lookFrom.at(1) + (w/2);
      ULPos.at(2) = lookFrom.at(2) - (h/2);
    } else if (lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = lookFrom.at(0) + (w/2);
      ULPos.at(1) = lookFrom.at(1) + d;
      ULPos.at(2) = lookFrom.at(2) - (h/2);
    } else if (lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = lookFrom.at(0) - (w/2);
      ULPos.at(1) = lookFrom.at(1) - d;
      ULPos.at(2) = lookFrom.at(2) - (h/2);
    } else {
      std::cout << "ULPOS was not set for -z upDir" << std::endl;
    }
  } else {
    std::cout << "idk ULPOS was never properly set." << std::endl;
  }
  //Setting the upper left corner of the image plane
  ul = ULPos;
  // note: X = positive and x = negative
  // set DOWN representation char/////////////////////
  if (upDir.at(0) == -1) {
    // up: -x
    down = 'X';
  } else if (upDir.at(0) == 1) {
    // up: x
    down = 'x';
  } else if (upDir.at(1) == -1) {
    // up: -y
    down = 'Y';
  } else if (upDir.at(1) == 1) {
    // up: y
    down = 'y';
  } else if (upDir.at(2) == -1) {
    // up: -z
    down = 'Z';
  } else if (upDir.at(2) == 1) {
    // up: z
    down = 'z';
  } else {
    std::cout << "ERROR: down not set properly." << std::endl;
  }
  //set RIGHT reprentation char//////////////////////////
		
  if (upDir.at(1) == 1) {
    // up: +y
    if (lookAt.at(2) == 1) {
      // dir: +z
      right = 'x';
    } else if (lookAt.at(2) == -1) {
      // dir: -z
      right = 'X';
    } else if (lookAt.at(0) == 1) {
      // dir: +x
      right = 'Z';
    } else if (lookAt.at(0) == -1) {
      // dir: -x
      right = 'z';
    } else {
      std::cout << "ERROR: right was not set for +y upDir" << std::endl;
    }
  } else if (upDir.at(1) == -1) {
    // up: -y
    if (lookAt.at(2) == 1) {
      // dir: +z
      right = 'X';
    } else if (lookAt.at(2) == -1) {
      // dir: -z
      right = 'x';
    } else if (lookAt.at(0) == 1) {
      // dir: +x
      right = 'z';
    } else if (lookAt.at(0) == -1) {
      // dir: -x
      right = 'Z';
    } else {
      std::cout << "ERROR: cam.right was not set for -y upDir" << std::endl;
    }
  } else if (upDir.at(0) == 1) {
    // up: +x
    if (lookAt.at(2) == 1) {
      // dir: +z
      right = 'Y';
    } else if (lookAt.at(2) == -1) {
      // dir: -z
      right = 'y';
    } else if (lookAt.at(1) == 1) {
      // dir: +y
      right = 'z';
    } else if (lookAt.at(1) == -1) {
      // dir: -y
      right = 'Z';
    } else {
      std::cout << "ERROR: right was not set for +x upDir" << std::endl;
    }
  } else if (upDir.at(0) == -1) {
    //upDir : -x
    if (lookAt.at(2) == 1) {
      //dir = +z
      right = 'y';
    } else if (lookAt.at(2) == -1) {
      // dir = -z
      right = 'Y';
    } else if (lookAt.at(1) == 1) {
      //dir = +y
      right = 'Z';
    } else if (lookAt.at(1) == -1) {
      //dir = -y
      right = 'z';
    } else {
      std::cout << "ERROR: cam.right was not set for -x upDir" << std::endl;
    }
  } else if (upDir.at(2) == 1) {
    //upDir = +z
    if (lookAt.at(0) == 1) {
      // dir : +x
      right = 'y';
    } else if (lookAt.at(0) == -1) {
      // dir : -x
      right = 'Y';
    } else if (lookAt.at(1) == 1) {
      //dir : +y
      right = 'X';
    } else if (lookAt.at(1) == -1) {
      //dir : -y
      right = 'x';
    } else {
      std::cout << "ERROR: right was not set for +z upDir" << std::endl;
    }
  } else if (upDir.at(2) == -1) {
    //upDir : -z
    if (lookAt.at(0) == 1) {
      //lookAt : +x
      right = 'Y';
    } else if (lookAt.at(0) == -1) {
      // lookAt : -x
      right = 'y';
    } else if (lookAt.at(1) == 1) {
      //lookAt : +y
      right = 'x';
    } else if (lookAt.at(1) == -1) {
      //lookAt : -y
      right = 'X';
    } else {
      std::cout << "ERROR: right was not set for -z upDir" << std::endl;
    }
  } else {
    std::cout << "ERROR: right was never properly set." << std::endl;
  }
  */
}


