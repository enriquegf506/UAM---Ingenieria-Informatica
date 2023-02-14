/** 
 * @brief It tests player modlule
 * 
 * @file player_test.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 31-03-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "player_test.h"


#define MAX_TESTS 32  /*!< Number of tests done*/

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
    printf("Running all test for module Player:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_player_create();
  if (all || test == 2) test2_player_create();
  if (all || test == 3) test1_player_get_location();
  if (all || test == 4) test2_player_get_location();
  if (all || test == 5) test1_player_set_location();
  if (all || test == 6) test2_player_set_location();
  if (all || test == 7) test1_player_get_id();
  if (all || test == 8) test2_player_get_id();
  if (all || test == 9) test1_player_set_health();
  if (all || test == 10) test2_player_set_health();
  if (all || test == 11) test1_player_get_health();
  if (all || test == 12) test2_player_get_health();
  if (all || test == 13) test1_player_get_name();
  if (all || test == 14) test2_player_get_name();
  if (all || test == 15) test1_player_set_name();
  if (all || test == 16) test2_player_set_name();
  if (all || test == 17) test1_player_get_object();
  if (all || test == 18) test2_player_get_object();
  if (all || test == 19) test1_player_destroy();
  if (all || test == 20) test2_player_destroy();
  if (all || test == 21) test1_player_print();
  if (all || test == 22) test2_player_print();
  if (all || test == 23) test1_player_contain_object();
  if (all || test == 24) test2_player_contain_object();
  if (all || test == 25) test1_player_del_object();
  if (all || test == 26) test2_player_del_object();
  if (all || test == 27) test1_player_set_max();
  if (all || test == 28) test2_player_set_max();
  if (all || test == 29) test1_player_get_max();
  if (all || test == 30) test2_player_get_max();
  if (all || test == 31) test1_player_add_object();
  if (all || test == 32) test2_player_add_object();
  PRINT_PASSED_PERCENTAGE;

  return 1;
}

void test1_player_create() {
  int result;
  Player *p;
  p = player_create(5);
  result=p!=NULL ;
  PRINT_TEST_RESULT(result);
  player_destroy(p);
}
void test2_player_create() {
  Player *p;
  p = player_create(4);
  PRINT_TEST_RESULT(player_get_id(p) == 4);
  player_destroy(p);
}

void test1_player_set_name() {
  Player *p;
  p = player_create(5);
  PRINT_TEST_RESULT(player_set_name(p, "hola") == OK);
  player_destroy(p);
}

void test2_player_set_name() {
  Player *p = NULL;
  PRINT_TEST_RESULT(player_set_name(p, "hola") == ERROR);
}

void test1_player_set_location() {
  Player *p;
  p = player_create(5);
  PRINT_TEST_RESULT(player_set_location(p, 4) == OK);
  player_destroy(p);
}

void test2_player_set_location() {
  Player *p = NULL;
  PRINT_TEST_RESULT(player_set_location(p, 4) == ERROR);
}

void test1_player_get_location() {
  Player *p;
  p = player_create(5);
  player_set_location(p, 6);
  PRINT_TEST_RESULT(player_get_location(p) == 6);
  player_destroy(p);
}

void test2_player_get_location() {
  Player *p = NULL;
  PRINT_TEST_RESULT(player_get_location(p) == NO_ID);
}


void test1_player_get_id() {
  Player *p;
  p = player_create(5);
  player_set_location(p, 6);
  PRINT_TEST_RESULT(player_get_id(p) == 5);
  player_destroy(p);
}

void test2_player_get_id() {
  Player *p = NULL;
  PRINT_TEST_RESULT(player_get_id(p) == NO_ID);
}

void test1_player_get_name() {
  Player *p;
  p = player_create(1);
  player_set_name(p, "adios");
  PRINT_TEST_RESULT(strcmp(player_get_name(p), "adios") == 0);
  player_destroy(p);
}

void test2_player_get_name() {
  Player *p = NULL;
  PRINT_TEST_RESULT(player_get_name(p) == NULL);
}

void test1_player_set_health() {
  Player *p;
  p = player_create(5);
  PRINT_TEST_RESULT(player_set_health(p, 4) == OK);
  player_destroy(p);
}

void test2_player_set_health(){
  Player *p = NULL;
  PRINT_TEST_RESULT(player_set_health(p, 4) == ERROR);
}

void test1_player_get_health() {
  Player *p;
  p = player_create(5);
  player_set_health(p, 6);
  PRINT_TEST_RESULT(player_get_health(p) == 6);
  player_destroy(p);
}

void test2_player_get_health() {
  Player *p = NULL;
  PRINT_TEST_RESULT(player_get_health(p) == -1);
}

void test1_player_get_object(){
  Player *p;
  Inventory *i;
  p = player_create(5);
  i=inventory_create();
  PRINT_TEST_RESULT(player_get_objects(p) != i);
  player_destroy(p);
  inventory_destroy(i);
}

void test2_player_get_object(){
  Player *p = NULL;
  PRINT_TEST_RESULT(player_get_objects(p) == NULL);
}

void test1_player_destroy(){
  Player *p;
  p = player_create(4);
  PRINT_TEST_RESULT(player_destroy(p) == OK);
}

void test2_player_destroy(){
  Player *p = NULL;
  PRINT_TEST_RESULT(player_destroy(p) == ERROR);
}

void test1_player_print(){
  Player *p;
  p = player_create(4);
  PRINT_TEST_RESULT(player_print(p) == OK);
}

void test2_player_print(){
  Player *p = NULL;
  PRINT_TEST_RESULT(player_print(p) == ERROR);
}

void test1_player_contain_object(){
  Player *p;
  p = player_create(3);
  player_add_object(p,6);
  PRINT_TEST_RESULT(player_contain_object(p,6)== TRUE);
  player_destroy(p);
}

void test2_player_contain_object(){
  Player *p=NULL;
  PRINT_TEST_RESULT(player_contain_object(p,6)== FALSE);
}

void test1_player_del_object(){
  Player *p;
  p = player_create(3);
  player_add_object(p,4);
  PRINT_TEST_RESULT(player_del_object(p,4) == OK);
  player_destroy(p);
}

void test2_player_del_object(){
  Player *p = NULL;
  PRINT_TEST_RESULT(player_del_object(p,6) == ERROR);
}

void test1_player_set_max() {
  Player *p;
  p = player_create(5);
  PRINT_TEST_RESULT(player_set_max(p, 4) == OK);
  player_destroy(p);
}

void test2_player_set_max(){
  Player *p = NULL;
  PRINT_TEST_RESULT(player_set_max(p, 4) == ERROR);
}

void test1_player_get_max() {
  Player *p;
  p = player_create(5);
  player_set_max(p, 6);
  PRINT_TEST_RESULT(player_get_max(p) == 6);
  player_destroy(p);
}

void test2_player_get_max() {
  Player *p = NULL;
  PRINT_TEST_RESULT(player_get_max(p) == -1);
}

void test1_player_add_object(){
  Player *p;
  p = player_create(9);
  PRINT_TEST_RESULT(player_add_object(p,6)== OK);
  player_destroy(p);
}

void test2_player_add_object(){
  Player *p=NULL;
  PRINT_TEST_RESULT(player_add_object(p,6)== ERROR);
}
