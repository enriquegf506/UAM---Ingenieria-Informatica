/** 
 * @brief It declares the tests for the inventory module
 * 
 * @file inventory_test.h
 * @author Profesores Pprog
 * @version 2.0 
 * @date 12-03-2021
 * @copyright GNU Public License
 */
#ifndef INVENTORY_TEST_H
#define INVENTORY_TEST_H
#include "inventory.h"
#include "test.h"
#include "types.h"
#include "set.h"


/**
 * @test Test function for inventory adding
 * @pre inventory ID
 * @post Ouput==OK 
 */
void test1_inventory_add();
/**
 * @brief Test function for inventory adding
 * @pre inventory == NULL
 *@post Ouput==ERROR
 */
void test2_inventory_add();
/**
 * @brief Test function for set adding
 * @pre inventory Id == NO_ID
 *@post Ouput==ERROR
 */
void test3_inventory_add();
/**
 * @brief Test function for inventory deleting
 * @pre inventory Id
 *@post Ouput==OK 
 */
void test1_inventory_delete();
/**
 * @brief Test function for set deleting
 * @pre inventory==NULL
 *@post Ouput==ERROR
 */
void test2_inventory_delete();

/**
 * @brief Test function for inventory_number_ids getting
 * @pre inventory Id
 *@post Output==0
 */
void test1_inventory_get_nids();
/**
 * @brief Test function for inventory_number_ids getting
 * @pre inventory Set==NULL
 *@post Output==-1
 */
void test2_inventory_get_nids();
/**
 * @brief Test function for inventory_set_ids getting
 * @pre inventory Set Id
 *@post Output==Id
 */
void test1_inventory_get_max();
/**
 * @brief Test function for inventory_set_ids getting
 * @pre inventory Set == NULL
 *@post Output==NO_ID
 */
void test2_inventory_get_max();

/**
 * @brief Test function for inventory_destroying 
 * @pre inventory Id
 *@post Output==OK
 */
void test1_inventory_destroy();
/**
 * @brief Test function for inventory_destroying 
 * @pre inventory Id
 *@post Output==ERROR
 */
void test2_inventory_destroy();
/**
 * @brief Test function for inventory_set_ids setting
 * @pre inventory Set ID
 *@post Output==OK
 */
void test1_inventory_set_max();
/**
 * @brief Test function for inventory_set_ids setting
 * @pre inventory Set == NULL
 *@post Output==ERROR
 */
void test2_inventory_set_max();
/**
 * @brief Test function for inventory_id finding
 * @pre inventory== NULL
 *@post Output==FALSE
 */
void test1_inventory_findId();
/**
 * @brief Test function for inventory_id finding
 * @pre inventory Set ID==4, inventory Id
 *@post Output==TRUE
 */
void test2_inventory_findId();

#endif
