progetto.out: main.o marciapiede.o rana.o prato.o autostrada.o fiume.o tane.o funzioniGenerali.o collisioni.o avvio.o funzioniMain.o
	gcc main.o marciapiede.o rana.o prato.o autostrada.o fiume.o tane.o funzioniGenerali.o collisioni.o avvio.o funzioniMain.o -lncurses -o progetto.out
marciapiede.o: marciapiede.c marciapiede.h lib.h
	gcc -c marciapiede.c
main.o: main.c lib.h
	gcc -c main.c
rana.o: rana.c rana.h lib.h
	gcc -c rana.c
prato.o: prato.c prato.h lib.h
	gcc -c prato.c
autostrada.o: autostrada.c autostrada.h lib.h
	gcc -c autostrada.c
fiume.o: fiume.c fiume.h lib.h
	gcc -c fiume.c
tane.o: tane.c tane.h lib.h
	gcc -c tane.c
funzioniGenerali.o: funzioniGenerali.c funzioniGenerali.h lib.h
	gcc -c funzioniGenerali.c 
avvio.o: avvio.c avvio.h lib.h
	gcc -c avvio.c
collisioni.o: collisioni.c collisioni.h lib.h
	gcc -c collisioni.c
funzioniMain.o: funzioniMain.c funzioniMain.h lib.h
	gcc -c funzioniMain.c
clean:
	rm *.o
	rm *.out