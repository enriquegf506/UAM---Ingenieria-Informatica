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
 * @brief Test function for space_name setting
 * @pre space name= NULL
 *@post Output==ERROR
 */
void test1_space_set_north();
/**
 * @brief Test function for space north setting
 * @pre Space ID
 *@post Output==OK
 */
void test2_space_set_north();
/**
 * @brief Test function for space_north setting
 * @pre space Id= NO_ID
 *@post Output==ERROR
 */
void test3_space_set_north();

/**
 * @brief Test function for space north setting
 * @pre pointer to space_name = NULL (point to space = NON NULL) 
 *@post Output==ERROR
 */
void test1_space_set_south();
/**
 * @brief Test function for space  setting
 * @pre Space ID
 *@post Output==OK
 */
void test2_space_set_south();
/**
 * @brief Test function for space south setting
 * @pre pointer to space_name = NULL (point to space = NON NULL) 
 *@post Output==ERROR
 */
void test3_space_set_south();
/**
 * @brief Test function for space_south setting
 * @pre space Id= NULL
 *@post Output==ERROR
 */
void test1_space_set_east();
/**
 * @brief Test function for space east setting
 * @pre Space ID
 *@post Output==OK
 */
void test2_space_set_east();
/**
 * @brief Test function for space east setting
 * @pre pointer to space_name = NULL (point to space = NON NULL) 
 *@post Output==ERROR
 */
void test3_space_set_east();
/**
 * @brief Test function for space_east setting
 * @pre space Id= N0_ID
 *@post Output==ERROR
 */
void test1_space_set_west();
/**
 * @brief Test function for space west setting
 * @pre Space ID
 *@post Output==OK
 */
void test2_space_set_west();
/**
 * @brief Test function for space west setting
 * @pre pointer to space_name = NULL (point to space = NON NULL) 
 *@post Output==ERROR
 */
 
 /**
 * @brief Test function for space_west setting
 * @pre space Id= NO_ID
 *@post Output==ERROR
 */
void test3_space_set_west();


/**
 * @brief Test function for space_id getting
 * @pre space 
 *@post Output==space ID
 */
void test1_space_get_id();


/**
 * @brief Test function for space_id getting
 * @pre space =NULL
 *@post Output==NO_ID
 */
void test2_space_get_id();


/**
 * @brief Test function for space_object setting
 * @pre object = TRUE
 *@post Output==OK
 */
void test1_space_add_object();


/**
 * @brief Test function for space_object setting
 * @pre space=NULL
 *@post Output==ERROR
 */
void test2_space_add_object();


/**
 * @brief Test function for space_name getting
 * @pre space, name= 'adios'
 *@post Output==0
 */
void test1_space_get_name();

 /**
 * @brief Test function for space_name getting
 * @pre space=NULL
 *@post Output==NULL
 */
void test2_space_get_name();


/**
 * @brief Test function for space_north getting
 * @pre space ID
 *@post Output==4
 */
 void test1_space_get_north();


/**
 * @brief Test function for space_north getting
 * @pre space=NULL
 *@post Output==NO_ID
 */

void test2_space_get_north();

/**
 * @brief Test function for space_south getting
 * @pre space ID
 *@post Output==4
 */
void test1_space_get_south();


/**
 * @brief Test function for space_south getting
 * @pre space=NULL
 *@post Output==NO_ID
 */
void test2_space_get_south();

 
 /**
 * @brief Test function for space_east getting
 * @pre space ID
 *@post Output==4
 */
void test1_space_get_east();

 
 /**
 * @brief Test function for space_east getting
 * @pre space=NULL
 *@post Output==NO_ID
 */
void test2_space_get_east();

 
 /**
 * @brief Test function for space_west getting
 * @pre space ID
 *@post Output==4
 */
void test1_space_get_west();

 
 /**
 * @brief Test function for space_west getting
 * @pre space=NULL
 *@post Output==NO_ID
 */
void test2_space_get_west();

 
 /**
 * @brief Test function for space_object getting
 * @pre space ID
 *@post Output==FALSE
 */
void test1_space_get_object();

 
 /**
 * @brief Test function for space_object getting
 * @pre space ID
 *@post Output==TRUE
 */
void test2_space_get_object();

 
 /**
 * @brief Test function for space_object getting
 * @pre space=NULL
 *@post Output==FALSE
 */
void test3_space_get_object();
/**
 * @test Test function for space_graphic description setting
 * @pre String with space gdesc
 * @post Ouput==OK 
 */
void test1_space_set_gdesc();
/**
 * @test Test function for space_graphic description setting
 * @pre pointer to space = NULL 
 * @post Output==ERROR
 */
void test2_space_set_gdesc();
/**
 * @test Test function for space_graphic_description getting
 * @pre String with space gdesc=="hola"
 * @post Output == "hola"
 */
void test1_space_get_gdesc();
/**
 * @test Test function for space_graphic_description getting
 * @pre pointer to space=NULL
 * @post Output == NULL
 */
void test2_space_get_gdesc();
#endif
