/** 
 * @brief It declares the tests for the space module
 * 
 * @file space_test.h
 * @author Profesores Pprog
 * @version 2.0 
 * @date 09-03-2021
 * @copyright GNU Public License
 */

#ifndef SPACE_TEST_H
#define SPACE_TEST_H

/**
 * @test Test space creation
 * @pre Space ID 
 * @post Non NULL pointer to space 
 */
void test1_space_create();

/**
 * @test Test space creation
 * @pre Space ID 
 * @post Space_ID == Supplied Space Id
 */
void test2_space_create();

/**
 * @test Test function for space_name setting
 * @pre String with space name
 * @post Ouput==OK 
 */
void test1_space_set_name();

/**
 * @test Test function for space_name setting
 * @pre pointer to space = NULL 
 * @post Output==ERROR
 */
void test2_space_set_name();

/**
 * @test Test function for space_name setting
 * @pre pointer to space_name = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test3_space_set_name();
/**
 * @test Test function for space_noth setting
 * @pre pointer to space_set_north = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_set_north();

void test3_space_set_name();
/**
 * @test Test function for space_noth setting
 * @pre pointer to space_set_north = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_set_north();
void test3_space_set_name();
/**
 * @test Test function for space_noth setting
 * @pre pointer to space_set_north = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test3_space_set_north();
void test3_space_set_name();
/**
 * @test Test function for space_noth setting
 * @pre pointer to space_set_north = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test4_space_set_north();
void test3_space_set_name();
/**
 * @test Test function for space_south setting
 * @pre pointer to space_set_south = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_set_south();
/**
 * @test Test function for space_south setting
 * @pre pointer to space_set_south = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_set_south();
/**
 * @test Test function for space_south setting
 * @pre pointer to space_set_south = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test3_space_set_south();
/**
 * @test Test function for space_south setting
 * @pre pointer to space_set_south = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test4_space_set_south();
/**
 * @test Test function for space_south setting
 * @pre pointer to space_set_south = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_set_east();
/**
 * @test Test function for space_east setting
 * @pre pointer to space_set_east = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_set_east();
/**
 * @test Test function for space_east setting
 * @pre pointer to space_set_east = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test3_space_set_east();
/**
 * @test Test function for space_east setting
 * @pre pointer to space_set_east = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test4_space_set_east();
/**
 * @test Test function for space_west setting
 * @pre pointer to space_set_west = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_set_west();
/**
 * @test Test function for space_west setting
 * @pre pointer to space_set_west = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_set_west();
/**
 * @test Test function for space_west setting
 * @pre pointer to space_set_west = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test3_space_set_west();
/**
 * @test Test function for space_west setting
 * @pre pointer to space_set_west = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test4_space_set_west();
/**
 * @test Test function for space_id getting
 * @pre pointer to space_get_id = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_get_id();
/**
 * @test Test function for space_id getting
 * @pre pointer to space_get_id = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_get_id();
/**
 * @test Test function for space_object setting
 * @pre pointer to space_set_object = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_set_object();
/**
 * @test Test function for space_object setting
 * @pre pointer to space_set_object = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_set_object();
/**
 * @test Test function for space_name getting
 * @pre pointer to space_get_name = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_get_name();
/**
 * @test Test function for space_name getting
 * @pre pointer to space_get_name = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_get_name();
/**
 * @test Test function for space_north getting
 * @pre pointer to space_get_north = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_get_north();
/**
 * @test Test function for space_north getting
 * @pre pointer to space_get_north = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_get_north();
/**
 * @test Test function for space_south getting
 * @pre pointer to space_get_south = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_get_south();
/**
 * @test Test function for space_south getting
 * @pre pointer to space_get_south = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_get_south();
/**
 * @test Test function for space_east getting
 * @pre pointer to space_get_east = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_get_east();
/**
 * @test Test function for space_east getting
 * @pre pointer to space_get_east = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_get_east();
/**
 * @test Test function for space_west getting
 * @pre pointer to space_get_west = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_get_west();
/**
 * @test Test function for space_west getting
 * @pre pointer to space_get_west = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_get_west();
/**
 * @test Test function for space_object getting
 * @pre pointer to space_get_object = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test1_space_get_object();
/**
 * @test Test function for space_object getting
 * @pre pointer to space_get_object = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test2_space_get_object();
/**
 * @test Test function for space_object getting
 * @pre pointer to space_get_object = NULL (point to space = NON NULL) 
 * @post Output==ERROR
 */
void test3_space_get_object();


#endif
