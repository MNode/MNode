# Makefile for Robot Controller
# (C) 2005 Jason Hunt - nulluser@gmail.com
#
CFLAGS=-O4
LIBS=-l pthread
EXE=mnode

mnode:	mnode.o network.o
	$(CC)	mnode.o network.o -o $(EXE) $(LIBS)
	#cp	server /bin/server

	
mnode.o:	mnode.c  mnode.h
<<<<<<< HEAD:Makefile
	gcc	-c mnode.c -o mnode.o -O4 -Wall


network.o:	network.c  network.h
	gcc	-c network.c -o network.o -O4 -Wall
=======
	$(CC)	-c mnode.c -o mnode.o $(CFLAGS)


network.o:	network.c  network.h
	$(CC)	-c network.c -o network.o $(CFLAGS)
>>>>>>> b1ead17eda875a034ab87687408d4e9f75a11515:Makefile

.PHONY: clean
clean:
	rm	-f *.o $(EXE)
