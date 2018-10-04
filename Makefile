# Makefile for the farming calculator program. The program utilizes the libcs50 library, which
# is a library that I used for my CS50 class (Software Design and Implementation) at Dartmouth College
#
# Myles Holt July 2018

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I ../libcs50
PROG = farmingCalculator
OBJS = farmingCalculator.o

MAKE = make
LIBS = libcs50/libcs50.a

.PHONY: clean sourcelist

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)


farmingCalculator.o: libcs50/hashtable.h libcs50/webpage.h


clean:

	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f $(PROG)
