/** 
 * @brief It declares the tests for the link module
 * 
 * @file link_test.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 11-04-2021
 * @copyright GNU Public License
 */
#ifndef LINK_TEST_H
#define LINK_TEST_H
#include "link.h"
#include "test.h"
#include "types.h"

/**
 * @test Test link creation
 * @pre link ID 
 * @post Non NULL pointer to link 
 */
void test1_link_create();
/**
 * @test Test link creation
 * @pre link ID 
 * @post linkr_ID == Supplied link Id
 */
void test2_link_create();
/**
 * @test Test function for link_name setting
 * @pre String with link name
 * @post Ouput==OK 
 */
void test1_link_set_name();
/**
 * @test Test function for link_name setting
 * @pre pointer to link = NULL 
 * @post Output==ERROR
 */
void test2_link_set_name();
/**
 * @brief Test function for link_name getting
 * @pre link, name= 'adios'
 *@post Output==0
 */
void test1_link_get_name();
/**
 * @brief Test function for link_name getting
 * @pre link=NULL
 *@post Output==NULL
 */
void test2_link_get_name();
/**
 * @test Test link_origin setting
 * @pre link ID 
 * @post Output==OK
 */
void test1_link_set_origin();
/**
 * @test Test link_origin setting
 * @pre link =NULL
 * @post Output==ERROR
 */
void test2_link_set_origin();
/**
 * @test Test link_origin getting
 * @pre link , Origin Id==6
 * @post Output==6
 */
void test1_link_get_origin();
/**
 * @test Test link_origin getting
 * @pre link =NULL
 * @post Output==NO_ID
 */
void test2_link_get_origin();
/**
 * @test Test link_destination setting
 * @pre link, destination_ID ==4
 * @post Output==OK
 */
void test1_link_set_dest();
/**
 * @test Test link_destination setting
 * @pre link =NULL
 * @post Output==ERROR
 */
void test2_link_set_dest();
/**
 * @test Test link_destination getting
 * @pre link , Destination Id==4
 * @post Output==4
 */
void test1_link_get_dest();
/**
 * @test Test link_destination getting
 * @pre link =NULL
 * @post Output==NO_ID
 */
void test2_link_get_dest();
/**
 * @test Test link_state setting
 * @pre link, link status==NULL
 * @post Output==ERROR
 */
void test1_link_set_state();
/**
 * @test Test link_state setting
 * @pre link , link status= CLOSE
 * @post Output==OK
 */
void test2_link_set_state();
/**
 * @test Test link_state getting
 * @pre link =NULL
 * @post Output==-1
 */
void test1_link_get_state();
/**
 * @test Test link_state getting
 * @pre link , link state==CLOSE
 * @post Output==CLOSE
 */
void test2_link_get_state();
/**
 * @test Test link_direction setting
 * @pre link, link direction==N
 * @post Output==OK
 */
void test1_link_set_direction();
/**
 * @test Test link_direction setting
 * @pre link, link direction==NULL
 * @post Output==ERROR
 */
void test2_link_set_direction();
/**
 * @test Test link_direction getting
 * @pre link =NULL
 * @post Output==-1
 */
void test1_link_get_direction();
/**
 * @test Test link_direction getting
 * @pre link , link direction==N
 * @post Output==N
 */
void test2_link_get_direction();

/**
 * @brief Test function for link_id getting
 * @pre link ID==5, link destination==6;
 *@post Output==5
 */

void test1_link_get_id();
/**
 * @brief Test function for link_id getting
 * @pre link =NULL
 *@post Output==NO_ID
 */
void test2_link_get_id();
/**
 * @test Test link_origin destroying
 * @pre link ID 
 * @post Output==OK
 */
void test1_link_destroy();
/**
 * @test Test link_origin destroying
 * @pre link =NULL
 * @post Output==ERROR
 */
void test2_link_destroy();
/**
 * @test Test link_origin printing
 * @pre link ID 
 * @post Output==OK
 */
void test1_link_print();
/**
 * @test Test link_origin printing
 * @pre link =NULL
 * @post Output==ERROR
 */
void test2_link_print();
#endif
 
 
