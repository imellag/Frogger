progetto.o: main.o marciapiede.o
	gcc main.o marciapiede.o -lncurses -o progetto
marciapiede.o: marciapiede.c marciapiede.h lib.h
	gcc -c marciapiede.c -lncurses
main.o: main.c lib.h
	gcc -c main.c -lncurses


clean:
	rm *.o
	rm *.out