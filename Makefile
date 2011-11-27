# Makefile for Robot Controller
# (C) 2005 Jason Hunt - nulluser@gmail.com
#

mnode:	mnode.o network.o
	gcc	mnode.o network.o -o mnode -O4 -l pthread
	#cp	server /bin/server

	
mnode.o:	mnode.c  mnode.h
	gcc	-c mnode.c -o mnode.o -O4


network.o:	network.c  network.h
	gcc	-c network.c -o network.o -O4

