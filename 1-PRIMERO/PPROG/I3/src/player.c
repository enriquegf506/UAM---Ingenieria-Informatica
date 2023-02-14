/**
 * @brief Funcionalidad para el manejo de los jugadores
 *
 * @file player.h
 * @author Iñigo Alvarez
 * @version 2.0
 * @date 16-02-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

/**
 * @brief Player
 *
 * This struct stores all the information of a player.
 */
struct _Player {
  Id id;                    /*!< Id number of the player, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the player */
  Id location;              /*!< Location of the player*/
  Inventory *objs;              /*!< object or not */
  int health;		/*!< Health of the player*/
};

/** player_create allocates memory for a new player
  */
Player* player_create(Id id){
    Player *newPlayer = NULL;

    /* Error control */
  if (id == NO_ID)
    return NULL;

  newPlayer = (Player *) malloc(sizeof (Player));
  if (newPlayer == NULL)
    return NULL;

    /* Initialization of an new player*/
  newPlayer->id = id;
  newPlayer->name[0] = '\0';
  newPlayer->objs = inventory_create();
  newPlayer->location = NO_ID;
  newPlayer->health= PL_HEALTH;

  return newPlayer;

}

/** player_destroy frees the previous memory allocation 
  *  for a player
  */
STATUS player_destroy(Player* player){
    if (!player) {
    return ERROR;
  }
  inventory_destroy(player->objs);
  free(player);
  player = NULL;
  return OK;
}

/** It gets the location of a player
  */
Id player_get_location(Player* player){
    if (!player) {
    return NO_ID;
  }
  return player->location;
}

/** It gets the id of a player
  */
Id player_get_id(Player* player){
    if (!player) {
    return NO_ID;
  }
  return player->id;
}

/** It sets the location of a player
  */
STATUS player_set_location(Player* player, Id id){
  if (!player || id == NO_ID) {
    return ERROR;
  }
  player->location = id;
  return OK;
}

/** It sets the name of a player
  */
STATUS player_set_name(Player* player, char* name){
    if (!player || !name) {
    return ERROR;
  }

  if (!strcpy(player->name, name)) {
    return ERROR;
  }
  return OK;
    if (!player) {
    return NO_ID;
  }
  return player->id;

}

/** It gets the name of a player
  */
const char* player_get_name(Player* player){
    if (!player){
      return NULL;
    }
  return player->name;
}
/** It sets whether the player has an object or not
  */
STATUS player_add_object(Player* player, Id id){
    if (!player) {
    return ERROR;
  }
  inventory_add(player->objs, id);
  return OK;
}

/** It gets whether the player has an object or not
  */
Inventory *player_get_objects(Player* player){
    if (!player) {
    return NULL;
  }
  return player->objs;
}

STATUS player_del_object(Player* player, Id id) {
  if (!player) {
    return ERROR;
  }
  if (inventory_delete(player->objs, id) == ERROR){
    return ERROR ;
  }
  return OK;
}

/** It sets the health of a player
  */
STATUS player_set_health(Player* player, int health){
/* Error control */
  if (!player){
    return ERROR;
  }

  player->health = health;
    

  return OK;
}

STATUS player_set_max(Player *player, int max)
{
  if (!player)
    return ERROR;

  inventory_set_max(player->objs, max);
  return OK;
}

int player_get_max(Player *player)
{
  if (!player)
    return -1;

  return inventory_get_max(player->objs);
}

BOOL player_contain_object(Player* player, Id id)
{
  BOOL st = FALSE;
  if (!player)
    return (FALSE);
  st = inventory_findId(player->objs, id);
  return st;
}
/** It gets the health of a player
  */
int player_get_health(Player* player) {
/* Error control */
  if (!player){
    return -1;
  }

  return player->health;
}

/** It prints the player data
  */
STATUS player_print(Player* player){

  /* Error control */
  if (!player) {
    return ERROR;
  }

  /* 1. Prints the player id, the name and location */
  fprintf(stdout, "--> Player (Id: %ld; Name: %s; Location: %ld; Health: %d)\n", player->id, player->name, player->location, player->health);
  
  /* 2. Prints if the player has an object or not */
  /*if (player_get_object(player)) {
    fprintf(stdout, "---> Player owns an object.\n");
  } else {
    fprintf(stdout, "---> Player do not owns an object.\n");
  }*/

  return OK;
}

