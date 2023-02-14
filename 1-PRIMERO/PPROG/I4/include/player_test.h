/** 
 * @brief It declares the tests for the player module
 * 
 * @file player_test.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 31-03-2021
 * @copyright GNU Public License
 */
#ifndef PLAYER_TEST_H
#define PLAYER_TEST_H
#include "player.h"
#include "test.h"
#include "types.h"
#include "inventory.h"

/**
 * @test Test player creation
 * @pre player ID 
 * @post Non NULL pointer to player 
 */
void test1_player_create();
/**
 * @test Test player creation
 * @pre player ID 
 * @post player_ID == Supplied player Id
 */
void test2_player_create();
/**
 * @test Test function for player_name setting
 * @pre String with player name
 * @post Ouput==OK 
 */
void test1_player_set_name();
/**
 * @test Test function for player_name setting
 * @pre pointer to player = NULL 
 * @post Output==ERROR
 */
void test2_player_set_name();
/**
 * @brief Test function for player_location setting
 * @pre player location
 *@post Output==OK
 */
void test1_player_set_location();
/**
 * @brief Test function for player_location setting
 * @pre player location ==NULL
 *@post Output==OK
 */
void test2_player_set_location();
/**
 * @brief Test function for player_location getting
 * @pre player 
 *@post Output==player location
 */
void test1_player_get_location();
/**
 * @brief Test function for player_location getting
 * @pre player =NULL
 *@post Output==NO_ID
 */
void test2_player_get_location();
/**
 * @brief Test function for player_id setting
 * @pre player ID
 *@post Output==OK
 */

void test1_player_get_id();
/**
 * @brief Test function for player_id getting
 * @pre player =NULL
 *@post Output==NO_ID
 */
void test2_player_get_id();
/**
 * @brief Test function for player_name getting
 * @pre player, name= 'adios'
 *@post Output==0
 */
void test1_player_get_name();
/**
 * @brief Test function for player_name getting
 * @pre player=NULL
 *@post Output==NULL
 */
void test2_player_get_name();
/**
 * @brief Test function for player_health getting
 * @pre player health==6
 *@post Output==6
 */
void test1_player_get_health();
/**
 * @brief Test function for player_health getting
 * @pre player=NULL
 *@post Output==-1
 */
void test2_player_get_health();
/**
 * @brief Test function for player hetting
 * @pre player health ==4, player ID
 *@post Output==OK
 */
void test1_player_set_health();
/**
 * @brief Test function for player_health setting
 * @pre player=NULL
 *@post Output==ERROR
 */
void test2_player_set_health();
/**
 * @brief Test function for player_object(Id) getting
 * @pre player object(Id)==6
 *@post Output==6
 */
void test1_player_get_object();
/**
 * @brief Test function for player_object(ID) getting
 * @pre player=NULL
 *@post Output==NO_ID
 */
void test2_player_get_object();
/**
 * @brief Test function for player_object(Id) setting
 * @pre player_object(Id) ==4, player ID
 *@post Output==OK
 */

void test1_player_destroy();
/**
 * @brief Test function for player destroying
 * @pre player==NULL
 *@post Output==ERROR
 */
void test2_player_destroy();
/**
 * @brief Test function for player painting
 * @pre player(Id) ==4
 *@post Output==OK
 */
void test1_player_print();
/**
 * @brief Test function for player painting
 * @pre player==NULL
 *@post Output==ERROR
 */
void test2_player_print();
/**
 * @brief Test function for player_object finding
 * @pre player id, 
 * Add id to player==6
 *@post Output==TRUE
 */
void test1_player_contain_object();
/**
 * @brief Test function for player_object finding
 * @pre player==NULL
 *@post Output==FALSE
 */
void test2_player_contain_object();
/**
 * @brief Test function for player_object deleting
 * @pre player_object id ==4, player ID
 *@post Output==OK
 */
void test1_player_del_object();
/**
 * @brief Test function for player_object deleting
 * @pre player ID==NULL
 *@post Output==ERROR
 */
void test2_player_del_object();
/**
 * @brief Test function for player hetting
 * @pre player health ==4, player ID
 *@post Output==OK
 */
void test1_player_set_max();
/**
 * @brief Test function for player_health setting
 * @pre player=NULL
 *@post Output==ERROR
 */
void test2_player_set_max();
/**
 * @brief Test function for player_health getting
 * @pre player health==6
 *@post Output==6
 */
void test1_player_get_max();
/**
 * @brief Test function for player_health getting
 * @pre player=NULL
 *@post Output==-1
 */
void test2_player_get_max();
/**
 * @brief Test function for player_object adding
 * @pre player ID; 
 *@post Output==OK
 */
void test1_player_add_object();
/**
 * @brief Test function for player_object adding
 * @pre player=NULL
 *@post Output==ERROR;
 */
void test2_player_add_object();

#endif
