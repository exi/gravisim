LIBS =  `sdl-config --libs` -lpthread
FLAGS = `sdl-config --cflags` -D CORECOUNT=2 -O2 -mtune=k8
all: grav

camera.o:
	g++ ${FLAGS} -Wall camera.cpp -c

clusterbuild.o:
	g++ ${FLAGS} -Wall clusterbuild.cpp -c

gravenv.o:
	g++ ${FLAGS} -Wall -D NO_MAKEFILE gravenv.cpp -c

planet.o:
	g++ ${FLAGS} -Wall planet.cpp -c

grav: camera.o gravenv.o planet.o 
	g++ ${FLAGS} -Wall ${LIBS} camera.o planet.o gravenv.o main.cpp -o grav

clean:
	rm *.o grav || exit 0
