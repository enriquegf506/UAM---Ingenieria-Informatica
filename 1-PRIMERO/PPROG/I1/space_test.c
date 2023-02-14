/** 
 * @brief It tests space module
 * 
 * @file space_test.c
 * @author Profesores Pprog
 * @version 3.0 
 * @date 09-03-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "space.h"
#include "space_test.h"
#include "test.h"

#define MAX_TESTS 28

/** 
 * @brief Main function for SPACE unit tests. 
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
    printf("Running all test for module Space:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_space_create();
  if (all || test == 2) test2_space_create();
  if (all || test == 3) test1_space_set_name();
  if (all || test == 4) test2_space_set_name();
  if (all || test == 5) test3_space_set_name();
  if (all || test == 6) test1_space_set_north();
  if (all || test == 7) test2_space_set_north();
  if (all || test == 8) test1_space_set_south();
  if (all || test == 9) test2_space_set_south();
  if (all || test == 10) test1_space_set_east();
  if (all || test == 11) test2_space_set_east();
  if (all || test == 12) test1_space_set_west();
  if (all || test == 13) test2_space_set_west();
  if (all || test == 14) test1_space_get_name();
  if (all || test == 15) test2_space_get_name();
  if (all || test == 16) test1_space_get_object();
  if (all || test == 17) test2_space_get_object();
  if (all || test == 18) test3_space_get_object();
  if (all || test == 19) test1_space_get_north();
  if (all || test == 20) test2_space_get_north();
  if (all || test == 21) test1_space_get_south();
  if (all || test == 22) test2_space_get_south();
  if (all || test == 23) test1_space_get_east();
  if (all || test == 24) test2_space_get_east();
  if (all || test == 25) test1_space_get_west();
  if (all || test == 26) test2_space_get_west();
  if (all || test == 27) test1_space_get_id();
  if (all || test == 28) test2_space_get_id();

  PRINT_PASSED_PERCENTAGE;

  return 1;
}
/**
 * test space creation
 */
 
void test1_space_create() {
  int result;
  Space *s;
  s = space_create(5);
  result=s!=NULL ;
  PRINT_TEST_RESULT(result);
  space_destroy(s);
}
/**
 * test second space creation
 */
void test2_space_create() {
  Space *s;
  s = space_create(4);
  PRINT_TEST_RESULT(space_get_id(s) == 4);
  space_destroy(s);
}
/**
 * test the function space set name
 */
void test1_space_set_name() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_name(s, "hola") == OK);
  space_destroy(s);
}
/**
 * test the second function space set name
 */
void test2_space_set_name() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_name(s, "hola") == ERROR);
}
/**
 * test the third function space set name
 */
void test3_space_set_name() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_name(s, NULL) == ERROR);
  space_destroy(s);
}
/**
 * test the function for setting the north
 */
void test1_space_set_north() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_north(s, 4) == OK);
  space_destroy(s);
}
/**
 * test the second function for setting the north
 */ 
void test2_space_set_north() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_north(s, 4) == ERROR);
}
/**
 * test the function for setting the south
 */
void test1_space_set_south() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_south(s, 4) == OK);
  space_destroy(s);
}
/**
 * test the second function for setting the south
 */
void test2_space_set_south() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_south(s, 4) == ERROR);
}
/**
 * test the function for setting the seat
 */
void test1_space_set_east() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_east(s, 4) == OK);
  space_destroy(s);
}
/**
 * test the second function for setting the east
 */
void test2_space_set_east() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_east(s, 4) == ERROR);
}
/** 
 * test the function for setting the west
 */
void test1_space_set_west() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_west(s, 4) == OK);
  space_destroy(s);
}
/** 
 * test the second function for setting the west
 */
void test2_space_set_west() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_west(s, 4) == ERROR);
}
/**
 * test the function for setting the object in the space
 */
void test1_space_set_object() {
  Space *s;
  s = space_create(1);
  PRINT_TEST_RESULT(space_set_object(s,TRUE) == OK);
  space_destroy(s);
}
/**
 * test the second function for setting the object in the space
 */
void test2_space_set_object() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_object(s,TRUE) == ERROR);
}
/**
 * test the function for getting the space name
 */
void test1_space_get_name() {
  Space *s;
  s = space_create(1);
  space_set_name(s, "adios");
  PRINT_TEST_RESULT(strcmp(space_get_name(s), "adios") == 0);
  space_destroy(s);
}
/**
 * test the second function for getting the space name
 */
void test2_space_get_name() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_name(s) == NULL);
}
/**
 * test the function for getting the object in the space
 */
void test1_space_get_object() {
  Space *s;
  s = space_create(1);
  PRINT_TEST_RESULT(space_get_object(s) == FALSE);
  space_destroy(s);
}
/**
 * test the secong function for getting the object in the space
 */
void test2_space_get_object() {
  Space *s;
  s = space_create(1);
  space_set_object(s,TRUE);
  PRINT_TEST_RESULT(space_get_object(s) == TRUE);
  space_destroy(s);  
}
/**
 * teste the third function for getting the object in the space
 */
void test3_space_get_object() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_object(s) == FALSE);
}
/**
 * test the function for getting the north of the space
 */
void test1_space_get_north() {
  Space *s;
  s = space_create(5);
  space_set_north(s, 4);
  PRINT_TEST_RESULT(space_get_north(s) == 4);
  space_destroy(s);
}
/**
 * test the function for getting the north of the space
 */
void test2_space_get_north() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_north(s) == NO_ID);
}
/**
 * test the function for getting the south of the space
 */
void test1_space_get_south() {
  Space *s;
  s = space_create(5);
  space_set_south(s, 2);
  PRINT_TEST_RESULT(space_get_south(s) == 2);
  space_destroy(s);
}
/**
 * test the second function for getting the south of the space
 */
void test2_space_get_south() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_south(s) == NO_ID);
}
/**
 * test the function for getting the east of the space
 */
void test1_space_get_east() {
  Space *s;
  s = space_create(5);
  space_set_east(s, 1);
  PRINT_TEST_RESULT(space_get_east(s) == 1);
  space_destroy(s);
}
/**
 * test the second for getting the east of the space
 */
void test2_space_get_east() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_east(s) == NO_ID);
}
/**
 * test the function for getting the west of the space
 */
void test1_space_get_west() {
  Space *s;
  s = space_create(5);
  space_set_west(s, 6);
  PRINT_TEST_RESULT(space_get_west(s) == 6);
  space_destroy(s);
}
/**
 * test the second function for getting the west of the space
 */
void test2_space_get_west() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_west(s) == NO_ID);
}
/**
 * test the function for getting the id of the space
 */
void test1_space_get_id() {
  Space *s;
  s = space_create(25);
  PRINT_TEST_RESULT(space_get_id(s) == 25);
  space_destroy(s);
}
/**
 * test the second function for getting the id of the space
 */
void test2_space_get_id() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_id(s) == NO_ID);
}
