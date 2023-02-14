/** 
 * @brief It declares the tests for the game_reader module
 * 
 * @file game_reader_test.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 31-03-2021
 * @copyright GNU Public License
 */
#ifndef GAME_READER_TEST_H
#define GAME_READER_TEST_H
#include "game_reader.h"
#include "test.h"
#include "types.h"
#include "game.h"

/**
 * @test Test function for game_reader_objects loading
 * @pre game pointer to game, document.dat
 * @post Output==OK
 */
void test1_game_reader_load_spaces();
/**
 * @test Test function for game_reader_objects loading
 * @pre char name== NULL, game pointer to game
 * @post Output==ERROR
 */
void test2_game_reader_load_spaces();
/**
 * @test Test function for game_reader_objects loading
 * @pre game pointer to game, document.dat
 * @post Output==OK
 */
void test1_game_reader_load_objects();
/**
 * @test Test function for game_reader_objects loading
 * @pre char name== NULL, game pointer to game
 * @post Output==ERROR
 */
void test2_game_reader_load_objects();
/**
 * @test Test function for game_reader_players loading
 * @pre game pointer to game, document.dat
 * @post Output==OK
 */
void test1_game_reader_load_players();
/**
 * @test Test function for game_reader_playersloading
 * @pre char name== NULL, game pointer to game
 * @post Output==ERROR
 */
void test2_game_reader_load_players();
/**
 * @test Test function for game_reader_links loading
 * @pre game pointer to game, document.dat
 * @post Output==OK
 */
void test1_game_reader_load_links();
/**
 * @test Test function for game_reader_links loading
 * @pre char name== NULL, game pointer to game
 * @post Output==ERROR
 */
void test2_game_reader_load_links();
/**
 * @test Test function for game_reader_enemies loading
 * @pre game pointer to game, document.dat
 * @post Output==OK
 */
void test1_game_reader_load_enemies();
/**
 * @test Test function for game_reader_enemies loading
 * @pre char name== NULL, game pointer to game
 * @post Output==ERROR
 */
void test2_game_reader_load_enemies();

#endif
