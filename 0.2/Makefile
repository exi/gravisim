LIBS =  `sdl-config --libs` -lpthread
FLAGS = `sdl-config --cflags`
all: grav

camera.o:
	g++ ${FLAGS} -Wall camera.cpp -c

gravenv.o:
	g++ ${FLAGS} -Wall -D NO_MAKEFILE gravenv.cpp -c

planet.o:
	g++ ${FLAGS} -Wall planet.cpp -c

grav: clean camera.o gravenv.o planet.o
	g++ ${FLAGS} -Wall ${LIBS} camera.o planet.o gravenv.o main.cpp -o grav

clean:
	rm *.o grav || exit 0