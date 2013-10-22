CC = g++
CFLAGS = -Wall -O2 -I./FreeImage/
LDFLAGS = -lm -lfreeimage -L./FreeImage/libfreeimage.a

default: scene

algebra3.o: algebra3.cpp algebra3.h
	$(CC) $(CFLAGS) -c -g algebra3.cpp

camera.o: camera.cpp camera.h
	$(CC) $(CFLAGS) -c -g camera.cpp

film.o: film.cpp film.h
	$(CC) $(CFLAGS) -c -g film.cpp

raytracer.o: raytracer.cpp raytracer.h
	$(CC) $(CFLAGS) -c -g raytracer.cpp

sampler.o: sampler.cpp sampler.h
	$(CC) $(CFLAGS) -c -g sampler.cpp

scene.o: scene.cpp scene.h camera.h algebra3.h film.h sampler.h raytracer.h
	$(CC) $(CFLAGS) -c -g scene.cpp

scene: scene.o sampler.o camera.o film.o raytracer.o algebra3.o
	$(CC) $(CFLAGS) -g algebra3.o camera.o film.o raytracer.o sampler.o scene.o $(LDFLAGS) -o as2

clean:
	rm -rf *.o *~ as2

