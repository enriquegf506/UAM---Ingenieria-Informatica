/** 
 * @brief It declares the tests for the object module
 * 
 * @file object_test.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 04-04-2021
 * @copyright GNU Public License
 */
#ifndef PLAYER_TEST_H
#define PLAYER_TEST_H
#include "object.h"
#include "test.h"
#include "types.h"

/**
 * @test Test object creation
 * @pre object ID
 * @post Non NULL pointer to object
 */
void test1_object_create();
/**
 * @test Test object creation
 * @pre object ID 
 * @post object_ID == Supplied object Id
 */
void test2_object_create();
/**
 * @test Test function for object_name setting
 * @pre String with object name
 * @post Ouput==OK 
 */
void test1_object_set_name(); 
 /**
 * @test Test function for object_name setting
 * @pre pointer to object = NULL 
 * @post Output==ERROR
 */
void test2_object_set_name();
/**
 * @brief Test function for object_name getting
 * @pre object, name= 'adios'
 *@post Output==OK
 */
void test1_object_get_name();
/**
 * @brief Test function for object_name getting
 * @pre object, name= 'adios'
 *@post Output==0
 */
void test2_object_get_name();
/**
 * @brief Test function for object_id getting
 * @pre object =NULL
 *@post Output==NO_ID
 */
void test1_object_get_id();
/**
 * @brief Test function for object_id getting
 * @pre object, name= 'adios'
 *@post Output==
 */
void test2_object_get_id();
/**
 * @brief Test function for object_location setting
 * @pre object id ==NULL
 *@post Output==OK
 */
void test1_object_set_id();
/**
 * @brief Test function for object_location getting
 * @pre object 
 *@post Output==object id
 */
void test2_object_set_id();
/**
 * @brief Test function for object destroying
 * @pre object(Id) ==4
 *@post Output==OK
 */
void test1_object_destroy();
/**
 * @brief Test function for object destroying
 * @pre object==NULL
 *@post Output==ERROR
 */
void test2_object_destroy();
/**
 * @brief Test function for object painting
 * @pre object(Id) ==4
 *@post Output==OK
 */
void test1_object_print();
/**
 * @brief Test function for object painting
 * @pre object==NULL
 *@post Output==ERROR
 */
void test2_object_print();
/**
 * @test Test function for object_description setting
 * @pre object pointer to object, description == "hola"
 * @post Output==OK
 */
void test1_object_set_description();
/**
 * @test Test function for object_description setting
 * @pre object pointer to object==NULL, description == "hola"
 * @post Output==ERROR
 */
void test2_object_set_description();
/**
 * @test Test function for object_description Getting
 * @pre object pointer to object, description == "hola"
 * @post Output=="hola"
 */
void test1_object_get_description();
/**
 * @test Test function for object_description setting
 * @pre  object pointer to object==NULL
 * @post Output==NULL
 */
void test2_object_get_description();
/**
 * @test Test function for object_movable setting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==OK
 */
void test1_object_set_movable();
/**
 * @test Test function for object_movable setting
 * @pre object pointer to object, Bool movable==5
 * @post Output==ERROR
 */
void test2_object_set_movable();
/**
 * @test Test function for object_movable Getting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==TRUE
 */
void test1_object_get_movable();
/**
 * @test Test function for object_movable Getting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==-1
 */
void test2_object_get_movable();
/**
 * @test Test function for object_dependency setting
 * @pre object pointer to object, Id id_object_dependency=15
 * @post Output==OK
 */
void test1_object_set_dependency();
/**
 * @test Test function for object_dependency setting
 * @pre object pointer to object==NULL
 * @post Output==ERROR
 */
void test2_object_set_dependency();
/**
 * @test Test function for object_dependency getting
 * @pre object pointer to object, Id id_object_dependency=11
 * @post Output==11
 */
void test1_object_get_dependency();
/**
 * @test Test function for object_dependency getting
 * @pre object pointer to object==NULL
 * @post Output==NO_ID
 */
void test2_object_get_dependency();
/**
 * @test Test function for object_open setting
 * @pre object pointer to object, Id id_link=15
 * @post Output==OK
 */
void test1_object_set_open();
/**
 * @test Test function for object_open setting
 * @pre object pointer to object==NULL , Id id_link=15
 * @post Output==ERROR
 */
void test2_object_set_open();
/**
 * @test Test function for object_open setting
 * @pre object pointer to object, Id id_link=11
 * @post Output==11
 */
void test1_object_get_open();
/**
 * @test Test function for object_open setting
 * @pre object pointer to object==NULL, Id id_link=15
 * @post Output==NO_ID
 */
void test2_object_get_open();
/**
 * @test Test function for object_illuminate setting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==OK
 */
void test1_object_set_illuminate();
/**
 * @test Test function for object_illuminate setting
 * @pre object pointer to object, Bool movable==5
 * @post Output==ERROR
 */
void test2_object_set_illuminate();
/**
 * @test Test function for object_illuminate Getting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==TRUE
 */
void test1_object_get_illuminate();
/**
 * @test Test function for object_illuminate Getting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==-1
 */
void test2_object_get_illuminate();
/**
 * @test Test function for object_turnedon setting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==OK
 */
void test1_object_set_turnedon();
/**
 * @test Test function for object_turnedon setting
 * @pre object pointer to object, Bool movable==5
 * @post Output==ERROR
 */
void test2_object_set_turnedon();
/**
 * @test Test function for object_turnedon Getting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==TRUE
 */
void test1_object_get_turnedon();
/**
 * @test Test function for object_turnedon Getting
 * @pre object pointer to object, Bool movable==TRUE
 * @post Output==-1
 */
void test2_object_get_turnedon();



#endif
