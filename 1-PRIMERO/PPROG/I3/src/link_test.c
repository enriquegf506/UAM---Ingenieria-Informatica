/** 
 * @brief It tests link modlule
 * 
 * @file link_test.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 31-03-2021
 * @copyright GNU Public License
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "link_test.h"


#define MAX_TESTS 28  /*!< Macro for the maximum test*/

/** 
 * @brief Main function for LINK unit tests. 
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
    printf("Running all test for module Link:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 && test > MAX_TESTS) {
      printf("Error: unknown test %d\t", test);
      exit(EXIT_SUCCESS);
    }
  }


  if (all || test == 1) test1_link_create();
  if (all || test == 2) test2_link_create();
  if (all || test == 3) test1_link_set_name();
  if (all || test == 4) test2_link_set_name();
  if (all || test == 5) test1_link_get_name();
  if (all || test == 6) test2_link_get_name();
  if (all || test == 7) test1_link_get_origin();
  if (all || test == 8) test2_link_get_origin();
  if (all || test == 9) test1_link_set_origin();
  if (all || test == 10) test2_link_set_origin();
  if (all || test == 11) test1_link_set_dest();
  if (all || test == 12) test2_link_set_dest();
  if (all || test == 13) test1_link_get_dest();
  if (all || test == 14) test2_link_get_dest();
  if (all || test == 15) test1_link_set_state();
  if (all || test == 16) test2_link_set_state();
  if (all || test == 17) test1_link_get_state();
  if (all || test == 18) test2_link_get_state();
  if (all || test == 19) test1_link_set_direction();
  if (all || test == 20) test2_link_set_direction();
  if (all || test == 21) test1_link_get_direction();
  if (all || test == 22) test1_link_get_direction();
  if (all || test == 23) test1_link_get_id();
  if (all || test == 24) test2_link_get_id();
  if (all || test == 25) test1_link_destroy();
  if (all || test == 26) test2_link_destroy();
  if (all || test == 27) test1_link_print();
  if (all || test == 28) test2_link_print();
  PRINT_PASSED_PERCENTAGE;

  return 1;
}

void test1_link_create() {
  int result;
  Link *l;
  l = link_create(5);
  result=l!=NULL ;
  PRINT_TEST_RESULT(result);
  link_destroy(l);
}
void test2_link_create() {
  Link *l;
  l = link_create(4);
  PRINT_TEST_RESULT(link_get_id(l) == 4);
  link_destroy(l);
}

void test1_link_set_name() {
  Link *l;
  l = link_create(5);
  PRINT_TEST_RESULT(link_set_name(l, "hola") == OK);
  link_destroy(l);
}

void test2_link_set_name() {
  Link *l = NULL;
  PRINT_TEST_RESULT(link_set_name(l, "hola") == ERROR);
}

void test1_link_get_name() {
  Link *l;
  l = link_create(5);
  link_set_name(l, "adios");
  PRINT_TEST_RESULT(strcmp(link_get_name(l), "adios") == 0);
  link_destroy(l);
}

void test2_link_get_name() {
  Link *l = NULL;
  PRINT_TEST_RESULT(link_get_name(l) == NULL);
}

void test1_link_set_origin() {
  Link *l;
  l = link_create(5);
  PRINT_TEST_RESULT(link_set_origin(l, 4) == OK);
  link_destroy(l);
}

void test2_link_set_origin(){
  Link *l = NULL;
  PRINT_TEST_RESULT(link_set_origin(l, 4) == ERROR);
}

void test1_link_get_origin(){
  Link *l;
  l = link_create(5);
  link_set_origin(l, 6);
  PRINT_TEST_RESULT(link_get_origin(l) == 6);
  link_destroy(l);
}

void test2_link_get_origin(){
  Link *l = NULL;
  PRINT_TEST_RESULT(link_get_origin(l) == NO_ID);
}

void test1_link_set_dest(){
  Link *l;
  l = link_create(3);
  PRINT_TEST_RESULT(link_set_dest(l, 4) == OK);
  link_destroy(l);
}

void test2_link_set_dest(){
  Link *l = NULL;
  PRINT_TEST_RESULT(link_set_dest(l, 4) == ERROR);
}

void test1_link_get_dest(){
  Link *l;
  l = link_create(3);
  link_set_dest(l, 4);
  PRINT_TEST_RESULT(link_get_dest(l) == 4);
  link_destroy(l);
}

void test2_link_get_dest(){
  Link *l = NULL;
  PRINT_TEST_RESULT(link_get_dest(l) == NO_ID);
}

void test1_link_set_state(){
  Link *l=NULL;
  LINKSTATUS state=OPEN;
  PRINT_TEST_RESULT(link_set_state(l, state)==ERROR);
}

void test2_link_set_state(){
  Link *l;
  l = link_create(5);
  PRINT_TEST_RESULT(link_set_state(l, CLOSE)==OK);
  link_destroy(l);
}

void test1_link_get_state(){
  Link *l=NULL;
  PRINT_TEST_RESULT(link_get_state(l)==-1);
}

void test2_link_get_state(){
  Link *l;
  l = link_create(5);
  link_set_state(l, CLOSE);
  PRINT_TEST_RESULT(link_get_state(l)==CLOSE);
  link_destroy(l);
}

void test1_link_set_direction(){
  Link *l;
  l = link_create(5);
  PRINT_TEST_RESULT(link_set_direction(l, N)==OK);
  link_destroy(l);
}

void test2_link_set_direction(){
  Link *l=NULL;
  DIRECTION dir=N;
  PRINT_TEST_RESULT(link_set_direction(l, dir)==ERROR);
}

void test1_link_get_direction(){
  Link *l = NULL;
  PRINT_TEST_RESULT(link_get_direction(l)==-1);
}

void test2_link_get_direction(){
  Link *l;
  l = link_create(2);
  link_set_direction(l, N);
  PRINT_TEST_RESULT(link_get_direction(l)==N);
  link_destroy(l);
}

void test1_link_get_id() {
  Link *l;
  l = link_create(5);
  link_set_dest(l, 6);
  PRINT_TEST_RESULT(link_get_id(l) == 5);
  link_destroy(l);
}

void test2_link_get_id() {
  Link *l = NULL;
  PRINT_TEST_RESULT(link_get_id(l) == NO_ID);
}

void test1_link_destroy(){
  Link *l;
  l = link_create(3);
  PRINT_TEST_RESULT(link_destroy(l) == OK);
}

void test2_link_destroy(){
  Link *l = NULL;
  PRINT_TEST_RESULT(link_destroy(l) == ERROR);
}

void test1_link_print(){
  Link *l;
  l = link_create(3);
  PRINT_TEST_RESULT(link_print(l) == OK);
  link_destroy(l);
}

void test2_link_print(){
  Link *l = NULL;
  PRINT_TEST_RESULT(link_print(l) == ERROR);
}