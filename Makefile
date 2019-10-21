all: optimalCut

clean:
	-rm *.o optimalCut

optimalCut: optimalCut.o algorithms.o
	gcc -o optimalCut optimalCut.o algorithms.o -L /usr/local/include -l glpk

optimalCut.o: optimalCut.c algorithms.h
	gcc -o optimalCut.o -c optimalCut.c -W -Wall

algorithms.o: algorithms.c
	gcc -o algorithms.o -c algorithms.c -W -Wall 
