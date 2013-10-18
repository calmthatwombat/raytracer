#include "scene.h"


/* METHOD DESCRIPTION: begins for-looping through each pixel in window of size width 
 * height. This method will generate a ray that will shoot to each pixel in the window, and get the RGB value to be passed to Film.*/
/* PARAM width, height : width and height of image plane
   PARAM depth : the depth of recursion
   PARAM outputFileName : the output file name that film needs to create the PNG output file.
   PARAM Sampler : needs Sampler object to get access to a Sample and the Camera */
void render(int width, int height, int depth, std::string outputFileName, 
	    Sampler &sampler){
  //Getting the camera.right and camera.down for the iteration
  char camRight = sampler.camera.right;
  char camDown = sampler.camera.down;

  //Initializing a sample object to cam.ul coordinates:
  Sample samp(sampler.camera.ul.at(0), sampler.camera.ul.at(1), sampler.camera.ul.at(2));

  for(int j = 0; j < height; j++) {
    for(int i = 0; i < width; i++) {
      //using GenerateRay to create a ray shooting to the sample position
      Ray outRay = sampler.camera.generateRay(samp);

      //getting the rgb value returned by calling raytracer.trace
      samp.rgb = sampler.raytracer.trace(outRay, depth);

      //Passing the rgb value to film (this rgb value will to be saved in the Film.bitmap)
      sampler.film.setPixel(i, j, samp.rgb);

      //increment/decrement the samp object depending on what is camera.right
      if (camRight == 'X') {
        samp.x += 1.0f;
	if (i == width - 1)	// check for reset
	  samp.x = sampler.camera.ul.at(0);
      } else if (camRight == 'x') {
        samp.x -= 1.0f;
	if (i == width - 1)	// check for reset
	  samp.x = sampler.camera.ul.at(0);
      } else if (camRight == 'Y') {
        samp.y += 1.0f;
	if (i == width - 1)	// check for reset
	  samp.y = sampler.camera.ul.at(1);
      } else if (camRight == 'y') {
        samp.y -= 1.0f;
	if (i == width - 1)	// check for reset
	  samp.y = sampler.camera.ul.at(1);
      } else if (camRight == 'Z') {
        samp.z += 1.0f;
	if (i == width - 1)	// check for reset
	  samp.z = sampler.camera.ul.at(2);
      } else if (camRight == 'z') {
        samp.z -= 1.0f;
	if (i == width - 1)	// check for reset
	  samp.z = sampler.camera.ul.at(2);
      }
    }
    //increment/decrement the samp object depending on what is camera.down
    if (camDown == 'X') {
      samp.x += 1.0f;
    } else if (camDown == 'x') {
      samp.x -= 1.0f;
    } else if (camDown == 'Y') {
      samp.y += 1.0f;
    } else if (camDown == 'y') {
      samp.y -= 1.0f;
    } else if (camDown == 'Z') {
      samp.z += 1.0f;
    } else if (camDown == 'z') {
      samp.z -= 1.0f;
    }

  }
  //A png image is outputted by calling the film.outputImage method!
  //At this point, the Film.Bitmap (size width*height) should be completey filled, and ready
  //to produce a PNG image!
  sampler.film.outputImage(outputFileName);


}

