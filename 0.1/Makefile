COMPILE_ARGS = `sdl-config --cflags` `sdl-config --libs` -D NO_MAKEFILE
OPTI_ARGS = 
all: grav

camera.o:
	g++ ${OPTI_ARGS} -Wall -c camera.cpp

gravcalc.o:
	g++ ${OPTI_ARGS} -Wall -c gravcalc.cpp -D NO_MAKEFILE `sdl-config --cflags`
	
planetsim.o:
	g++ ${OPTI_ARGS} -Wall -c planetsim.cpp `sdl-config --cflags`

grav: clean gravcalc.o planetsim.o camera.o
	g++ ${OPTI_ARGS} -Wall ${COMPILE_ARGS} gravcalc.o planetsim.o camera.o main.cpp -o grav

clean:
	rm *.o grav || exit 0