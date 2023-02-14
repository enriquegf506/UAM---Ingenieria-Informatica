all: p1_e1 p1_e2 p1_e3

CC = gcc
CFLAGS = -c -Wall -pedantic -g

p1_e1: p1_e1.o map.o point.o
	$(CC) -o$@ $^

p1_e2: p1_e2.o map.o point.o
	$(CC) -o$@ $^

p1_e3: p1_e3.o map.o point.o
	$(CC) -o$@ $^

valgrind1:
	valgrind ./p1_e1

valgrind2:
	valgrind ./p1_e2

valgrind3:
	valgrind ./p1_e3

p1_e1.o: p1_e1.c types.h point.h
	$(CC) $(CFLAGS) $<

p1_e2.o: p1_e2.c map.h types.h point.h
	$(CC) $(CFLAGS) $<

p1_e3.o: p1_e3.c map.h types.h point.h
	$(CC) $(CFLAGS) $<

point.o: point.c point.h types.h
	$(CC) $(CFLAGS) $<

map.o: map.c map.h point.h types.h
	$(CC) $(CFLAGS) $<

clean:
	@echo "Cleaning:"
	rm -rf *.o p1_e1 p1_e2 p1_e3