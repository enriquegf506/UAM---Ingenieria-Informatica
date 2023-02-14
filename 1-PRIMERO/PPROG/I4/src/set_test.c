/** 
 * @brief It tests set module
 * 
 * @file set_test.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 12-03-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "set_test.h"


#define MAX_TESTS 14  /*!< Number of tests done*/

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
    printf("Running all test for module Set:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_set_create();
  if (all || test == 2) test2_set_create();
  if (all || test == 3) test1_set_add();
  if (all || test == 4) test2_set_add();
  if (all || test == 5) test1_set_del();
  if (all || test == 6) test2_set_del();
  if (all || test == 7) test1_set_getNids();
  if (all || test == 8) test2_set_getNids();
  if (all || test == 9) test1_set_getId();
  if (all || test == 10) test2_set_getId();
  if (all || test == 11) test1_set_destroy();
  if (all || test == 12) test2_set_destroy();
  if (all || test == 13) test1_set_findId();
  if (all || test == 14) test2_set_findId();
 /* if (all || test == 15) test1_set_print();
  if (all || test == 16) test2_set_print();*/
  PRINT_PASSED_PERCENTAGE;

  return 1;
}

void test1_set_create() {
  int result;
  Set *s;
  s = set_create(1);
  result=s!=NULL ;
  PRINT_TEST_RESULT(result);
  set_destroy(s);
}

void test2_set_create(){
  int result;
  Set *s;
  result=4;
  s = set_create(result);
  PRINT_TEST_RESULT(4);
  set_destroy(s);
}

void test1_set_add() {
  Set *s;
  s = set_create(1);
  PRINT_TEST_RESULT(set_add(s, 5) == OK);
  set_destroy(s);
}

void test2_set_add() {
  Set *s = NULL;
  PRINT_TEST_RESULT(set_add(s, 5) == ERROR);
}


void test1_set_del() {
  Set *s;
  s = set_create(1);
  set_add(s,5);
  PRINT_TEST_RESULT(set_delete(s, 5) == OK);
  set_destroy(s);
}

void test2_set_del() {
  Set *s = NULL;
  PRINT_TEST_RESULT(set_delete(s, 5) == ERROR);
}


void test1_set_getNids() {
  Set *s;
  s = set_create(1);
  PRINT_TEST_RESULT(set_getNids(s) == 0);
}

void test2_set_getNids() {
  Set *s = NULL;
  PRINT_TEST_RESULT(set_getNids(s) == -1);
  set_destroy(s);
}

void test1_set_getId() {
  Set *s;
  s = set_create(1);
  set_add(s,4);
  PRINT_TEST_RESULT(set_getIds(s) != NULL);
}

void test2_set_getId() {
  Set *s = NULL;
  PRINT_TEST_RESULT(set_getIds(s) == NULL);
  set_destroy(s);
}

void test1_set_destroy(){
  Set *s;
  s = set_create(1);
  PRINT_TEST_RESULT(set_destroy(s) == OK);
}

void test2_set_destroy(){
  Set *s = NULL;
  PRINT_TEST_RESULT(set_destroy(s) == ERROR);
}

void test1_set_findId(){
  Set *s;
  s = set_create(1);
  PRINT_TEST_RESULT(set_findId(s,0) == FALSE);
}

void test2_set_findId(){
  Set *s = NULL;
  s = set_create(1);
  PRINT_TEST_RESULT(set_findId(s,1) == FALSE);
}
/*
void test1_set_print(){
  Set *s;
  FILE *f;
  s = set_create(1);
  PRINT_TEST_RESULT(set_print(f,s) == OK);
}

void test2_set_print(){
  Set *s = NULL;
  FILE *f;
  PRINT_TEST_RESULT(set_print(f,s) == ERROR);
}*/
