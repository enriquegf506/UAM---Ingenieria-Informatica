/** 
 * @brief It tests enemy module
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
#include "enemy_test.h"


#define MAX_TESTS 22 /*!< Macro for the maximum test*/

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
    printf("Running all test for module Enemy:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_enemy_create();
  if (all || test == 2) test2_enemy_create();
  if (all || test == 3) test1_enemy_get_location();
  if (all || test == 4) test2_enemy_get_location();
  if (all || test == 5) test1_enemy_set_location();
  if (all || test == 6) test2_enemy_set_location();
  if (all || test == 7) test1_enemy_set_id();
  if (all || test == 8) test2_enemy_set_id();
  if (all || test == 9) test1_enemy_get_id();
  if (all || test == 10) test2_enemy_get_id();
  if (all || test == 11) test1_enemy_set_health();
  if (all || test == 12) test2_enemy_set_health();
  if (all || test == 13) test1_enemy_get_health();
  if (all || test == 14) test2_enemy_get_health();
  if (all || test == 15) test2_enemy_get_name();
  if (all || test == 16) test2_enemy_get_name();
  if (all || test == 17) test2_enemy_set_name();
  if (all || test == 18) test2_enemy_set_name();
  if (all || test == 19) test2_enemy_print();
  if (all || test == 20) test2_enemy_print();
  if (all || test == 21) test2_enemy_destroy();
  if (all || test == 22) test2_enemy_destroy();
  PRINT_PASSED_PERCENTAGE;

  return 1;
}

void test1_enemy_create() {
  int result;
  Enemy *e;
  e = enemy_create(5);
  result=e!=NULL ;
  PRINT_TEST_RESULT(result);
  enemy_destroy(e);
}

void test2_enemy_create() {
  Enemy *e;
  e = enemy_create(4);
  PRINT_TEST_RESULT(enemy_get_id(e) == 4);
  enemy_destroy(e);
}

void test1_enemy_set_name() {
  Enemy *e;
  e = enemy_create(5);
  PRINT_TEST_RESULT(enemy_set_name(e, "hola") == OK);
  enemy_destroy(e);
}

void test2_enemy_set_name() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_set_name(e, "hola") == ERROR);
}

void test1_enemy_set_location() {
  Enemy *e;
  e = enemy_create(5);
  PRINT_TEST_RESULT(enemy_set_location(e, 4) == OK);
  enemy_destroy(e);
}

void test2_enemy_set_location() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_set_location(e, 4) == ERROR);
}

void test1_enemy_get_location() {
  Enemy *e;
  e = enemy_create(5);
  enemy_set_location(e, 6);
  PRINT_TEST_RESULT(enemy_get_location(e) == 6);
  enemy_destroy(e);
}

void test2_enemy_get_location() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_get_location(e) == NO_ID);
}

void test1_enemy_set_id() {
  Enemy *e;
  e = enemy_create(5);
  PRINT_TEST_RESULT(enemy_set_id(e, 4) == OK);
  enemy_destroy(e);
}

void test2_enemy_set_id() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_set_id(e, 4) == ERROR);
}

void test1_enemy_get_id() {
  Enemy *e;
  e = enemy_create(5);
  enemy_set_id(e, 6);
  PRINT_TEST_RESULT(enemy_get_id(e) == 6);
  enemy_destroy(e);
}

void test2_enemy_get_id() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_get_id(e) == NO_ID);
}

void test1_enemy_get_name() {
  Enemy *e;
  e = enemy_create(1);
  enemy_set_name(e, "adios");
  PRINT_TEST_RESULT(strcmp(enemy_get_name(e), "adios") == 0);
  enemy_destroy(e);
}

void test2_enemy_get_name() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_get_name(e) == NULL);
}

void test1_enemy_set_health() {
  Enemy *e;
  e = enemy_create(5);
  PRINT_TEST_RESULT(enemy_set_health(e, 4) == OK);
  enemy_destroy(e);
}

void test2_enemy_set_health(){
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_set_health(e, 4) == ERROR);
}

void test1_enemy_get_health() {
  Enemy *e;
  e = enemy_create(5);
  enemy_set_health(e, 6);
  PRINT_TEST_RESULT(enemy_get_health(e) == 6);
  enemy_destroy(e);
}

void test2_enemy_get_health() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_get_health(e) == -1);
}

void test1_enemy_print() {
  Enemy *e;
  e = enemy_create(5);
  PRINT_TEST_RESULT(enemy_print(e) == OK);
  enemy_destroy(e);
}

void test2_enemy_print(){
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_print(e) == ERROR);
}

void test1_enemy_destroy() {
  Enemy *e;
  e = enemy_create(5);
  PRINT_TEST_RESULT(enemy_destroy(e) == OK);
}

void test2_enemy_destroy() {
  Enemy *e = NULL;
  PRINT_TEST_RESULT(enemy_destroy(e) == ERROR);
}