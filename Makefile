CC = g++
CFLAGS = -Wall -O2 -I./FreeImage/
LDFLAGS = -lm -lfreeimage -L./FreeImage/libfreeimage.a

default: scene

camera.o: camera.cpp camera.h
	$(CC) $(CFLAGS) -c camera.cpp

film.o: film.cpp film.h
	$(CC) $(CFLAGS) -c film.cpp

raytracer.o: raytracer.cpp raytracer.h
	$(CC) $(CFLAGS) -c raytracer.cpp

sampler.o: sampler.cpp sampler.h
	$(CC) $(CFLAGS) -c sampler.cpp

scene.o: scene.cpp scene.h camera.h film.h sampler.h raytracer.h
	$(CC) $(CFLAGS) -c scene.cpp

scene: scene.o sampler.o camera.o film.o raytracer.o
	$(CC) $(CFLAGS) camera.o film.o raytracer.o sampler.o scene.o $(LDFLAGS) -o as2

clean:
	rm -rf *.o *~ as2

