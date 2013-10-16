CC = g++
CFLAGS = -Wall
LDFLAGS = -lm -lfreeimage

default: scene

camera.o: camera.cpp camera.h
	$(CC) $(CFLAGS) -c camera.cpp

film.o: film.cpp film.h
	$(CC) $(CFLAGS) -c film.cpp

sampler.o: sampler.cpp sampler.h
	$(CC) $(CFLAGS) -c sampler.cpp

raytracer.o: raytracer.cpp raytracer.h
	$(CC) $(CFLAGS) -c raytracer.cpp

scene.o: scene.cpp scene.h camera.h film.h sampler.h raytracer.h
	$(CC) $(CFLAGS) -c scene.cpp

scene: scene.o camera.o film.o raytracer.o
	$(CC) $(CFLAGS) camera.o film.o sampler.o raytracer.o scene.o $(LDFLAGS) -o as2

clean:
	rm -rf *.o *~ as2

#assignment_01 makefile:

# CC = g++
# ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
# 	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
# 	LDFLAGS = -framework GLUT -framework OpenGL \
#     	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
#     	-lGL -lGLU -lm -lstdc++
# else
# 	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iexample_01/glut-3.7.6-bin
# 	LDFLAGS = -lglut -lGLU -lGL -lm -lpng
# endif
	
# RM = /bin/rm -f 
# all: main 
# main: example_01.o 
# 	$(CC) $(CFLAGS) -o as1 example_01.o $(LDFLAGS) 
# example_01.o: example_01.cpp
# 	$(CC) $(CFLAGS) -c example_01.cpp -o example_01.o
# clean: 
# 	$(RM) *.o as1