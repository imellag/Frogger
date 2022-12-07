progetto.out: main.o marciapiede.o rana.o prato.o autostrada.o fiume.o
	gcc main.o marciapiede.o rana.o prato.o autostrada.o fiume.o -lncurses -o progetto.out
marciapiede.o: marciapiede.c marciapiede.h lib.h
	gcc -c marciapiede.c -lncurses
main.o: main.c lib.h
	gcc -c main.c -lncurses
rana.o: rana.c rana.h lib.h
	gcc -c rana.c -lncurses
prato.o: prato.c prato.h lib.h
	gcc -c prato.c -lncurses
autostrada.o: autostrada.c autostrada.h lib.h
	gcc -c autostrada.c -lncurses
fiume.o: fiume.c fiume.h lib.h
	gcc -c fiume.c -lncurses

clean:
	rm *.o
	rm *.out 
