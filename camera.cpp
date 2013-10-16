#include "camera.h"

/** Define Ray empty constructor */
Ray::Ray():
  pos(3, 0.0f), 
  dir(3, 0.0f)
{}

/** Define Ray constructor */
Ray::Ray(vector<float> start, vector<float> direction) {
  startPos = start;
  dir = direction;
}

/** Define Camera empty constructor */
Camera::Camera(float x, float y, float fov):
  w(x),
  h(y),
  d(0.0f),
  ul(3, 0.0f),
  lookFrom(3, 0.0f),
  lookAt(3, 0.0f),
  upDir(3, 0.0f)
{
  d = h / (2*tan((fov/2)*(3.14159265/180)));
  // Don't forget to set ul corner
}

/** PARAM *sample and PARAM *ray
 *  Returns a Ray going from Camera location towards the sample,
 *  probably calculated using fov */
Ray Camera::generateRay(Sample &sample) {
  Ray result();
  // Result's ray point
  result.pos.at(0) = lookFrom.at(0);
  result.pos.at(1) = lookFrom.at(1);
  result.pos.at(2) = lookFrom.at(2);
  // Result's ray vector
  result.dir.at(0) = sample.x - lookFrom.at(0);
  result.dir.at(1) = sample.y - lookFrom.at(1);
  result.dir.at(2) = sample.z - lookFrom.at(2);
  normalizationizerificationator(result.dir);
  return result;
}

