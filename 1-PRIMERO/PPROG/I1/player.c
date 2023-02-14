/** 
 * @brief It implements the player module
 * 
 * @file player.c
 * @author Enrique Gómez
 * @version 2.0 
 * @date 19-02-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"



struct _Player{
    Id id;
    char name[WORD_SIZE + 1];
    Id location;
    Id object_id;
};


Player* player_create(Id id){
  Player *newPlayer = NULL;

  if (id==NO_ID)
    return NULL;

  newPlayer = (Player *) malloc(sizeof (Player));
  if (newPlayer == NULL)
    return NULL;

  newPlayer->id = id;
  newPlayer->name[0] = '\0';
  newPlayer->location = NO_ID;
  newPlayer->object_id = NO_ID;
  return newPlayer;
}


STATUS player_destroy(Player* player){
  if (!player)
    return ERROR;

  free(player);
  player = NULL;
  return OK;
}


Id player_get_id(Player* player){
  if (!player)
    return NO_ID;

  return player->id;
}


STATUS player_set_id(Player* player, Id id){
  if(!player||id==NO_ID)
    return ERROR;

  player->id= id;
  return OK;
}


STATUS player_set_name(Player* player, char* name) {
  if (!player || !name)
    return ERROR;

  if (!strcpy(player->name, name))
    return ERROR;

  return OK;
}


const char * player_get_name(Player* player) {
  if (!player)
    return NULL;

  return player->name;
}


Id player_get_location(Player* player){
  if(!player)
    return NO_ID;

  return player->location;
}


STATUS player_set_location(Player* player, Id id){
  if(!player||id==NO_ID)
    return ERROR;

  player->location=id;
  return OK;
}


Id player_get_object_id(Player* player){
  if(!player)
    return NO_ID;

  return player->object_id;
}


STATUS player_set_object_id(Player* player, Id id){
  if(!player)
    return ERROR;

  player->object_id=id;
  return OK;
}


STATUS player_print(Player* player){
  if (!player)
    return ERROR;

  fprintf(stdout, "-->player (id: %ld; name: %s; location: %ld; object: %ld)\n", player->id, player->name,player->location,player->object_id);
  return OK;
}