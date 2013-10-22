#include "scene.h"


/* METHOD DESCRIPTION: begins for-looping through each pixel in window of size width 
 * height. This method will generate a ray that will shoot to each pixel in the window, and get the RGB value to be passed to Film.*/
/* PARAM width, height : width and height of image plane
   PARAM depth : the depth of recursion
   PARAM outputFileName : the output file name that film needs to create the PNG output file.
   PARAM Sampler : needs Sampler object to get access to a Sample and the Camera */
void render(int width, int height, int depth, std::string outputFileName, 
	    Sampler *sampler){

  //Initializing a sample object to cam.ul coordinates:
  Sample samp(sampler->camera->ul.at(0), sampler->camera->ul.at(1), sampler->camera->ul.at(2));

  for(int j = 0; j < height; j++) {
    for(int i = 0; i < width; i++) {
      //using GenerateRay to create a ray shooting to the sample position
      Ray outRay = sampler->camera->generateRay(samp);

      //getting the rgb value returned by calling raytracer.trace
      samp.rgb = sampler->raytracer->trace(outRay, 5);

      //Passing the rgb value to film (this rgb value will to be saved in the Film.bitmap)
      sampler->film->setPixel(i, height - 1 - j, samp.rgb);

      // Increment samp.x, samp.y, and samp.z by cam->right
      samp.x += sampler->camera->right.at(0);
      samp.y += sampler->camera->right.at(1);
      samp.z += sampler->camera->right.at(2);
    }

    // Reset the i portion
    samp.x -= sampler->camera->right.at(0) * width;
    samp.y -= sampler->camera->right.at(1) * width;
    samp.z -= sampler->camera->right.at(2) * width;

    // Increment samp.x, samp.y, and samp.z by cam->down
    samp.x += sampler->camera->down.at(0);
    samp.y += sampler->camera->down.at(1);
    samp.z += sampler->camera->down.at(2);
  }

  /* film->outputImage is called to print the image and complete the program.
   * the bitmap should be completely filled by now */
  sampler->film->outputImage(outputFileName);
}

