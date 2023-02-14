CC=gcc
CFLAGS= -g -Wall -pedantic -ansi -I$(INC)
EXE = I3
SRC = src
OBJ = obj 
INC = include 
SRCS = command.c enemy.c game_reader.c game.c graphic_engine.c inventory.c link.c object.c player.c set.c space.c
OBJECTS = $(patsubst %, obj/%, $(notdir $(SRCS:.c=.o))) 


all: $(EXE)
tests: set_test space_test player_test enemy_test link_test game_test game_reader_test object_test inventory_test

$(EXE): $(OBJECTS) obj/game_loop.o
	$(CC) $(CFLAGS) -o $@ $^ -Llib -lscreen

set_test: obj/set_test.o obj/set.o
	$(CC) $(CFLAGS) -o $@ $^

space_test: obj/space_test.o obj/space.o obj/set.o
	$(CC) $(CFLAGS) -o $@ $^

player_test: obj/player_test.o obj/player.o obj/inventory.o obj/set.o obj/object.o
	$(CC) $(CFLAGS) -o $@ $^

enemy_test: obj/enemy_test.o obj/enemy.o obj/space.o obj/set.o
	$(CC) $(CFLAGS) -o $@ $^

link_test: obj/link_test.o obj/link.o
	$(CC) $(CFLAGS) -o $@ $^

game_test: obj/game_test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -Llib -lscreen

game_reader_test: obj/game_reader_test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -Llib -lscreen

object_test: obj/object_test.o obj/object.o
	$(CC) $(CFLAGS) -o $@ $^

inventory_test: obj/inventory_test.o obj/inventory.o obj/set.o
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o : src/%.c 
	$(CC) $(CFLAGS) -c $< -o $@


doxygen:
	cp -r ./src/*.c ./include/*.h ./
	doxygen Doxyfile; rm -rf *.c; rm -rf *.h;

.PHONY: clean fclean run vrun all tests doxygen

clean:
	rm -f obj/*.o

fclean: clean
	rm -rf $(EXE) set_test space_test enemy_test player_test link_test game_test inventory_test object_test game_reader_test doc

re: fclean all

run:
	@echo "Starting and running the game."
	./game_exe hormiguero.dat

vrun:
	@echo "Starting and running the game with valgrind."
	valgrind --leak-check=full -s --show-leak-kinds=all --track-origins=yes ./game_exe hormiguero.dat
