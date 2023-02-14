/** 
 * @brief It declares the tests for the set module
 * 
 * @file set_test.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 12-03-2021
 * @copyright GNU Public License
 */
#ifndef SET_TEST_H
#define SET_TEST_H
#include "set.h"
#include "test.h"
#include "types.h"


/**
 * @test Test set creation
 * @pre Set ID 
 * @post Non NULL pointer to set 
 */
void test1_set_create();
/**
 * @test Test set creation
 * @pre Set ID ==4
 * @post Output==4
 */
void test2_set_create();
/**
 * @test Test function for set adding
 * @pre Set ID
 * @post Ouput==OK 
 */
void test1_set_add();
/**
 * @brief Test function for set adding
 * @pre Set == NULL
 *@post Ouput==ERROR
 */
void test2_set_add();
/**
 * @brief Test function for set adding
 * @pre Set Id == NO_ID
 *@post Ouput==ERROR
 */
void test3_set_add();
/**
 * @brief Test function for set deleting
 * @pre Set Id
 *@post Ouput==OK 
 */
void test1_set_del();
/**
 * @brief Test function for set deleting
 * @pre Set==NULL
 *@post Ouput==ERROR
 */
void test2_set_del();
/**
 * @brief Test function for set deleting
 * @pre no previous adding, set id==NO_ID
 *@post Ouput==ERROR
 */
void test3_set_del();
/**
 * @brief Test function for set_number_ids getting
 * @pre Set Id
 *@post Output==0
 */
void test1_set_getNids();
/**
 * @brief Test function for set_number_ids getting
 * @pre Set==NULL
 *@post Output==-1
 */
void test2_set_getNids();
/**
 * @brief Test function for set_ids getting
 * @pre Set Id
 *@post Output==Id
 */
void test1_set_getId();
/**
 * @brief Test function for set_ids getting
 * @pre Set == NULL
 *@post Output==NO_ID
 */
void test2_set_getId();
/**
 * @brief Test function for set_ids getting
 * @pre Set Id
 *@post Output==NO_ID
 */
void test3_set_getId();
/**
 * @brief Test function for set_destroying 
 * @pre Set Id
 *@post Output==OK
 */
void test1_set_destroy();
/**
 * @brief Test function for set_destroying 
 * @pre Set Id
 *@post Output==ERROR
 */
void test2_set_destroy();
/**
 * @brief Test function for set_Id finding
 * @pre Set Id != TEST
 *@post Output==FALSE
 */
void test1_set_findId();
/**
 * @brief Test function for set_Id finding
 * @pre Set == NULL
 *@post Output==FALSE
 */
void test2_set_findId();
/**
 * @brief Test function for set_printing
 * @pre Set Id
 *@post Output==OK
 */
void test1_set_print();
/**
 * @brief Test function for set_printing
 * @pre Set Id
 *@post Output==ERROR
 */
void test2_set_print();

#endif
