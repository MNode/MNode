# Makefile for MNode
# (C) 2011 Jason Hunt - nulluser@gmail.com
#

include Makefile.inc

monitor:	monitor.o mnode.o datatap.o network.o  bin
	$(CC)	monitor.o mnode.o datatap.o network.o -o $(EXE) $(LIBS)

$(UI): bin
	cd ui; make

monitor.o:	monitor.c  monitor.h mnode.h network.h
	$(CC)	-c monitor.c -o monitor.o $(CFLAGS)

mnode.o:	mnode.c  mnode.h network.h
	$(CC)	-c mnode.c -o mnode.o $(CFLAGS)

datatap.o:	datatap.c  datatap.h network.h
	$(CC)	-c datatap.c -o datatap.o $(CFLAGS)

network.o:	network.c  network.h
	$(CC)	-c network.c -o network.o $(CFLAGS)

bin:
	mkdir bin

.PHONY: clean
clean:
	rm	-f *.o $(EXE) $(UI)