int main(int argc, char *argv[]) {
  // Start stopwatch
  double start = clock();
  
  // Parsing command line for input file name:
  std::string file = std::string(argv[1]);
  
  // Initialize/Declare everything!
  float windowW = 0;
  float windowH = 0;
  unsigned int maxDepth = 0;
  Camera c;
  unsigned int shapeNum = 0;
  BRDF brdf;
  float a1 = 1.0f;
  float a2 = 0.0f;
  float a3 = 0.0f;

  // Initialize Containers
  std::vector< std::vector<float> > vertices;
  std::vector< std::vector<float> > trinormvertices;
  std::vector< std::vector<float> > trinormnormals; 
  std::vector<Shape *> shapes0;
  std::vector<Shape> shapes1;
  std::vector<Sphere> spheres1;
  std::vector<Triangle> triangles1;
  std::vector<TriNormal> trinormals1;
  std::vector<DirectionalLight> DLs1;
  std::vector<PointLight> PLs1;
  std::vector<DirectionalLight *> DLs0;
  std::vector<PointLight *> PLs0;
  
  std::string fname = "output.bmp";

  std::ifstream inpfile(file.c_str());
  if(!inpfile.is_open()) {
    std::cout << "Unable to open file" << std::endl;
  } else {
    std::string line;
    // MatrixStack mst;

    while(inpfile.good()) {
      std::vector<std::string> splitline;
      std::string buf;

      std::getline(inpfile,line);
      std::stringstream ss(line);

      while (ss >> buf) {
        splitline.push_back(buf);
      }
      // Ignore blank lines
      if(splitline.size() == 0) {
        continue;
      }

      // Ignore comments
      if(splitline[0][0] == '#') {
        continue;
      }

      // Valid commands:
      // size width height
      //  must be first command of file, controls image size
      else if(!splitline[0].compare("size")) {
        windowW = atoi(splitline[1].c_str());
	c.w = windowW;
        c.h = windowH = atoi(splitline[2].c_str());
      }
      // maxdepth depth
      //  max # of bounces for ray (default 5)
      else if(!splitline[0].compare("maxdepth")) {
        maxDepth = atoi(splitline[1].c_str());
      }
      // output filename
      //  output file to write image to 
      else if(!splitline[0].compare("output")) {
        fname = splitline[1];
      }

      // camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
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

      // sphere x y z radius
      //  Deﬁnes a sphere with a given position and radius.
      else if(!splitline[0].compare("sphere")) {
	Shape shape(shapeNum);
	Sphere s(atof(splitline[1].c_str()),
		 atof(splitline[2].c_str()),
		 atof(splitline[3].c_str()),
		 atof(splitline[4].c_str()),
		 brdf, shapeNum++);
	spheres1.push_back(s);
	shape.brdf = brdf;
	shapes1.push_back(shape);
        //   STORE CURRENT TOP OF MATRIX STACK
      }

      // Ignore these, unused
      else if(!splitline[0].compare("maxverts")) {
        continue;
      }
      else if(!splitline[0].compare("maxvertsnorms")) {
        continue;
      }
      
      //vertex x y z
      //  Deﬁnes a vertex at the given location.
      //  The vertex is put into a pile, starting to be numbered at 0.
      else if(!splitline[0].compare("vertex")) {
	std::vector<float> p(3, 0.0f);
	p.at(0) = atof(splitline[1].c_str());
	p.at(1) = atof(splitline[2].c_str());
	p.at(2) = atof(splitline[3].c_str());
	vertices.push_back(p);
	
        // Create a new vertex with these 3 values, store in some array
      }
      //vertexnormal x y z nx ny nz
      //  Similar to the above, but deﬁne a surface normal with each vertex.
      //  The vertex and vertexnormal set of vertices are completely independent
      //  (as are maxverts and maxvertnorms).
      else if(!splitline[0].compare("vertexnormal")) {
	std::vector<float> p(6, 0.0f);
	std::vector<float> n(6, 0.0f);
	p.at(0) = atof(splitline[1].c_str());
	p.at(1) = atof(splitline[2].c_str());
	p.at(2) = atof(splitline[3].c_str());
	n.at(3) = atof(splitline[4].c_str());
	n.at(4) = atof(splitline[5].c_str());
	n.at(5) = atof(splitline[6].c_str());
	trinormvertices.push_back(p);
	trinormnormals.push_back(n);
      }
      //tri v1 v2 v3
      //  Create a triangle out of the vertices involved (which have previously 
      //  been speciﬁed with the vertex command). The vertices are assumed to 
      //  be speciﬁed in counter-clockwise order. Your code should internally 
      //  compute a face normal for this triangle.
      else if(!splitline[0].compare("tri")) {
	Shape shape(shapeNum);
	std::vector<float> *v1 = &vertices.at(atoi(splitline[1].c_str()));
	std::vector<float> *v2 = &vertices.at(atoi(splitline[2].c_str()));
	std::vector<float> *v3 = &vertices.at(atoi(splitline[3].c_str()));
	Triangle t(v1, v2, v3, brdf, shapeNum++);
	triangles1.push_back(t);
	shape.brdf = brdf;
	shapes1.push_back(shape);
        //   STORE CURRENT TOP OF MATRIX
      }
      //trinormal v1 v2 v3
      //  Same as above but for vertices speciﬁed with normals.
      //  In this case, each vertex has an associated normal, 
      //  and when doing shading, you should interpolate the normals 
      //  for intermediate points on the triangle.
      else if(!splitline[0].compare("trinormal")) {
	Shape shape(shapeNum);
	std::vector<float> *v1 = &trinormvertices.at(atoi(splitline[1].c_str()));
	std::vector<float> *v2 = &trinormvertices.at(atoi(splitline[2].c_str()));
	std::vector<float> *v3 = &trinormvertices.at(atoi(splitline[3].c_str()));
	std::vector<float> *n1 = &trinormnormals.at(atoi(splitline[1].c_str()));
	std::vector<float> *n2 = &trinormnormals.at(atoi(splitline[2].c_str()));
	std::vector<float> *n3 = &trinormnormals.at(atoi(splitline[3].c_str()));
	TriNormal t(v1, v2, v3, n1, n2, n3, brdf, shapeNum++);
	trinormals1.push_back(t);
	shape.brdf = brdf;
	shapes1.push_back(shape);
        //   STORE CURRENT TOP OF MATRIX STACK
      }

      //Translate x y z
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
	DLs1.push_back(*new DirectionalLight (atof(splitline[1].c_str()),
					     atof(splitline[2].c_str()),
					     atof(splitline[3].c_str()),
					     atof(splitline[4].c_str()),
					     atof(splitline[5].c_str()),
					     atof(splitline[6].c_str())));
      }
      //point x y z r g b
      //  The location of a point source and the color, as in OpenGL.
      else if(!splitline[0].compare("point")) {
	PLs1.push_back(*new PointLight (atof(splitline[1].c_str()),
				       atof(splitline[2].c_str()),
				       atof(splitline[3].c_str()),
				       atof(splitline[4].c_str()),
				       atof(splitline[5].c_str()),
				       atof(splitline[6].c_str()), a1, a2, a3));
      }
      //attenuation const linear quadratic
      //  Sets the constant, linear and quadratic attenuations 
      //  (default 1,0,0) as in OpenGL.
      else if(!splitline[0].compare("attenuation")) {
        a1 = atof(splitline[1].c_str());
        a2 = atof(splitline[2].c_str());
        a3 = atof(splitline[3].c_str());
      }
      //ambient r g b
      //  The global ambient color to be added for each object 
      //  (default is .2,.2,.2), set in BRDF constructor
      else if(!splitline[0].compare("ambient")) {
        brdf.ka.at(0) = atof(splitline[1].c_str());
        brdf.ka.at(1) = atof(splitline[2].c_str());
        brdf.ka.at(2) = atof(splitline[3].c_str());
      }
      //diﬀuse r g b
      //  speciﬁes the diﬀuse color of the surface.
      else if(!splitline[0].compare("diffuse")) {
        brdf.kd.at(0) = atof(splitline[1].c_str());
        brdf.kd.at(1) = atof(splitline[2].c_str());
        brdf.kd.at(2) = atof(splitline[3].c_str());
      }
      //specular r g b 
      //  speciﬁes the specular color of the surface.
      else if(!splitline[0].compare("specular")) {
        brdf.ks.at(0) = atof(splitline[1].c_str());
        brdf.ks.at(1) = atof(splitline[2].c_str());
        brdf.ks.at(2) = atof(splitline[3].c_str());
      }
      //shininess s
      //  speciﬁes the shininess of the surface.
      else if(!splitline[0].compare("shininess")) {
	brdf.s = atof(splitline[1].c_str());
      }
      //emission r g b
      //  gives the emissive color of the surface.
      else if(!splitline[0].compare("emission")) {
        brdf.ke.at(0) = atof(splitline[1].c_str());
        brdf.ke.at(1) = atof(splitline[2].c_str());
        brdf.ke.at(2) = atof(splitline[3].c_str());
      } else {
        std::cerr << "Unknown command: " << splitline[0] << std::endl;
      }
    }

    inpfile.close();
  }

  // BEGIN SHIT
  c.setD();			// set camera d
  c.setULRD();			// set camera ULRD
  Film f(windowW, windowH);
  RayTracer rt(&c);
  Sampler s(&c, &rt, &f);
  // Create iterators for containers
  unsigned int sphCount = 0;
  unsigned int triCount = 0;
  unsigned int tnCount = 0;
  // Set up shapes0, the finalized container
  for (unsigned int i = 0; i < shapes1.size(); i++) {
    if (sphCount < spheres1.size() && spheres1[sphCount].num == i) {
      shapes1[i].sph = &spheres1[sphCount++];
      shapes0.push_back(&shapes1[i]);
    } else if (triCount < triangles1.size() && triangles1[triCount].num == i) {
      shapes1[i].tri = &triangles1[triCount++];
      shapes0.push_back(&shapes1[i]);
    } else if (tnCount < trinormals1.size() && trinormals1[tnCount].num == i) {
      shapes1[i].tn = &trinormals1[tnCount++];
      shapes0.push_back(&shapes1[i]);
    } else {
      printf("container maker not working");
      std::exit(1);
    }
  }
  for (unsigned int i = 0; i < PLs1.size(); i++) {
    PLs0.push_back(&PLs1[i]);
  }
  for (unsigned int i = 0; i < DLs1.size(); i++) {
    DLs0.push_back(&DLs1[i]);
  }
  
  rt.shapes = &shapes0;
  rt.DLs = &DLs0;
  rt.PLs = &PLs0;
  
  // GOOOOOOOOOOOOOOO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  render(windowW, windowH, maxDepth, fname, &s);
  double total = (clock() - start) / 1000000;
  if (total > 3599) {
    std::cout<<"Time taken: " << total / 3600 << " hours\n";
  } else if (total > 59) {
    std::cout<<"Time taken: " << total/60 << " minutes\n";
  } else {
    std::cout<<"Time taken: " << total << " seconds\n";
  }
}


