/** 
 * @brief It tests game module
 * 
 * @file game_test.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 12-03-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "game_test.h"


#define MAX_TESTS 38  /*!< Macro for the maximum test*/

/** 
 * @brief Main function for GAME unit tests. 
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
    printf("Running all test for module Game:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }

  if (all || test == 1) test1_game_add_space();
  if (all || test == 2) test2_game_add_space();
  if (all || test == 3) test1_game_add_object();
  if (all || test == 4) test2_game_add_object();
  if (all || test == 5) test1_game_get_space_id_at();
  if (all || test == 6) test2_game_get_space_id_at();
  if (all || test == 7) test1_game_get_space();
  if (all || test == 8) test2_game_get_space();
  if (all || test == 9) test1_game_add_player();
  if (all || test == 10) test2_game_add_player();
  if (all || test == 11) test1_game_add_enemy();
  if (all || test == 12) test2_game_add_enemy();
  if (all || test == 13) test1_game_add_link();
  if (all || test == 14) test2_game_add_link();
  if (all || test == 15) test1_game_get_link();
  if (all || test == 16) test2_game_get_link();
  if (all || test == 17) test1_game_get_connection();
  if (all || test == 18) test2_game_get_connection();
  if (all || test == 19) test1_game_get_connection_status();
  if (all || test == 20) test2_game_get_connection_status();
  if (all || test == 21) test1_game_get_player();
  if (all || test == 22) test2_game_get_player();
  if (all || test == 21) test1_game_get_enemy();
  if (all || test == 22) test2_game_get_enemy();
  if (all || test == 23) test1_game_get_obj();
  if (all || test == 24) test2_game_get_obj();
  if (all || test == 25) test1_game_set_description();
  if (all || test == 26) test2_game_set_description();
  if (all || test == 27) test1_game_get_description();
  if (all || test == 28) test2_game_get_description();
  if (all || test == 29) test1_game_is_over();
  if (all || test == 30) test2_game_is_over();
  if (all || test == 31) test1_game_set_state();
  if (all || test == 32) test2_game_set_state();
  if (all || test == 33) test1_game_get_state();
  if (all || test == 34) test2_game_get_state();
  if (all || test == 35) test1_game_update();
  if (all || test == 36) test2_game_update();
  if (all || test == 37) test1_game_get_last_command();
  if (all || test == 38) test2_game_get_last_command();
  PRINT_PASSED_PERCENTAGE;

  return 1;
}

/**
   test1 for game_add_space
*/
void test1_game_add_space(){
  Game *g;
  Space *s;
  s=space_create(4);
  g=game_create();
  PRINT_TEST_RESULT(game_add_space(g,s)==OK);
}

/**
   test2 for game_add_space
*/
void test2_game_add_space(){
  Game *g;
  Space *s=NULL;
  g=game_create();
  PRINT_TEST_RESULT(game_add_space(g,s)==ERROR);
  game_destroy(g);
}

/**
   test1 for game_add_object
*/
void test1_game_add_object(){
  Game *g;
  Object *o;
  o=object_create(4);
  g=game_create();
  PRINT_TEST_RESULT(game_add_obj(g,o)==OK);
}

/**
   test2 for game_add_object
*/
void test2_game_add_object(){
  Game *g;
  Object *o=NULL;
  g=game_create();
  PRINT_TEST_RESULT(game_add_obj(g,o)==ERROR);
}

/**
   test1 for game_get_space_id_at
*/
void test1_game_get_space_id_at(){
  Game *g;  
  int pos=0;
  Space *s;
  g=game_create();
  s=space_create(3);
  game_add_space(g,s);
  PRINT_TEST_RESULT(game_get_space_id_at(g,pos)==3);
}

/**
   test2 for game_get_space_id_at
*/
void test2_game_get_space_id_at(){
  Game *g;
  int pos=-1;
  g=game_create();
  PRINT_TEST_RESULT(game_get_space_id_at(g,pos)==NO_ID);
}

/**
   test1 for game_get_space
*/
void test1_game_get_space(){
  Game *g;
  Space *s;
  g=game_create();
  s=space_create(4);
  game_add_space(g,s);
  PRINT_TEST_RESULT(game_get_space(g,4)==s);
}

