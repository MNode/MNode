# Makefile for MNode
# (C) 2005 Jason Hunt - nulluser@gmail.com
#

CFLAGS=-O4 -Wall
LIBS=-l pthread
EXE=mnode

mnode:	mnode.o network.o
	$(CC)	mnode.o network.o -o $(EXE) $(LIBS)

	
mnode.o:	mnode.c  mnode.h
	$(CC)	-c mnode.c -o mnode.o $(CFLAGS)


network.o:	network.c  network.h
	$(CC)	-c network.c -o network.o $(CFLAGS)

.PHONY: clean
clean:
	rm	-f *.o $(EXE)
