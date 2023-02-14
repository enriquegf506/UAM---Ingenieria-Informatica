all: p2_e1a p2_e1b p2_e1c p2_e2 p2_e3

CC = gcc
CFLAGS = -c -Wall -pedantic -g

.PHONY: clean
.PHONY: valgrind1a
.PHONY: valgrind1b
.PHONY: valgrind1c
.PHONY: valgrind2
.PHONY: valgrind3

p2_e1a: p2_e1a.o point.o
	$(CC) -o$@ $^ -lm 

p2_e1b: p2_e1b.o point.o
	$(CC) -o$@ $^ -L. -lstack_fDoble -lm

p2_e1c: p2_e1c.o
	$(CC) -o$@ $^ -L. -lstack_fDoble -lm

p2_e2: p2_e2.o point.o map.o stack_fDoble.o
	$(CC) -o$@ $^ -L. -lstack_fDoble -lm

p2_e3: p2_e3.o point.o map.o stack_fDoble.o
	$(CC) -o$@ $^ -lm

p2_e1a.o: p2_e1a.c point.h types.h 
	$(CC) $(CFLAGS) $<

p2_e1b.o: p2_e1b.c point.h types.h
	$(CC) $(CFLAGS) $<

p2_e1c.o: p2_e1c.c point.h types.h
	$(CC) $(CFLAGS) $<

p2_e2.o: p2_e2.c stack_fDoble.h point.h map.h types.h
	$(CC) $(CFLAGS) $<

p2_e3.o: p2_e3.c stack_fDoble.h point.h map.h types.h
	$(CC) $(CFLAGS) $<

point.o: point.c point.h types.h
	$(CC) $(CFLAGS) $<

map.o: map.c map.h point.h types.h
	$(CC) $(CFLAGS) $<

stack_fDoble.o: stack_fDoble.c stack_fDoble.h types.h
	$(CC) $(CFLAGS) $<

# We introduce 5 just to check, this data can be modified

valgrind1a:
	valgrind --leak-check=full ./p2_e1a 5

valgrind1b:
	valgrind --leak-check=full ./p2_e1b 5

valgrind1c:
	valgrind --leak-check=full ./p2_e1c 5

valgrind2:
	valgrind --leak-check=full ./p2_e2 laberinto_2.txt

valgrind3:
	valgrind --leak-check=full ./p2_e3 laberinto_2.txt

clean:
	@echo "Cleaning:"
	rm -rf *.o p2_e1a p2_e1b p2_e1c p2_e2 p2_e3