/**
   test2 for game_get_space
*/
void test2_game_get_space(){
  Game *g;
  Space *s;
  g=game_create();
  s=space_create(4);
  game_add_space(g,s);
  PRINT_TEST_RESULT(game_get_space(g,1)==NULL);
}

/**
   test1 for game_add_player
*/
void test1_game_add_player(){
  Game *g;
  Player *p;
  p=player_create(4);
  g=game_create();
  PRINT_TEST_RESULT(game_add_player(g,p)==OK);
}

/**
   test2 for game_add_player
*/
void test2_game_add_player(){
  Game *g=NULL;
  Player *p=NULL;
  g=game_create();
  PRINT_TEST_RESULT(game_add_player(g,p)==ERROR);
}

/**
   test1 for game_add_enemy
*/
void test1_game_add_enemy(){
  Game *g;
  Enemy *e;
  e=enemy_create(4);
  g=game_create();
  PRINT_TEST_RESULT(game_add_enemy(g,e)==OK);
}

/**
   test2 for game_add_enemy
*/
void test2_game_add_enemy(){
  Game *g;
  Enemy *e=NULL;
  g=game_create();
  PRINT_TEST_RESULT(game_add_enemy(g,e)==ERROR);
}

/**
   test1 for game_add_link
*/
void test1_game_add_link(){
  Game *g=NULL;
  Link *l=NULL;
  l=link_create(6);
  g=game_create();
  PRINT_TEST_RESULT(game_add_link(g,l)==OK);
}

/**
   test2 for game_add_link
*/
void test2_game_add_link(){
  Game *g=NULL;
  Link *l=NULL;
  g=game_create();
  PRINT_TEST_RESULT(game_add_link(g,l)==ERROR);
}

/**
   test1 for game_get_link
*/
void test1_game_get_link(){
  Game *g;
  Link *l;
  g=game_create();
  l=link_create(4);
  link_set_origin(l,4);
  link_set_direction(l,N);
  game_add_link(g,l);
  PRINT_TEST_RESULT(game_get_link(g,4,N)==l);
}

/**
   test2 for game_get_link
*/
void test2_game_get_link(){
  Game *g=NULL;
  Link *l;
  l=link_create(4);
  link_set_origin(l,4);
  link_set_direction(l,N);
  PRINT_TEST_RESULT(game_get_link(g,4,N)== NULL);
}

/**
   test1 for game_get_connection
*/
void test1_game_get_connection(){
  Game *g=NULL;
  Link *l=NULL;
  g=game_create();
  link_set_dest(l,6);
  link_set_direction(l,N);
  game_add_link(g,l);
  PRINT_TEST_RESULT(game_get_connection(g,4,N)==NO_ID);
}

/**
   test2 for game_get_connection
*/
void test2_game_get_connection(){
  Game *g=NULL;
  Link *l;
  l=link_create(4);
  link_set_dest(l,5);
  link_set_direction(l,N);
  PRINT_TEST_RESULT(game_get_connection(g,4,N)==NO_ID);
}

/**
   test1 for game_get_connection_status
*/
void test1_game_get_connection_status(){
  Game *g=NULL;
  Link *l;
  g=game_create();
  l=link_create(4);
  link_set_direction(l,N);
  link_set_state(l,OPEN);
  game_add_link(g,l);
  PRINT_TEST_RESULT(game_get_connection_status(g,4,N)== -1);
}

/**
   test2 for game_get_connection_status
*/
void test2_game_get_connection_status(){
  Game *g=NULL;
  Link *l;
  l=link_create(4);
  link_set_state(l,CLOSE);
  link_set_direction(l,N);
  PRINT_TEST_RESULT(game_get_connection_status(g,4,N)==-1);
}

/**
   test1 for game_get_player
*/
void test1_game_get_player(){
  Game *g;
  Player *p;
  g=game_create();
  p=player_create(4);
  game_add_player(g,p);
  PRINT_TEST_RESULT(game_get_player(g)==p);
}

/**
   test2 for game_get_player
*/
void test2_game_get_player(){
  Game *g=NULL;
  Player *p=NULL;
  p=player_create(4);
  game_add_player(g,p);
  PRINT_TEST_RESULT(game_get_player(g)==NULL);
}

