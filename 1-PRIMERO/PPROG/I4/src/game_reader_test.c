/** 
 * @brief It tests game_reader module
 * 
 * @file enemy_test.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 31-03-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "game_reader_test.h"


#define MAX_TESTS 18 /*!< Macro for the maximum test*/

/** 
 * @brief Main function for SET unit tests. 
 * 
 * You may execute ALL or a SINGLE test
 *   1.- No parameter -> ALL test are executed 
 *   2.- A number means a particular test (the one identified by that number) 
 *       is executed
 *  
 */
int main(int argc, char** argv) {

  int test = 0;
  int all = 1;

  if (argc < 2) {
    printf("Running all test for module Game_reader:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_game_reader_load_spaces();
  if (all || test == 2) test2_game_reader_load_spaces();
  if (all || test == 3) test1_game_reader_load_objects();
  if (all || test == 4) test2_game_reader_load_objects();
  if (all || test == 5) test1_game_reader_load_players();
  if (all || test == 6) test2_game_reader_load_players();
  if (all || test == 7) test1_game_reader_load_enemies();
  if (all || test == 8) test2_game_reader_load_enemies();
  if (all || test == 9) test1_game_reader_load_links();
  if (all || test == 10) test2_game_reader_load_links();
  PRINT_PASSED_PERCENTAGE;

  return 1;
}

void test1_game_reader_load_spaces(){
    Game *game = game_create();
    PRINT_TEST_RESULT(game_load_spaces(game,"hormiguero.dat")==OK);
    game_destroy(game);
}

void test2_game_reader_load_spaces(){
    Game *game = NULL;
    char *n = NULL;
    game = game_create();
    PRINT_TEST_RESULT(game_load_spaces(game,n)==ERROR);
    game_destroy(game);
}

void test1_game_reader_load_objects(){
    Game *game = game_create();
    PRINT_TEST_RESULT(game_load_objects(game,"hormiguero.dat")==OK);
    game_destroy(game);
}

void test2_game_reader_load_objects(){
    Game *game = NULL;
    char *n = NULL;
    game = game_create();
    PRINT_TEST_RESULT(game_load_objects(game,n)==ERROR);
    game_destroy(game);
}

void test1_game_reader_load_players(){
    Game *game = game_create();
    PRINT_TEST_RESULT(game_load_players(game,"hormiguero.dat")==OK);
    game_destroy(game);
}

void test2_game_reader_load_players(){
    Game *game = NULL;
    char *n = NULL;
    game = game_create();
    PRINT_TEST_RESULT(game_load_players(game,n)==ERROR);
    game_destroy(game);
}

void test1_game_reader_load_links(){
    Game *game = game_create();
    PRINT_TEST_RESULT(game_load_links(game,"hormiguero.dat")==OK);
    game_destroy(game);
}

void test2_game_reader_load_links(){
    Game *game = NULL;
    char *n = NULL;
    game = game_create();
    PRINT_TEST_RESULT(game_load_links(game,n)==ERROR);
    game_destroy(game);
}

void test1_game_reader_load_enemies(){
    Game *game = game_create();
    PRINT_TEST_RESULT(game_load_enemies(game,"hormiguero.dat")==OK);
    game_destroy(game);
}

void test2_game_reader_load_enemies(){
    Game *game = NULL;
    char *n = NULL;
    game = game_create();
    PRINT_TEST_RESULT(game_load_enemies(game,n)==ERROR);
    game_destroy(game);
}
