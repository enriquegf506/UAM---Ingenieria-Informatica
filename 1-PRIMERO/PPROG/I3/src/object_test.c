/** 
 * @brief It tests object modlule
 * 
 * @file player_test.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 04-04-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "object_test.h"


#define MAX_TESTS 18  /*!< Macro for the maximum test*/

/** 
 * @brief Main function for object unit tests. 
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
    printf("Running all test for module Object:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_object_create();
  if (all || test == 2) test2_object_create();
  if (all || test == 3) test1_object_set_name();
  if (all || test == 4) test2_object_set_name();
  if (all || test == 5) test1_object_get_name();
  if (all || test == 6) test2_object_get_name();
  if (all || test == 7) test1_object_get_id();
  if (all || test == 8) test2_object_get_id();
  if (all || test == 9) test1_object_set_id();
  if (all || test == 10) test2_object_set_id();
  if (all || test == 11) test1_object_destroy();
  if (all || test == 12) test2_object_destroy();
  if (all || test == 13) test1_object_print();
  if (all || test == 14) test2_object_print();
  if (all || test == 15) test1_object_set_description();
  if (all || test == 16) test2_object_set_description();
  if (all || test == 17) test1_object_get_description();
  if (all || test == 18) test2_object_get_description();

  PRINT_PASSED_PERCENTAGE;

  return 1;
}

void test1_object_create(){
  Object *o;
  o = object_create(4);
  PRINT_TEST_RESULT(object_get_id(o)==4);
  object_destroy(o);
}

void test2_object_create(){
  int result;
  Object *o;
  o = object_create(4);
  result=o!=NULL ;
  PRINT_TEST_RESULT(result);
  object_destroy(o);
}

void test1_object_set_name() {
  Object *o;
  o = object_create(5);
  PRINT_TEST_RESULT(object_set_name(o, "hola") == OK);
  object_destroy(o);
}

void test2_object_set_name() {
  Object *o = NULL;
  PRINT_TEST_RESULT(object_set_name(o, "hola") == ERROR);
}

void test1_object_get_name() {
  Object *o;
  o = object_create(1);
  object_set_name(o, "adios");
  PRINT_TEST_RESULT(strcmp(object_get_name(o), "adios") == 0);
  object_destroy(o);
}

void test2_object_get_name() {
  Object *o = NULL;
  PRINT_TEST_RESULT(object_get_name(o) == NULL);
}
  
void test1_object_get_id() {
  Object *o;
  o = object_create(5);
  object_set_id(o, 6);
  PRINT_TEST_RESULT(object_get_id(o) == 6);
  object_destroy(o);
}

void test2_object_get_id() {
  Object *o = NULL;
  PRINT_TEST_RESULT(object_get_id(o) == NO_ID);
}

void test1_object_set_id() {
  Object *o;
  o = object_create(4);
  PRINT_TEST_RESULT(object_set_id(o, 4) == OK);
  object_destroy(o);
}

void test2_object_set_id() {
  Object *o = NULL;
  PRINT_TEST_RESULT(object_set_id(o, 4) == ERROR);
}


void test1_object_destroy(){
  Object *o;
  o = object_create(4);
  PRINT_TEST_RESULT(object_destroy(o) == OK);
}

void test2_object_destroy(){
  Object *o = NULL;
  PRINT_TEST_RESULT(object_destroy(o) == ERROR);
}

void test1_object_print(){
  Object *o;
  o = object_create(4);
  PRINT_TEST_RESULT(object_print(o) == OK);
}

void test2_object_print(){
  Object *o = NULL;
  PRINT_TEST_RESULT(object_print(o) == ERROR);
}

/**
   test1 for object_get_description
*/
void test1_object_get_description(){
  Object *o;
  o=object_create(5);
  object_set_description(o, "hola");
  PRINT_TEST_RESULT(strcmp(object_get_description(o), "hola") == 0);
}

/**
   test2 for object_get_description
*/
void test2_object_get_description(){
  Object *o=NULL;
  PRINT_TEST_RESULT(object_get_description(o) == NULL);
}

/**
   test1 for object_set_description
*/
void test1_object_set_description(){
  Object *o;
  o=object_create(6);
  PRINT_TEST_RESULT(object_set_description(o, "hola") == OK);
}

/**
   test2 for object_set_description
*/
void test2_object_set_description(){
  Object *o=NULL;
  PRINT_TEST_RESULT(object_set_description(o, "hola") == ERROR);
}