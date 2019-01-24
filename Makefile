CFLAGS=-Wall -Wextra -pedantic -O2
FICHEIROS=cgi.h estado.c estado.h pathfinder.c pathfinder.h restart.c restart.h animation.c animation.h score.c score.h menu.h menu.c perihelion.c  Makefile

install: perihelion
	sudo cp perihelion /usr/lib/cgi-bin
	sudo cp -r images /var/www/html
	touch install
	sudo mkdir /var/www/html/ficheiros
	sudo chmod a+rw /var/www/html/ficheiros

perihelion: perihelion.o estado.o pathfinder.o restart.o animation.o score.o menu.o
	cc -o perihelion perihelion.o estado.o pathfinder.o restart.o animation.o score.o menu.o -lm

perihelion.zip: $(FICHEIROS)
	zip -9 perihelion.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o perihelion Doxyfile latex html install

estado.o: estado.c estado.h
restart.o: estado.h restart.c restart.h
animation.o: estado.h animation.c animation.h cgi.h restart.h
pathfinder.o: estado.h pathfinder.c pathfinder.h
score.o: cgi.h estado.h score.c score.h
menu.o: cgi.h estado.h menu.c menu.h
perihelion.o: perihelion.c cgi.h estado.h pathfinder.h restart.h score.h menu.h
