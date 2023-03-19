progetto.out: main.o rana.o autostrada.o fiume.o funzioniGenerali.o collisioni.o avvio.o funzioniMain.o areaDisegno.o areaGioco.o
	gcc main.o rana.o autostrada.o fiume.o funzioniGenerali.o collisioni.o avvio.o funzioniMain.o areaDisegno.o areaGioco.o -lncursesw -lpthread -o progetto.out
main.o: main.c lib.h
	gcc -c main.c
rana.o: rana.c rana.h lib.h
	gcc -c rana.c
autostrada.o: autostrada.c autostrada.h lib.h
	gcc -c autostrada.c
fiume.o: fiume.c fiume.h lib.h
	gcc -c fiume.c
funzioniGenerali.o: funzioniGenerali.c funzioniGenerali.h lib.h
	gcc -c funzioniGenerali.c 
avvio.o: avvio.c avvio.h lib.h
	gcc -c avvio.c
collisioni.o: collisioni.c collisioni.h lib.h
	gcc -c collisioni.c
funzioniMain.o: funzioniMain.c funzioniMain.h lib.h
	gcc -c funzioniMain.c
areaDisegno.o: areaDisegno.c areaDisegno.h lib.h
	gcc -c areaDisegno.c
areaGioco.o: areaGioco.c areaGioco.h lib.h
	gcc -c areaGioco.c
clean:
	rm *.o
	rm *.out