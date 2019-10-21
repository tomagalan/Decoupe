all: optimalCut

clean:
	-rm *.o optimalCut

optimalCut: optimalCut.o algorithms.o
	gcc -g -o optimalCut optimalCut.o algorithms.o -L /usr/local/include -l glpk

optimalCut.o: optimalCut.c algorithms.h
	gcc -g -o optimalCut.o -c optimalCut.c -W -Wall

algorithms.o: algorithms.c
	gcc -g -o algorithms.o -c algorithms.c -W -Wall
