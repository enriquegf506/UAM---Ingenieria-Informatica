/**
 * @brief It defines the enemy interface
 *
 * @file enemy.h
 * @author Enrique Gómez 
 * @version 2.0
 * @date 27-02-2022
 * @copyright GNU Public License
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"

#define EN_HEALTH 10  /*!< Health of the enemy*/
#define ENEMY_ID 1    /*!< Id of the enemy*/


typedef struct _Enemy Enemy;

/**
  * @brief It creates a player
  * @author Enrique Gómez
  *
  * enemy_create allocates memory for a new enemy 
  *  
  * @param id the identification number for the new enemy 
  * @return a new enemy initialized if everything goes well or NULL if there was some mistake
  */
Enemy* enemy_create(Id id);

/**
  * @brief It destroys an enemy
  * @author Enrique Gómez
  *
  * enemy_destroy frees the previous memory allocation 
  *  for a enemy
  * @param enemy a pointer to the enemy that must be destroyed  
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS enemy_destroy(Enemy* enemy);

/**
  * @brief It sets the name of a enemy
  * @author Enrique Gómez
  * 
  * @param enemy a pointer to the enemy
  * @param the id to store
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS enemy_set_id(Enemy* enemy, Id id);


/**
  * @brief It gets the id of an enemy
  * @author Enrique Gómez
  * 
  * @param enemy a pointer to the enemy  
  * @return the id of the enemy
  */
Id enemy_get_id(Enemy* enemy);

/**
  * @brief It sets the name of an enemy
  * @author Enrique Gómez
  * 
  * @param enemy a pointer to the enemy
  * @param name a string with the name to store
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS enemy_set_name(Enemy* enemy, char* name);

/**
  * @brief It gets the name of a enemy
  * @author Enrique Gómez
  * 
  * @param enemy a pointer to the enemy
  * @return  a string with the name of the enemy
  */
const char * enemy_get_name(Enemy* enemy);

/**
  * @brief It gets the id location of the enemy 
  * @author Enrique Gómez
  *
  * @param enemy a pointer to the enemy
  * @return the id number of the enemy location
  */
Id enemy_get_location(Enemy* enemy);

/**
  * @brief It sets the id of the enemy location
  * @author Enrique Gómez
  *
  * @param enemy a pointer to the enemy
  * @param id the id number of the enemy location
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS enemy_set_location(Enemy* enemy, Id id);

/**
  * @brief It gets the enemy's health
  * @author Enrique Gómez
  *
  * @param enemy a pointer to the enemy
  * @return enemy's health or NULL  if something goes wrong and the enemy's health 
  */
int enemy_get_health(Enemy* enemy) ;

/**
  * @brief It sets the enemy's health
  * @author Enrique Gómez
  *
  * @param enemy a pointer to the enemy
  * @param enemy's health
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS enemy_set_health(Enemy* enemy, int health);

/**
  * @brief It prints the enemy information
  * @author Enrique Gómez
  *
  * This fucntion shows the id, the name, the location and health of the enemy
  * @param enemy a pointer to the enemy
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS enemy_print(Enemy* enemy);

#endif
