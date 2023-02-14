all: p4_exe

CC = gcc
CFLAGS = -c -Wall -pedantic -g 

.PHONY: clean
.PHONY: valgrind

p4_exe: p4.o bstree.o point.o
	$(CC) -o$@ $^ -lm

p4.o: p4.c point.h bstree.h
	$(CC) $(CFLAGS) $< 

bstree.o: bstree.c bstree.h types.h
	$(CC) $(CFLAGS) $<

point.o: point.c point.h types.h
	$(CC) $(CFLAGS) $< 

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./p4_exe tree_example.txt
	
clean:
	rm -rf *.o p4_exe