# -*- Makefile -*-
CC=gcc
CFLAGS=-std=c11 -Wall -Werror -g
OBJS=set.o graph.o BSTree.o readData.o mystring.o

scaledFootrule : scaledFootrule.o $(OBJS)
	gcc $(CFLAGS) scaledFootrule.o $(OBJS) -o scaledFootrule

searchPagerank : searchPagerank.o $(OBJS)
	gcc $(CFLAGS) searchPagerank.o $(OBJS) -o searchPagerank

searchTfIdf : searchTfIdf.o $(OBJS)
	gcc $(CFLAGS) searchTfIdf.o $(OBJS) -lm -o searchTfIdf

pagerank: pagerank.o $(OBJS)
	gcc $(CFLAGS) $(OBJS) pagerank.o -o pagerank

invertedIndex : invertedIndex.o $(OBJS)
	gcc $(CFLAGS) invertedIndex.o $(OBJS) -o invertedIndex

readData : $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o readData

scaledFootrule.o : scaledFootrule.c
	gcc $(CFLAGS) -c scaledFootrule.c

searchPagerank.o : searchPagerank.c 
	gcc $(CFLAGS) -c searchPagerank.c 

pagerank.o : pagerank.c 
	gcc $(CFLAGS) -c pagerank.c 

searchTfIdf.o : searchTfIdf.c 
	gcc $(CFLAGS) -c searchTfIdf.c 

invertedIndex.o : invertedIndex.c 
	gcc $(CFLAGS) -c invertedIndex.c 

readData.o : readData.c
	gcc $(CFLAGS) -c readData.c

graph.o : graph.c
	gcc $(CFLAGS) -c graph.c

set.o : set.c
	gcc $(CFLAGS) -c set.c

BSTree.o : BSTree.c
	gcc $(CFLAGS) -c BSTree.c

mystring.o : mystring.c 
	gcc $(CFLAGS) -c mystring.c

clean:
	rm -f $(OBJS) searchTfIdf.o invertedIndex.o searchPagerank.o scaledFootrule.o
