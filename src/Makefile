LIBS =  `sdl-config --libs` -lpthread -lboost_thread
FLAGS = `sdl-config --cflags` -D CORECOUNT=2 -O2 
all: grav

camera.o:
	g++ ${FLAGS} -Wall camera.cpp -c

gravenv.o:
	g++ ${FLAGS} -Wall -D NO_MAKEFILE gravenv.cpp -c

planet.o:
	g++ ${FLAGS} -Wall planet.cpp -c

main.o:
	g++ ${FLAGS} -Wall main.cpp -c

grav: camera.o gravenv.o planet.o main.o
	g++ ${FLAGS} -Wall ${LIBS} camera.o planet.o gravenv.o main.o -o grav

clean:
	rm *.o grav || exit 0
