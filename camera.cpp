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

/** Set d in a camera */
void Camera::setD() {
  d = h / (2*tan((fov/2)*(3.14159265/180)));
}

/** Given a camera with defined upDir and d, set its UL field, RIGHT, and DOWN */
void Camera::setULRD() {
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
}


