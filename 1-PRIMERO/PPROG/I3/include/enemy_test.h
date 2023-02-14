/** 
 * @brief It declares the tests for the enemy module
 * 
 * @file enemy_test.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 31-03-2021
 * @copyright GNU Public License
 */
#ifndef ENEMY_TEST_H
#define ENEMY_TEST_H
#include "enemy.h"
#include "test.h"
#include "types.h"

/**
 * @test Test enemy creation
 * @pre enemy ID 
 * @post Non NULL pointer to enemy 
 */
void test1_enemy_create();
/**
 * @test Test enemy creation
 * @pre enemy ID 
 * @post enemy_ID == Supplied enemy Id
 */
void test2_enemy_create();
/**
 * @test Test function for enemy_name setting
 * @pre String with enemy name
 * @post Ouput==OK 
 */
void test1_enemy_set_name();
/**
 * @test Test function for enemy_name setting
 * @pre pointer to enemy = NULL 
 * @post Output==ERROR
 */
void test2_enemy_set_name();
/**
 * @brief Test function for Enemy_location setting
 * @pre Enemy location
 *@post Output==OK
 */
void test1_enemy_set_location();
/**
 * @brief Test function for Enemy_location setting
 * @pre Enemy location ==NULL
 *@post Output==OK
 */
void test2_enemy_set_location();
/**
 * @brief Test function for enemy_location getting
 * @pre enemy 
 *@post Output==enemy location
 */
void test1_enemy_get_location();
/**
 * @brief Test function for enemy_location getting
 * @pre enemy =NULL
 *@post Output==NO_ID
 */
void test2_enemy_get_location();
/**
 * @brief Test function for Enemy_id setting
 * @pre Enemy ID
 *@post Output==OK
 */
void test1_enemy_set_id();
/**
 * @brief Test function for Enemy_id setting
 * @pre Enemy ID== NULL
 *@post Output==OK
 */
void test2_enemy_set_id();
/**
 * @brief Test function for enemy_id getting
 * @pre enemy 
 *@post Output==enemy ID
 */
void test1_enemy_get_id();
/**
 * @brief Test function for enemy_id getting
 * @pre enemy =NULL
 *@post Output==NO_ID
 */
void test2_enemy_get_id();
/**
 * @brief Test function for enemy_name getting
 * @pre enemy, name= 'adios'
 *@post Output==0
 */
void test1_enemy_get_name();
/**
 * @brief Test function for enemy_name getting
 * @pre enemy=NULL
 *@post Output==NULL
 */
void test2_enemy_get_name();
/**
 * @brief Test function for enemy_health getting
 * @pre enemy health==6
 *@post Output==6
 */
void test1_enemy_get_health();
/**
 * @brief Test function for enemy_health getting
 * @pre enemy=NULL
 *@post Output==-1
 */
void test2_enemy_get_health();
/**
 * @brief Test function for enemy hetting
 * @pre enemy health ==4, enemy ID
 *@post Output==OK
 */
void test1_enemy_set_health();
/**
 * @brief Test function for enemy_health setting
 * @pre enemy=NULL
 *@post Output==ERROR
 */
void test2_enemy_set_health();
/**
 * @brief Test function for enemy painting
 * @pre enemy pointer to enemy
 *@post Output==OK
 */
void test1_enemy_print();
/**
 * @brief Test function for enemy_health getting
 * @pre enemy=NULL
 *@post Output==ERROR
 */
void test2_enemy_print();
/**
 * @brief Test function for enemy hetting
 * @pre enemy pointer to enemy
 *@post Output==OK
 */
void test1_enemy_destroy();
/**
 * @brief Test function for enemy_health setting
 * @pre enemy=NULL
 *@post Output==ERROR
 */
void test2_enemy_destroy();

#endif
