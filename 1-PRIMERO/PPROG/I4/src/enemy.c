/**
 * @brief It implements the enemy module
 *
 * @file enemy.c
 * @author Iñigo Alvarez
 * @version 2.0
 * @date 27-02-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enemy.h"


/**
 * @brief Enemy
 *
 * This struct stores all the information of a enemy.
 */
struct _Enemy{
    Id id;                    /*!< Id number of the space, it must be unique */
    char name[WORD_SIZE + 1]; /*!< Name of the enemy */
    Id location;              /*!< Id of the enemy location */
    int health;             /*!< Id of the enemy health */
};


/** enemy_create allocates memory for a new enemy
  *  and initializes its members
  */
Enemy* enemy_create(Id id){
  Enemy *newEnemy = NULL;
/* Error control */
  if (id==NO_ID)
    return NULL;

  newEnemy = (Enemy *) malloc(sizeof (Enemy));
  if (newEnemy == NULL)
    return NULL;

/* Initialization of an empty space*/
  newEnemy->id = id;
  newEnemy->name[0] = '\0';
  newEnemy->location = NO_ID;
  newEnemy->health= EN_HEALTH;
  
  return newEnemy;
}

/** enemy_destroy frees the previous memory allocation 
  *  for an enemy
  */
STATUS enemy_destroy(Enemy* enemy){
  /* Error control */

  if (!enemy)
    return ERROR;

  free(enemy);
  enemy = NULL;
  
  return OK;
}

/** It gets the id of an enemy
  */
Id enemy_get_id(Enemy* enemy){
/* Error control */

  if (!enemy){
    return NO_ID;
  }
  
  return enemy->id;
}

/** It gets the id of an enemy
  */
STATUS enemy_set_id(Enemy* enemy, Id id){
/* Error control */

  if(!enemy||id==NO_ID){
    return ERROR;
  }
  
  enemy->id= id;
  
  return OK;
}

/** It sets the name of an enemy
  */
STATUS enemy_set_name(Enemy* enemy, char* name) {
/* Error control */
  if (!enemy || !name){
    return ERROR;
  }

  if (!strcpy(enemy->name, name)){
    return ERROR;
  }

  return OK;
}

/** It gets the name of an enemy
  */
const char * enemy_get_name(Enemy* enemy) {
/* Error control */
  if (!enemy){
    return NULL;
  }

  return enemy->name;
}

/** It gets the location of an enemy
  */
Id enemy_get_location(Enemy* enemy){
  if(!enemy){
    return NO_ID;
  }
  
  return enemy->location;
}

/** It sets the location of an enemy
  */
STATUS enemy_set_location(Enemy* enemy, Id id){
  if(!enemy||id==NO_ID){
    return ERROR;
  }

  enemy->location=id;
  
  return OK;
}

/** It gets the health points of an enemy
  */
STATUS enemy_set_health(Enemy* enemy, int health){
/* Error control */
  if (!enemy){
    return ERROR;
  }

  enemy->health = health;
    

  return OK;
}
/** It sets the health points of an enemy
  */

int enemy_get_health(Enemy* enemy) {
/* Error control */
  if (!enemy){
    return -1;
  }

  return enemy->health;
}

/** It prints the enemy information
  */
STATUS enemy_print(Enemy* enemy){

  if (!enemy){
    return ERROR;
  }
 /* Print the id, the name, the location and health of an enemy */
  fprintf(stdout, "-->enemy (id: %ld; name: %s; location: %ld; health %d)\n", enemy->id, enemy->name,enemy->location,enemy->health);
  

  return OK;
}