/**
   test1 for game_get_enemy
*/
void test1_game_get_enemy(){
  Game *g;
  Enemy *e;
  g=game_create();
  e=enemy_create(4);
  game_add_enemy(g,e);
  PRINT_TEST_RESULT(game_get_enemy(g)==e);
}

/**
   test2 for game_get_enemy
*/
void test2_game_get_enemy(){
  Game *g=NULL;
  Enemy *e;
  e=enemy_create(4);
  game_add_enemy(g,e);
  PRINT_TEST_RESULT(game_get_enemy(g)==NULL);
}

/**
   test1 for game_get_obj
*/
void test1_game_get_obj(){
  Game *g;
  Object *o;
  g=game_create();
  o=object_create(4);
  game_add_obj(g,o);
  PRINT_TEST_RESULT(game_get_obj(g,4)==o);
}

/**
   test2 for game_get_obj
*/
void test2_game_get_obj(){
  Game *g;
  Object *o;
  g=game_create();
  o=object_create(4);
  game_add_obj(g,o);
  PRINT_TEST_RESULT(game_get_obj(g,1)==NULL);
}

/**
   test1 for game_set_description
*/
void test1_game_set_description(){
  Game *g;
  g=game_create();
  PRINT_TEST_RESULT(game_set_description(g, "hola") == OK);
}

/**
   test2 for game_set_description
*/
void test2_game_set_description(){
  Game *g=NULL;
  PRINT_TEST_RESULT(game_set_description(g, "hola") == ERROR);
}

/**
   test1 for game_get_description
*/
void test1_game_get_description(){
  Game *g;
  g=game_create();
  game_set_description(g, "hola");
  PRINT_TEST_RESULT(strcmp(game_get_description(g), "hola") == 0);
}

/**
   test2 for game_get_description
*/
void test2_game_get_description(){
  Game *g=NULL;
  PRINT_TEST_RESULT(game_get_description(g) == NULL);
}

void test1_game_is_over(){
  Game *g;
  Player *p;
  g=game_create();
  p=player_create(5);
  player_set_health(p, 3);
  game_add_player(g, p);
  PRINT_TEST_RESULT(game_is_over(g) == FALSE);
}

/**
   test2 for game_is_over
*/
void test2_game_is_over(){
  Game *g;
  Player *p;
  g=game_create();
  p=player_create(5);
  player_set_health(p, 0);
  game_add_player(g, p);
  PRINT_TEST_RESULT(game_is_over(g) == TRUE);
}

/**
   test1 for game_set_state
*/
void test1_game_set_state(){
  Game *g;
  g=game_create();
  PRINT_TEST_RESULT(game_set_state(g, OK) == OK);
}

/**
   test2 for game_set_state
*/
void test2_game_set_state(){
  Game *g=NULL;
  PRINT_TEST_RESULT(game_set_state(g, OK) == ERROR);
}

/**
   test1 for game_get_state
*/
void test1_game_get_state(){
  Game *g;
  g=game_create();
  game_set_state(g, OK);
  PRINT_TEST_RESULT(game_get_state(g) == OK);
}

/**
   test2 for game_get_state
*/
void test2_game_get_state(){
  Game *g=NULL;
  PRINT_TEST_RESULT(game_get_state(g) == ERROR);
}

/**
   test1 for game_update
*/
void test1_game_update(){
  Game *g;
  g=game_create();
  PRINT_TEST_RESULT(game_update(g, ATTACK) == OK);
}

/**
   test2 for game_update
*/
void test2_game_update(){
  Game *g=NULL;
  PRINT_TEST_RESULT(game_update(g, ATTACK) == ERROR);
}

/**
   test1 for game_get_last_command
*/
void test1_game_get_last_command(){
  Game *g;
  g=game_create();
  game_update(g, ATTACK);
  PRINT_TEST_RESULT(game_get_last_command(g) == ATTACK);
}

/**
   test2 for game_update
*/
void test2_game_get_last_command(){
  Game *g=NULL;
  PRINT_TEST_RESULT(game_get_last_command(g) == UNKNOWN);
}
