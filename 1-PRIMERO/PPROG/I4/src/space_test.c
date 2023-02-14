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


#define MAX_TESTS 35  /*!< Number of tests done*/

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
  if (all || test == 6) test1_space_add_object();
  if (all || test == 7) test2_space_add_object();
  if (all || test == 8) test1_space_get_name();
  if (all || test == 9) test2_space_get_name();
  if (all || test == 10) test1_space_get_id();
  if (all || test == 11) test2_space_get_id();
  if (all || test == 12) test1_space_set_gdesc();
  if (all || test == 13) test2_space_set_gdesc();
  if (all || test == 14) test1_space_get_gdesc();
  if (all || test == 15) test2_space_get_gdesc();
  if (all || test == 16) test1_space_contain_object();
  if (all || test == 17) test2_space_contain_object();
  if (all || test == 18) test1_space_del_object();
  if (all || test == 19) test2_space_del_object();  
  if (all || test == 20) test1_space_get_objects();
  if (all || test == 21) test2_space_get_objects(); 
  if (all || test == 22) test1_space_set_full_description();
  if (all || test == 23) test2_space_set_full_description();
  if (all || test == 24) test1_space_get_full_description();
  if (all || test == 25) test2_space_get_full_description(); 
  if (all || test == 26) test1_space_set_description();
  if (all || test == 27) test2_space_set_description();
  if (all || test == 28) test1_space_get_description();
  if (all || test == 29) test2_space_get_description();   
  if (all || test == 30) test1_space_set_illumination();
  if (all || test == 31) test2_space_set_illumination();
  if (all || test == 32) test1_space_get_illumination();
  if (all || test == 33) test2_space_get_illumination();
  if (all || test == 34) test1_space_destroy();
  if (all || test == 35) test2_space_destroy();
  PRINT_PASSED_PERCENTAGE;

  return 1;
}

void test1_space_create() {
  int result;
  Space *s;
  s = space_create(5);
  result=s!=NULL ;
  PRINT_TEST_RESULT(result);
  space_destroy(s);
}

void test2_space_create() {
  Space *s;
  s = space_create(4);
  PRINT_TEST_RESULT(space_get_id(s) == 4);
  space_destroy(s);
}

void test1_space_set_name() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_name(s, "hola") == OK);
  space_destroy(s);
}

void test2_space_set_name() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_name(s, "hola") == ERROR);
}

void test3_space_set_name() {
  Space *s;
  s = space_create(5);
  PRINT_TEST_RESULT(space_set_name(s, NULL) == ERROR);
  space_destroy(s);
}



void test1_space_add_object() {
  Space *s;
  s = space_create(1);
  PRINT_TEST_RESULT(space_add_object(s,2) == OK);
  space_destroy(s);
}

void test2_space_add_object() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_add_object(s,1) == ERROR);
}

void test1_space_get_name() {
  Space *s;
  s = space_create(1);
  space_set_name(s, "adios");
  PRINT_TEST_RESULT(strcmp(space_get_name(s), "adios") == 0);
  space_destroy(s);
}

void test2_space_get_name() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_name(s) == NULL);
}


void test1_space_get_id() {
  Space *s;
  s = space_create(25);
  PRINT_TEST_RESULT(space_get_id(s) == 25);
  space_destroy(s);
}

void test2_space_get_id() {
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_id(s) == NO_ID);
}

void test1_space_set_gdesc(){
  Space *s;
  s= space_create(22);
  PRINT_TEST_RESULT(space_set_gdesc(s, "hola", 1)==OK);
  space_destroy(s);
}

void test2_space_set_gdesc(){
  Space *s = NULL;
  PRINT_TEST_RESULT(space_set_gdesc(s, "hola", 1)==ERROR);
}

void test1_space_get_gdesc(){
  Space *s;
  s= space_create(22);
  space_set_gdesc(s, "hola", 1);
  PRINT_TEST_RESULT(strcmp(space_get_gdesc(s, 1), "hola") == 0);
  space_destroy(s);
}

void test2_space_get_gdesc(){
  Space *s = NULL;
  PRINT_TEST_RESULT(space_get_gdesc(s, 1)==NULL);
}

void test1_space_get_objects(){
  Space *s;
  Set *set;
  s= space_create(5);
  set=set_create(3);
  PRINT_TEST_RESULT(space_get_objects(s)!=set);
  space_destroy(s);
  set_destroy(set);
}

void test2_space_get_objects(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_get_objects(s)== NULL);
}

void test1_space_contain_object(){
  Space *s;
  s = space_create(22);
  space_add_object(s,6);
  PRINT_TEST_RESULT(space_contain_object(s,6)== TRUE);
  space_destroy(s);
}

void test2_space_contain_object(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_contain_object(s,6)== FALSE);
}

void test1_space_del_object(){
  Space *s;
  s = space_create(22);
  PRINT_TEST_RESULT(space_del_object(s, 6)==OK);
  space_destroy(s);
}

void test2_space_del_object(){
  Space *s = NULL;
  PRINT_TEST_RESULT(space_del_object(s, 6) == ERROR);
}

/**
   test1 for space_set_full_description
*/
void test1_space_set_full_description(){
  Space *s;
  s=space_create(3);
  PRINT_TEST_RESULT(space_set_full_description(s, "hola") == OK);
}

/**
   test2 for space_set_full_description
*/
void test2_space_set_full_description(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_set_full_description(s, "hola") == ERROR);
}

/**
   test1 for space_get_full_description
*/
void test1_space_get_full_description(){
  Space *s;
  s=space_create(4);
  space_set_full_description(s, "hola");
  PRINT_TEST_RESULT(strcmp(space_get_full_description(s), "hola") == 0);
}

/**
   test2 for space_get_full_description
*/
void test2_space_get_full_description(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_get_full_description(s) == NULL);
}

/**
   test1 for space_set_description
*/
void test1_space_set_description(){
  Space *s;
  s=space_create(3);
  PRINT_TEST_RESULT(space_set_desc(s, "hola") == OK);
}

/**
   test2 for space_set_description
*/
void test2_space_set_description(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_set_desc(s, "hola") == ERROR);
}

/**
   test1 for space_get_description
*/
void test1_space_get_description(){
  Space *s;
  s=space_create(4);
  space_set_desc(s, "hola");
  PRINT_TEST_RESULT(strcmp(space_get_desc(s), "hola") == 0);
}

/**
   test2 for space_get_description
*/
void test2_space_get_description(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_get_desc(s) == NULL);
}

void test1_space_set_illumination(){
  Space *s;
  s = space_create(22);
  PRINT_TEST_RESULT(space_set_illumination(s,TRUE)== OK);
  space_destroy(s);
}

void test2_space_set_illumination(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_set_illumination(s,FALSE)== ERROR);
}

void test1_space_get_illumination(){
  Space *s;
  s = space_create(22);
  space_set_illumination(s, TRUE);
  PRINT_TEST_RESULT(space_get_illumination(s)== TRUE);
  space_destroy(s);
}

void test2_space_get_illumination(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_get_illumination(s)== FALSE);
}

void test1_space_destroy(){
  Space *s;
  s = space_create(22);
  PRINT_TEST_RESULT(space_destroy(s)== OK);
}

void test2_space_destroy(){
  Space *s=NULL;
  PRINT_TEST_RESULT(space_destroy(s)== ERROR);
}