/** Given a camera with defined upDir and d, set its UL field, RIGHT, and DOWN */
void setULRD(Camera cam) {
  vector<float> ULPos(3, 0.0f);
  // up: +y
  if (cam.upDir.at(1) == 1) {
    if (cam.lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = cam.lookFrom.at(0) + (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) + (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) + d;
    } else if (cam.lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = cam.lookFrom.at(0) - (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) + (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) - d;
    } else if (cam.lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = cam.lookFrom.at(0) + d;
      ULPos.at(1) = cam.lookFrom.at(1) + (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) - (w/2);	
    } else if (cam.lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = cam.lookFrom.at(0) - d;
      ULPos.at(1) = cam.lookFrom.at(1) + (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) + (w/2);	
    } else {
      std::cout << "ULPOS was not set for +y upDir" << std:endl;
    }
    // up: -y
  } else if (cam.upDir.at(1) == -1) {
    if (cam.lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = cam.lookFrom.at(0) - (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) - (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) + d;
    } else if (cam.lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = cam.lookFrom.at(0) + (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) - (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) - d;
    } else if (cam.lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = cam.lookFrom.at(0) + d;
      ULPos.at(1) = cam.lookFrom.at(1) - (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) + (w/2);	
    } else if (cam.lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = cam.lookFrom.at(0) - d;
      ULPos.at(1) = cam.lookFrom.at(1) - (h/2);
      ULPos.at(2) = cam.lookFrom.at(2) - (w/2);	
    } else {
      std::cout << "ULPOS was not set for -y upDir" << std:endl;
    }
    // up: +x
  } else if (cam.upDir.at(0) == 1) {
    if (cam.lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = cam.lookFrom.at(0) + (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) - (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) + d;
    } else if (cam.lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = cam.lookFrom.at(0) + (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) + (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) - d;
    } else if (cam.lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = cam.lookFrom.at(0) + (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) + d;
      ULPos.at(2) = cam.lookFrom.at(2) + (w/2);
    } else if (cam.lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = cam.lookFrom.at(0) + (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) - d;
      ULPos.at(2) = cam.lookFrom.at(2) - (w/2);
    } else {
      std::cout << "ULPOS was not set for +x upDir" << std:endl;
    }
    // up: -x
  } else if (cam.upDir.at(0) == -1) {
    if (cam.lookAt.at(2) == 1) {
      // dir: +z
      ULPos.at(0) = cam.lookFrom.at(0) - (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) + (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) + d;
    } else if (cam.lookAt.at(2) == -1) {
      // dir: -z
      ULPos.at(0) = cam.lookFrom.at(0) - (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) - (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) - d;
    } else if (cam.lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = cam.lookFrom.at(0) - (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) + d;
      ULPos.at(2) = cam.lookFrom.at(2) - (w/2);
    } else if (cam.lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = cam.lookFrom.at(0) - (h/2);
      ULPos.at(1) = cam.lookFrom.at(1) - d;
      ULPos.at(2) = cam.lookFrom.at(2) + (w/2);
    } else {
      std::cout << "ULPOS was not set for -x upDir" << std:endl;
    }
    // up: +z
  } else if (cam.upDir.at(2) == 1) {
    if (cam.lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = cam.lookFrom.at(0) + d;
      ULPos.at(1) = cam.lookFrom.at(1) + (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) + (h/2);
    } else if (cam.lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = cam.lookFrom.at(0) - d;
      ULPos.at(1) = cam.lookFrom.at(1) - (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) + (h/2);
    } else if (cam.lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = cam.lookFrom.at(0) - (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) + d;
      ULPos.at(2) = cam.lookFrom.at(2) + (h/2);
    } else if (cam.lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = cam.lookFrom.at(0) + (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) - d;
      ULPos.at(2) = cam.lookFrom.at(2) + (h/2);
    } else {
      std::cout << "ULPOS was not set for +z upDir" << std:endl;
    }
    // up: -z
  } else if (cam.upDir.at(0) == 0 && cam.upDir.at(1) == 0 && cam.upDir.at(2) == -1) {
    if (cam.lookAt.at(0) == 1) {
      // dir: +x
      ULPos.at(0) = cam.lookFrom.at(0) + d;
      ULPos.at(1) = cam.lookFrom.at(1) - (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) - (h/2);
    } else if (cam.lookAt.at(0) == -1) {
      // dir: -x
      ULPos.at(0) = cam.lookFrom.at(0) - d;
      ULPos.at(1) = cam.lookFrom.at(1) + (w/2);
      ULPos.at(2) = cam.lookFrom.at(2) - (h/2);
    } else if (cam.lookAt.at(1) == 1) {
      // dir: +y
      ULPos.at(0) = cam.lookFrom.at(0) + (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) + d;
      ULPos.at(2) = cam.lookFrom.at(2) - (h/2);
    } else if (cam.lookAt.at(1) == -1) {
      // dir: -y
      ULPos.at(0) = cam.lookFrom.at(0) - (w/2);
      ULPos.at(1) = cam.lookFrom.at(1) - d;
      ULPos.at(2) = cam.lookFrom.at(2) - (h/2);
    } else {
      std::cout << "ULPOS was not set for -z upDir" << std:endl;
    }
  } else {
    std::cout << "idk ULPOS was never properly set." << std::endl;
  }
  //Setting the upper left corner of the image plane
  cam.ulPos = ULPOS;
  // note: X = positive and x = negative
  // set DOWN representation char/////////////////////
  if (cam.upDir.at(0) == -1) {
    // up: -x
    cam.down = 'X';
  } else if (cam.upDir.at(0) == 1) {
    // up: x
    cam.down = 'x';
  } else if (cam.upDir.at(1) == -1) {
    // up: -y
    cam.down = 'Y';
  } else if (cam.upDir.at(1) == 1) {
    // up: y
    cam.down = 'y';
  } else if (cam.upDir.at(2) == -1) {
    // up: -z
    cam.down = 'Z';
  } else if (cam.upDir.at(2) == 1) {
    // up: z
    cam.down = 'z';
  } else {
    std::cout << "ERROR: cam.down not set properly." << std::endl;
  }
  //set RIGHT reprentation char//////////////////////////
		
  if (cam.upDir.at(1) == 1) {
    // up: +y
    if (cam.lookAt.at(2) == 1) {
      // dir: +z
      cam.right = 'x';
    } else if (cam.lookAt.at(2) == -1) {
      // dir: -z
      cam.right = 'X';
    } else if (cam.lookAt.at(0) == 1) {
      // dir: +x
      cam.right = 'Z';
    } else if (cam.lookAt.at(0) == -1) {
      // dir: -x
      cam.right = 'z';
    } else {
      std::cout << "ERROR: cam.right was not set for +y upDir" << std:endl;
    }
  } else if (cam.upDir.at(1) == -1) {
    // up: -y
    if (cam.lookAt.at(2) == 1) {
      // dir: +z
      cam.right = 'X';
    } else if (cam.lookAt.at(2) == -1) {
      // dir: -z
      cam.right = 'x';
    } else if (cam.lookAt.at(0) == 1) {
      // dir: +x
      cam.right = 'z';
    } else if (cam.lookAt.at(0) == -1) {
      // dir: -x
      cam.right = 'Z';
    } else {
      std::cout << "ERROR: cam.right was not set for -y upDir" << std:endl;
    }
  } else if (cam.upDir.at(0) == 1 && cam.upDir.at(1) == 0 && cam.upDir.at(2) == 0) {
    // up: +x
    if (cam.lookAt.at(2) == 1) {
      // dir: +z
      cam.right = 'Y';
    } else if (cam.lookAt.at(2) == -1) {
      // dir: -z
      cam.right = 'y';
    } else if (cam.lookAt.at(1) == 1) {
      // dir: +y
      cam.right = 'z';
    } else if (cam.lookAt.at(1) == -1) {
      // dir: -y
      cam.right = 'Z';
    } else {
      std::cout << "ERROR: cam.right was not set for +x upDir" << std:endl;
    }
  } else if (cam.upDir.at(0) == -1 && cam.upDir.at(1) == 0 && cam.upDir.at(2) == 0) {
    //upDir : -x
    if (cam.lookAt.at(2) == 1) {
      //dir = +z
      cam.right = 'y';
    } else if (cam.lookAt.at(2) == -1) {
      // dir = -z
      cam.right = 'Y';
    } else if (cam.lookAt.at(1) == 1) {
      //dir = +y
      cam.right = 'Z';
    } else if (cam.lookAt.at(1) == -1) {
      //dir = -y
      cam.right = 'z';
    } else {
      std::cout << "ERROR: cam.right was not set for -x upDir" << std:endl;
    }
  } else if (cam.upDir.at(0) == 0 && cam.upDir.at(1) == 0 && cam.upDir.at(2) == 1) {
    //upDir = +z
    if (cam.lookAt.at(0) == 1) {
      // dir : +x
      cam.right = 'y';
    } else if (cam.lookAt.at(0) == -1) {
      // dir : -x
      cam.right = 'Y';
    } else if (cam.lookAt.at(1) == 1) {
      //dir : +y
      cam.right = 'X';
    } else if (cam.lookAt.at(1) == -1) {
      //dir : -y
      cam.right = 'x';
    } else {
      std::cout << "ERROR: cam.right was not set for +z upDir" << std:endl;
    }
  } else if (cam.upDir.at(0) == 0 && cam.upDir.at(1) == 0 && cam.upDir.at(2) == -1) {
    //upDir : -z
    if (cam.lookAt.at(0) == 1) {
      //lookAt : +x
      cam.right = 'Y';
    } else if (cam.lookAt.at(0) == -1) {
      // lookAt : -x
      cam.right = 'y';
    } else if (cam.lookAt.at(1) == 1) {
      //lookAt : +y
      cam.right = 'x';
    } else if (cam.lookAt.at(1) == -1) {
      //lookAt : -y
      cam.right = 'X';
    } else {
      std::cout << "ERROR: cam.right was not set for -z upDir" << std:endl;
    }
  } else {
    std::cout << "ERROR: cam.right was never properly set." << std::endl;
  }
}
