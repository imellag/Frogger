progetto.out: main.o marciapiede.o rana.o
	gcc main.o marciapiede.o rana.o -lncurses -o progetto.out
marciapiede.o: marciapiede.c marciapiede.h lib.h
	gcc -c marciapiede.c -lncurses
main.o: main.c lib.h
	gcc -c main.c -lncurses
rana.o: rana.c rana.h lib.h
	gcc -c rana.c -lncurses
	
clean:
	rm *.o
	rm *.out 
