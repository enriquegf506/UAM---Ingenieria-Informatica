/** 
 * @brief It declares the tests for the game module
 * 
 * @file game_test.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 12-03-2021
 * @copyright GNU Public License
 */
#ifndef GAME_TEST_H
#define GAME_TEST_H
#include "game.h"
#include "test.h"
#include "types.h"


/**
 * @test Test function for game_space adding
 * @pre space ID, game pointer to game
 * @post Output==OK
 */
void test1_game_add_space();
/**
 * @test Test function for game_space adding
 * @pre space ID, game pointer to game, Space==NULL
 * @post Output==ERROR
 */
void test2_game_add_space();
/**
 * @test Test function for game_object adding
 * @pre object ID, game pointer to game
 * @post Output==OK
 */
void test1_game_add_object();
/**
 * @test Test function for game_object adding
 * @pre object ID, game pointer to game, Object==NULL
 * @post Output==ERROR
 */
void test2_game_add_object();
/**
 * @test Test function for game_space_id_at getting
 * @pre space ID==3, game pointer to game, position(pos)==0
 * @post Output==3
 */
void test1_game_get_space_id_at();
/**
 * @test Test function for game_space_id_at getting
 * @pre game pointer to game, position(pos)==-1
 * @post Output==NO_ID
 */
void test2_game_get_space_id_at();
/**
 * @test Test function for game_space getting
 * @pre space ID==4, game pointer to game
 * @post space created with ID==4
 */
void test1_game_get_space();
/**
 * @test Test function for game_space getting
 * @pre space ID==4, game pointer to game, id of space get==1
 * @post Output==NULL
 */
void test2_game_get_space();
/**
 * @test Test function for game_player adding
 * @pre player ID, game pointer to game
 * @post Output==OK
 */
void test1_game_add_player();
/**
 * @test Test function for game_player adding
 * @pre player ID, game pointer to game, Player==NULL
 * @post Output==ERROR
 */
void test2_game_add_player();
/**
 * @test Test function for game_enemy adding
 * @pre enemy ID, game pointer to game
 * @post Output==OK
 */
void test1_game_add_enemy();
/**
 * @test Test function for game_enemy adding
 * @pre enemy ID, game pointer to game, Enemy==NULL
 * @post Output==ERROR
 */
void test2_game_add_enemy();
/**
 * @test Test function for game_link adding
 * @pre link ID, game pointer to game
 * @post Output==OK
 */
void test1_game_add_link();
/**
 * @test Test function for game_link adding
 * @pre link ID, game pointer to game, Link==NULL
 * @post Output==ERROR
 */
void test2_game_add_link();
/**
 * @test Test function for game_link getting
 * @pre link ID, game pointer to game, link_origin==4 , link_direction==N
 * @post return the link created
 */
void test1_game_get_link();
/**
 * @test Test function for game_link getting
 * @pre link ID, game==NULL, link_origin==4 , link_direction==N
 * @post Output==NULL
 */
void test2_game_get_link();
/**
 * @test Test function for game_link getting
 * @pre link ID, game pointer to game, link_dest==6 , link_direction==N
 * @post Output==6
 */
void test1_game_get_connection();
/**
 * @test Test function for game_link getting
 * @pre link ID, game==NULL, link_origin==4 , link_direction==N
 * @post Output==NO_ID
 */
void test2_game_get_connection();
/**
 * @test Test function for game_link getting
 * @pre link ID, game pointer to game, link_state==OPEN , link_direction==N
 * @post Output==OPEN
 */
void test1_game_get_connection_status();
/**
 * @test Test function for game_link getting
 * @pre link ID, game==NULL, link_origin==4 , link_direction==N
 * @post Output==-1
 */
void test2_game_get_connection_status();
/**
 * @test Test function for game_player getting
 * @pre player ID==4, game pointer to game
 * @post player created with ID==4
 */
void test1_game_get_player();
/**
 * @test Test function for game_player getting
 * @pre player ID==4, game pointer to game==NULL
 * @post Output==NULL
 */
void test2_game_get_player();
/**
 * @test Test function for game_enemy getting
 * @pre enemy ID==4, game pointer to game
 * @post enemy created with ID==4
 */
void test1_game_get_enemy();
/**
 * @test Test function for game_enemy getting
 * @pre enemy ID==4, game pointer to game==NULL
 * @post Output==NULL
 */
void test2_game_get_enemy();
/**
 * @test Test function for game_object getting
 * @pre object ID==4, game pointer to game, object Id want to get == 4
 * @post object created with ID==4
 */
void test1_game_get_obj();
/**
 * @test Test function for game_object getting
 * @pre object ID==4, game pointer to game, object ID want to get==1
 * @post Output==NULL
 */
void test2_game_get_obj();
/**
 * @test Test function for game_description setting
 * @pre game pointer to game, description == "hola"
 * @post Output==OK
 */
void test1_game_set_description();
/**
 * @test Test function for game_description setting
 * @pre game pointer to game==NULL, description == "hola"
 * @post Output==ERROR
 */
void test2_game_set_description();
/**
 * @test Test function for game_description Getting
 * @pre game pointer to game, description == "hola"
 * @post Output=="hola"
 */
void test1_game_get_description();
/**
 * @test Test function for game_description getting
 * @pre game pointer to game==NULL
 * @post Output==NULL
 */
void test2_game_get_description();
/**
 * @test Test function for game_state setting
 * @pre game pointer to game, STATUS state==OK
 * @post Output==OK
 */
void test1_game_set_state();
/**
 * @test Test function for game_state setting
 * @pre game pointer to game==NULL
 * @post Output==ERROR
 */
void test2_game_set_state();
/**
 * @test Test function for game_state getting
 * @pre game pointer to game, STATUS state==OK
 * @post Output==OK
 */
void test1_game_get_state();
/**
 * @test Test function for game_state getting
 * @pre game pointer to game==NULL
 * @post Output==ERROR
 */
void test2_game_get_state();


/**
 * @test Test function for game is over
 * @pre game pointer to game, Player pointer to player, health=3
 * @post Output==FALSE
 */
void test1_game_is_over();
/**
 * @test Test function for game is over
 * @pre game pointer to game, Player pointer to player, health=0
 * @post Output==TRUE
 */
void test2_game_is_over();
#endif
