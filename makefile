#makefile for main.c

CC=gcc
CGLAGS = -g -Wall

all: count

count: main.c
	$(CC) $(CFLAGS) -o count main.c

clean:
	rm count
