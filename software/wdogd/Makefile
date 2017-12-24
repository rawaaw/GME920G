.DEFAULT_GOAL := all

CC=arm-linux-gcc
#CC=gcc
CFLAGS=-DDEBUG_ON=1 -O2 -D_GNU_SOURCE
#CFLAGS= -O2

wdogd.o:wdogd.c wdogd.h
	$(CC) -c $(CFLAGS) wdogd.c -o wdogd.o

wdogd:wdogd.o
	$(CC) -o wdogd wdogd.o

all:wdogd
	arm-linux-strip -s wdogd
	cp -f wdogd ../2.6.39/linux-at91

clean:
	rm -f *.o
	rm -f wdogd

	