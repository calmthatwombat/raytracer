#include "sampler.h"

/** Define Sampler constructor with Camera, Raytracer, and Film */
Sampler::Sampler(Camera *_camera, RayTracer *_raytracer, Film *_film):
  camera(_camera),
  raytracer(_raytracer),
  film(_film)
{}




/**

FILES LIZZIE CHANGED: Sampler.cpp , Sampler.h, Film.cpp, Film.h, Scene.cpp (added main and loadscene)

QUESTIONS:
in camera.generateRay, sample is (x, y, z), but in our definition last night, sample was just (x, y)
I never needed 3dpixel/incrementing to set pixel?
should I initialize sample object inside or outside for loop?
  follow up: am I really changing samp.x and samp.y....correctly?
check member initialization lol?


-----



main creates ONE sampler object (this is in scene.cpp)
    sampler holds fields that tracks the i, j (basically where we are throughout the looping), and camera
    SAMPLE definition: a xy point in 2D window
    Sampler will somehow need to obtain Camera's ul. One way is to put Camera into Sampler as a field.        char right and char down are necessary too.


    INCREMENTING i, j   in Sampler. (Each new i, j is a new Sample object):
    You start with (i, j) = (0, 0). 
    we don't need upDir anymore since we already calulated
    RIGHT and DOWN. so we also don't need lookAt. 
    MUST use  ul  to begin.
    Suppose ul == (4, 5, -6).
    Suppose right == X  and down == z.

    Example set of (i, j) from this data:
    (i,j): (x,y,z)
    (0,0): (4,5,-6)
    (0,1): (5,5,-6)
    (0,2): (6,5,-6)

    now suppose width == 140
    then,
    (0,139): (x,5,-6)
    (1,0): (4,5,-7) (-7 because it's a lowercase z. -5 if it were a cap Z.)

    

SAMPLER also needs the raytracer (pass it in) AND Film.
SAMPLER constructor needs raytracer, film, and camera, but has i and j fields.

SAMPLER purpose: holds info, tracks progress, of i,j iteration. also creates sample objects. sampler will eventually have created i*j sample objects

i*j == width*height

SAMPLE purpose: calls Camera's generateRay method. passes THAT RAY into the Raytracer object (which returns an eventual color (ABSTRACT THIS WELL)) That color is then sent to the Film class 

SAMPLE's CONSTRUCTOR:
vector<float> rgb = raytracer.trace(cam.generateRay(this);


Now that sample will contain the color of the i,j of the window viewport.
Sampler, which called all this Sample shit, will now send this Sample object to Film. Film stores every color and will eventually construct that .png

done.
*/
