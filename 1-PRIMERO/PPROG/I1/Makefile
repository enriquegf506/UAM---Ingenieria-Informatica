all: hormiguero

start : hormiguero
	./hormiguero hormiguero.dat 

hormiguero: command.o game_loop.o game.o graphic_engine.o space.o screen.o game_reader.o object.o player.o
	gcc -ohormiguero command.o game_loop.o game.o graphic_engine.o space.o screen.o game_reader.o object.o player.o
 
	
command.o: command.c command.h 
	gcc -c -Wall -pedantic command.c
	
game.o: game.c game.h game_reader.h
	gcc -c -Wall -pedantic game.c
	
space.o: space.c space.h
	gcc -c -Wall -pedantic space.c
	
graphic_engine.o: graphic_engine.c graphic_engine.h screen.h command.h
	gcc -c -Wall -pedantic graphic_engine.c
	
game_loop.o: game_loop.c graphic_engine.h game.h command.h
	gcc -c -Wall -pedantic game_loop.c
	
object.o: object.c object.h
	gcc -c -Wall -pedantic object.c
	
player.o: player.c player.h
	gcc -c -Wall -pedantic player.c
	
game_reader.o: game_reader.c game_reader.h
	gcc -c -Wall -pedantic game_reader.c

screen.o: screen.c screen.h
	gcc -c -Wall -pedantic screen.c
	
clean:
	rm -rf *o *gch	
