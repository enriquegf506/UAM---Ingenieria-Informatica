CC=gcc
CFLAGS= -g -Wall -pedantic
EJS = 13_a
OBJECTS13_a = 13_a.o mining.o pow.o


all: $(EJS) clear

13_a: $(OBJECTS13_a)
	$(CC) $(CFLAGS) -o 13_a $(OBJECTS13_a) -lpthread

13_a.o: 13_a.c mining.h pow.h
	$(CC) $(CFLAGS) -c 13_a.c

mining.o: mining.c mining.h pow.h
	$(CC) $(CFLAGS) -c mining.c

pow.o: pow.c pow.h
	$(CC) $(CFLAGS) -c pow.c

clear:
	rm -rf *.o 

clean:
	rm -rf *.o $(EJS)