/** 
 * @brief It tests inventory module
 * 
 * @file inventory_test.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 12-03-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "inventory_test.h"


#define MAX_TESTS 14  /*!< Macro for the maximum test*/

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
    printf("Running all test for module Inventory:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_inventory_add();
  if (all || test == 2) test2_inventory_add();
  if (all || test == 3) test1_inventory_delete();
  if (all || test == 4) test2_inventory_delete();
  if (all || test == 5) test1_inventory_get_nids();
  if (all || test == 6) test2_inventory_get_nids();
  if (all || test == 7) test1_inventory_get_max();
  if (all || test == 8) test2_inventory_get_max();
  if (all || test == 9) test1_inventory_destroy();
  if (all || test == 10) test2_inventory_destroy();
  if (all || test == 11) test1_inventory_set_max();
  if (all || test == 12) test2_inventory_set_max();
  if (all || test == 13) test1_inventory_findId();
  if (all || test == 14) test2_inventory_findId();
  PRINT_PASSED_PERCENTAGE;

  return 1;
}


void test1_inventory_add() {
  Inventory *i;
  i = inventory_create();
  PRINT_TEST_RESULT(inventory_add(i, 5) == OK);
  inventory_destroy(i);
}

void test2_inventory_add() {
  Inventory *i= NULL;
  PRINT_TEST_RESULT(inventory_add(i, 5) == ERROR);
}


void test1_inventory_delete() {
  Inventory *i;
  i = inventory_create();
  inventory_add(i,5);
  PRINT_TEST_RESULT(inventory_delete(i, 5) == OK);
  inventory_destroy(i);
}

void test2_inventory_delete() {
  Inventory *i = NULL;
  PRINT_TEST_RESULT(inventory_delete(i, 5) == ERROR);
}


void test1_inventory_get_nids() {
  Inventory *i;
  i = inventory_create();
  PRINT_TEST_RESULT(inventory_get_nids(i) == 0);
}

void test2_inventory_get_nids() {
  Inventory *i = NULL;
  PRINT_TEST_RESULT(inventory_get_nids(i) == -1);
  inventory_destroy(i);
}

void test1_inventory_get_max() {
  Inventory *i;
  i = inventory_create();
  inventory_set_max(i,4);
  PRINT_TEST_RESULT(inventory_get_max(i) == 4);
}

void test2_inventory_get_max() {
  Inventory *i = NULL;
  PRINT_TEST_RESULT(inventory_get_max(i) == -1);
  inventory_destroy(i);
}

void test1_inventory_destroy(){
  Inventory *i;
  i = inventory_create();
  PRINT_TEST_RESULT(inventory_destroy(i) == OK);
}

void test2_inventory_destroy(){
  Inventory *i = NULL;
  PRINT_TEST_RESULT(inventory_destroy(i) == ERROR);
}

void test1_inventory_set_max(){
  Inventory *i;
  i = inventory_create();
  PRINT_TEST_RESULT(inventory_set_max(i,4) == OK);
  inventory_destroy(i);
}

void test2_inventory_set_max(){
  Inventory *i = NULL;
  PRINT_TEST_RESULT(inventory_set_max(i,4) == ERROR);
}

void test1_inventory_findId(){
  Inventory *i = NULL;
  PRINT_TEST_RESULT(inventory_findId(i,4) == FALSE);
}

void test2_inventory_findId(){
  Inventory *i;
  i=inventory_create();
  inventory_add(i, 4);
  PRINT_TEST_RESULT(inventory_findId(i, 4) == TRUE);
  inventory_destroy(i);
}

