CC	= gcc -o
CFLAGS	= -c -pedantic -Wall -g -L -lscreen
EXE = I2
OBJS = command.o game_loop.o game.o graphic_engine.o space.o game_reader.o player.o object.o set.o enemy.o 
.PHONY: clean All cleand

All: $(EXE) SpaceTest SetTest EnemyTest cleand

I2: $(OBJS) 
	$(CC) $(EXE) $(OBJS) libscreen.a

%.o : %.c
	$(CC) -MD $(CFLAGS) $< -o $@ 
-include *.d

SpaceTest: space_test.o space.o set.o
	$(CC) space_test space_test.o space.o set.o 

EnemyTest: enemy_test.o enemy.o
	$(CC) enemy_test enemy_test.o enemy.o

SetTest: set_test.o set.o
	$(CC) set_test set_test.o set.o 

programa: I2
	./I2 hormiguero.dat
	
clean: 
	@rm  -f *.o

cleand: 
	@rm  -f *.d
  