//Lizzie: I addded this main method just so we know where to start the parsing. This can be changed, I guess.
int main(int argc, char *argv[]) {
  /**
  //parsing command line for input file name:
  std::string file = std::string(argv[1]);
  
  // Initialize/Declare everything!
  float width, height;
  int maxDepth;
  Camera c;

  // Initialize Containers
  
  std::string fname = "output.bmp";

  std::ifstream inpfile(file.c_str());
  if(!inpfile.is_open()) {
    std::cout << "Unable to open file" << std::endl;
  } else {
    std::string line;
    //MatrixStack mst;

    while(inpfile.good()) {
      std::vector<std::string> splitline;
      std::string buf;

      std::getline(inpfile,line);
      std::stringstream ss(line);

      while (ss >> buf) {
        splitline.push_back(buf);
      }
      //Ignore blank lines
      if(splitline.size() == 0) {
        continue;
      }

      //Ignore comments
      if(splitline[0][0] == '#') {
        continue;
      }

      //Valid commands:
      //size width height
      //  must be first command of file, controls image size
      else if(!splitline[0].compare("size")) {
        width = atof(splitline[1].c_str());
        height = atof(splitline[2].c_str());
      }
      //maxdepth depth
      //  max # of bounces for ray (default 5)
      else if(!splitline[0].compare("maxdepth")) {
        maxDepth = atoi(splitline[1].c_str());
      }
      //output filename
      //  output file to write image to 
      else if(!splitline[0].compare("output")) {
        fname = splitline[1];
      }

      //camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
      //  speciﬁes the camera in the standard way, as in homework 2.
      else if (!splitline[0].compare("camera")) {
        c.lookFrom.at(0) = atof(splitline[1].c_str());
	c.lookFrom.at(1) = atof(splitline[2].c_str());
	c.lookFrom.at(2) = atof(splitline[3].c_str());
	c.lookAt.at(0) = atof(splitline[4].c_str());
	c.lookAt.at(1) = atof(splitline[5].c_str());
	c.lookAt.at(2) = atof(splitline[6].c_str());
        c.upDir.at(0) = atof(splitline[7].c_str());
	c.upDir.at(1) = atof(splitline[8].c_str());
	c.upDir.at(2) = atof(splitline[9].c_str());
	c.fov = atof(splitline[10].c_str());
      }

      //sphere x y z radius
      //  Deﬁnes a sphere with a given position and radius.
      else if(!splitline[0].compare("sphere")) {
	Sphere sph(atof(splitline[1].c_str()), atof(splitline[2].c_str()), 
		   atof(splitline[3].c_str()), atof(splitline[4].c_str()));
        // Create new sphere:
        //   Store 4 numbers
        //   Store current property values
        //   Store current top of matrix stack
      }
      //maxverts number
      //  Deﬁnes a maximum number of vertices for later triangle speciﬁcations. 
      //  It must be set before vertices are deﬁned.
      else if(!splitline[0].compare("maxverts")) {
        // Care if you want
        // Here, either declare array size
        // Or you can just use a STL vector, in which case you can ignore this
      }
      //maxvertnorms number
      //  Deﬁnes a maximum number of vertices with normals for later speciﬁcations.
      //  It must be set before vertices with normals are deﬁned.
      else if(!splitline[0].compare("maxvertnorms")) {
        // Care if you want
      }
      //vertex x y z
      //  Deﬁnes a vertex at the given location.
      //  The vertex is put into a pile, starting to be numbered at 0.
      else if(!splitline[0].compare("vertex")) {
        // x: atof(splitline[1].c_str()),
        // y: atof(splitline[2].c_str()),
        // z: atof(splitline[3].c_str()));
        // Create a new vertex with these 3 values, store in some array
      }
      //vertexnormal x y z nx ny nz
      //  Similar to the above, but deﬁne a surface normal with each vertex.
      //  The vertex and vertexnormal set of vertices are completely independent
      //  (as are maxverts and maxvertnorms).
      else if(!splitline[0].compare("vertexnormal")) {
        // x: atof(splitline[1].c_str()),
        // y: atof(splitline[2].c_str()),
        // z: atof(splitline[3].c_str()));
        // nx: atof(splitline[4].c_str()),
        // ny: atof(splitline[5].c_str()),
        // nz: atof(splitline[6].c_str()));
        // Create a new vertex+normal with these 6 values, store in some array
      }
      //tri v1 v2 v3
      //  Create a triangle out of the vertices involved (which have previously been speciﬁed with
      //  the vertex command). The vertices are assumed to be speciﬁed in counter-clockwise order. Your code
      //  should internally compute a face normal for this triangle.
      else if(!splitline[0].compare("tri")) {
        // v1: atof(splitline[1].c_str())
        // v2: atof(splitline[2].c_str())
        // v3: atof(splitline[3].c_str())
        // Create new triangle:
        //   Store pointer to array of vertices
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
      }
      //trinormal v1 v2 v3
      //  Same as above but for vertices speciﬁed with normals.
      //  In this case, each vertex has an associated normal, 
      //  and when doing shading, you should interpolate the normals 
      //  for intermediate points on the triangle.
      else if(!splitline[0].compare("trinormal")) {
        // v1: atof(splitline[1].c_str())
        // v2: atof(splitline[2].c_str())
        // v3: atof(splitline[3].c_str())
        // Create new triangle:
        //   Store pointer to array of vertices (Different array than above)
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
      }

      //translate x y z
      //  A translation 3-vector
      else if(!splitline[0].compare("translate")) {
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // Update top of matrix stack
      }
      //rotate x y z angle
      //  Rotate by angle (in degrees) about the given axis as in OpenGL.
      else if(!splitline[0].compare("rotate")) {
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // angle: atof(splitline[4].c_str())
        // Update top of matrix stack
      }
      //scale x y z
      //  Scale by the corresponding amount in each axis (a non-uniform scaling).
      else if(!splitline[0].compare("scale")) {
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // Update top of matrix stack
      }
      //pushTransform
      //  Push the current modeling transform on the stack as in OpenGL. 
      //  You might want to do pushTransform immediately after setting 
      //   the camera to preserve the “identity” transformation.
      else if(!splitline[0].compare("pushTransform")) {
        //mst.push();
      }
      //popTransform
      //  Pop the current transform from the stack as in OpenGL. 
      //  The sequence of popTransform and pushTransform can be used if 
      //  desired before every primitive to reset the transformation 
      //  (assuming the initial camera transformation is on the stack as 
      //  discussed above).
      else if(!splitline[0].compare("popTransform")) {
        //mst.pop();
      }

      //directional x y z r g b
      //  The direction to the light source, and the color, as in OpenGL.
      else if(!splitline[0].compare("directional")) {
        // x: atof(splitline[1].c_str()),
        // y: atof(splitline[2].c_str()),
        // z: atof(splitline[3].c_str()));
        // r: atof(splitline[4].c_str()),
        // g: atof(splitline[5].c_str()),
        // b: atof(splitline[6].c_str()));
        // add light to scene...
      }
      //point x y z r g b
      //  The location of a point source and the color, as in OpenGL.
      else if(!splitline[0].compare("point")) {
        // x: atof(splitline[1].c_str()),
        // y: atof(splitline[2].c_str()),
        // z: atof(splitline[3].c_str()));
        // r: atof(splitline[4].c_str()),
        // g: atof(splitline[5].c_str()),
        // b: atof(splitline[6].c_str()));
        // add light to scene...
      }
      //attenuation const linear quadratic
      //  Sets the constant, linear and quadratic attenuations 
      //  (default 1,0,0) as in OpenGL.
      else if(!splitline[0].compare("attenuation")) {
        // const: atof(splitline[1].c_str())
        // linear: atof(splitline[2].c_str())
        // quadratic: atof(splitline[3].c_str())
      }
      //ambient r g b
      //  The global ambient color to be added for each object 
      //  (default is .2,.2,.2)
      else if(!splitline[0].compare("ambient")) {
        // r: atof(splitline[1].c_str())
        // g: atof(splitline[2].c_str())
        // b: atof(splitline[3].c_str())
      }

      //diﬀuse r g b
      //  speciﬁes the diﬀuse color of the surface.
      else if(!splitline[0].compare("diffuse")) {
        // r: atof(splitline[1].c_str())
        // g: atof(splitline[2].c_str())
        // b: atof(splitline[3].c_str())
        // Update current properties
      }
      //specular r g b 
      //  speciﬁes the specular color of the surface.
      else if(!splitline[0].compare("specular")) {
        // r: atof(splitline[1].c_str())
        // g: atof(splitline[2].c_str())
        // b: atof(splitline[3].c_str())
        // Update current properties
      }
      //shininess s
      //  speciﬁes the shininess of the surface.
      else if(!splitline[0].compare("shininess")) {
        // shininess: atof(splitline[1].c_str())
        // Update current properties
      }
      //emission r g b
      //  gives the emissive color of the surface.
      else if(!splitline[0].compare("emission")) {
        // r: atof(splitline[1].c_str())
        // g: atof(splitline[2].c_str())
        // b: atof(splitline[3].c_str())
        // Update current properties
      } else {
        std::cerr << "Unknown command: " << splitline[0] << std::endl;
      }
    }

    inpfile.close();
  }


  // BEGIN SHIT
  setULRD(c);			// set camera ULRD
  setD(c);			// set camera d
  Film f(width, height);
  RayTracer rt(c);
  Sampler s(c, rt, f);
  render(width, height, maxDepth, fname, s);
  */
  Camera c;
  c.w = 100;
  c.h = 100;
  c.lookFrom.at(0) = 0;
  c.lookFrom.at(1) = 0;
  c.lookFrom.at(2) = 50;
  c.lookAt.at(0) = 0;
  c.lookAt.at(1) = 0;
  c.lookAt.at(2) = -1;
  c.upDir.at(1) = 1;
  c.fov = 80;
  c.setD();
  c.setULRD();
  Film f(100, 100);
  RayTracer rt(c);
  Sampler s(c, rt, f);
  render(100, 100, 5, "output.png", s);
}


