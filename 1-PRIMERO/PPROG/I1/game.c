/** 
 * @brief It implements the game interface and all the associated calls
 * for each command
 * 
 * @file game.c
 * @author Profesores PPROG
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_reader.h"

#define N_CALLBACK 6

typedef void (*callback_fn)(Game* game);

/**
   List of callbacks for each command in the game
*/
void game_callback_unknown(Game* game);
void game_callback_exit(Game* game);
void game_callback_next(Game* game);
void game_callback_back(Game* game);
void game_callback_take(Game* game);
void game_callback_drop(Game* game);

static callback_fn game_callback_fn_list[N_CALLBACK]={
  game_callback_unknown,
  game_callback_exit,
  game_callback_next,
  game_callback_back,
  game_callback_take,
  game_callback_drop};



/**
   Private functions
*/
Id     game_get_space_id_at(Game* game, int position);
STATUS game_set_player_location(Game* game, Id id);
STATUS game_set_object_location(Game* game, Id id);


/**
   Game interface implementation
*/


/**
 * game_create creates the game in order to begin
 */
STATUS game_create(Game *game)
{
  int i;
/*Initialize game spaces to NULL*/
  for (i = 0; i < MAX_SPACES; i++)
  {
    game->spaces[i] = NULL;
  }
  /*Initialize game locations*/
  game->player= player_create(PLAYER_ID);
  game->object = object_create(OBJECT_ID);
  game->last_cmd = NO_CMD;

  return OK;
}

/**
 * game_create_from_file creates the game from the file given
 */
STATUS game_create_from_file(Game *game, char *filename)
{
/*Error handling returning ERROR if there are failures*/
  if (game_create(game) == ERROR)
    return ERROR;

  if (game_load_spaces(game, filename) == ERROR)
    return ERROR;

  /* The player and the object are located in the first space */
  game_set_player_location(game, game_get_space_id_at(game, 0));
  game_set_object_location(game, game_get_space_id_at(game, 0));

  return OK;
}

/**
 * game_destroy destroys game spaces for ending game
 */
STATUS game_destroy(Game *game)
{
  int i = 0;

  if(!game){
    return ERROR;
  }

  for (i = 0; game->spaces[i] != NULL; i++)
  {
    space_destroy(game->spaces[i]);
  }

  return OK;
}
/**
 * game_add_space add new space 
 */
STATUS game_add_space(Game *game, Space *space)
{
  int i = 0;
/* Error handling*/
  if (!space || !game)
  {
    return ERROR;
  }

  while (game->spaces[i] != NULL)
  {
    i++;
  }
/* Error handling*/
  if (i >= MAX_SPACES)
  {
    return ERROR;
  }
/*Creation of the new space*/
  game->spaces[i] = space;

  return OK;
}
/**
 *game_get_space_id_at gets the id of the position given
 *  
 */
Id game_get_space_id_at(Game *game, int position)
{
/* returns NO_ID if the position given is not possible*/
  if (!game || position < 0 || position >= MAX_SPACES)
  {
    return NO_ID;
  }
/* returns the id of the position*/
  return space_get_id(game->spaces[position]);
}

/**
 *game_get_space gets a space from an specific id  */
Space *game_get_space(Game *game, Id id)
{
  int i = 0;
/* Error handling of the id*/
  if (!game||id == NO_ID)
  {
    return NULL;
  }

  for (i = 0; game->spaces[i] != NULL; i++)
  {
    if (id == space_get_id(game->spaces[i]))
    {
      return game->spaces[i];
    }
  }

  return NULL;
}

/**
 * game_set_player_location gets the location of the player */
STATUS game_set_player_location(Game *game, Id id)
{

    /* Error handling of the id*/
  if (!game || id == NO_ID)
  {
    return ERROR;
  }

  player_set_location(game->player,id);
  
  return OK;
}

/**
 *  game_set_object_location gets the location of the specified object
 */
STATUS game_set_object_location(Game *game, Id id)
{
 int i;
 /* Error handling of the id*/
  if (!game||id == NO_ID)
  {
    return ERROR;
  }

  for(i=0;game->spaces[i]!=NULL;i++){
    if(space_get_id(game->spaces[i])==id){

      if(space_get_object(game->spaces[i])!=NO_ID){
          space_set_object(game->spaces[i],NO_ID);
          player_set_object_id(game->player,object_get_id(game->object));
          return OK;
        }

        if(space_get_object(game->spaces[i])==NO_ID){
          space_set_object(game->spaces[i],object_get_id(game->object));
          player_set_object_id(game->player,NO_ID);
          return OK;
        }

      }
    }

  return ERROR;
}

Id game_get_player_location(Game *game){

if(!game)
    return NO_ID;

  return player_get_location(game->player);
}

Id game_get_object_location(Game *game)
{
  int i;
  Id aux=NO_ID;

  if(!game)
    return NO_ID;

  for(i=0;game->spaces[i]!=NULL;i++){
    if(space_get_object(game->spaces[i])!=NO_ID){
      aux=space_get_id(game->spaces[i]);
      return aux;
    }
  }

 return aux;
}

/**
 * game_update updates the game with the command inserted by the player
 */
STATUS game_update(Game* game, T_Command cmd) {
  if(!game)
    return ERROR;

  game->last_cmd = cmd;
  (*game_callback_fn_list[cmd])(game);
  return OK;
}

/**
 * game_get_last_command gets the last command inserted by the player */
T_Command game_get_last_command(Game *game)
{
  return game->last_cmd;
}

/**
 * game_print_data prints the respective game data
 */
void game_print_data(Game* game) {
  int i = 0;

  printf("\n\n-------------\n\n");
  printf("=>spaces: \n");
  for (i = 0; game->spaces[i] != NULL; i++) {
    space_print(game->spaces[i]);
  }

  printf("=>object location: %ld\n", game_get_object_location(game));
  printf("=>player location: %ld\n", game_get_player_location(game));
  printf("prompt:> ");
}



/**
 * game_is_over ends game  */
BOOL game_is_over(Game *game)
{
  return FALSE;
}

/**
   Calls implementation for each action 
*/
/**
   Callbacks implementation for each action
*/

void game_callback_unknown(Game* game) {}
void game_callback_exit(Game* game) {}

void game_callback_next(Game* game) {
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = game_get_player_location(game);
  if (space_id == NO_ID)
    return;


  for (i = 0; game->spaces[i] != NULL; i++) {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id) {
      current_id = space_get_south(game->spaces[i]);
      if (current_id != NO_ID) {
	       game_set_player_location(game, current_id);
      }
      return;
    }
  }
}


void game_callback_back(Game* game) {
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = game_get_player_location(game);
  if (NO_ID == space_id)
    return;

  for (i = 0; game->spaces[i] != NULL; i++) {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id) {
      current_id = space_get_north(game->spaces[i]);
      if (current_id != NO_ID) {
		      game_set_player_location(game, current_id);
      }
      return;
    }
  }
}


void game_callback_take(Game* game){
  int i;

  if(!game)
    return;

  for(i=0;game->spaces[i]!=NULL;i++){
    if(game_get_player_location(game)==space_get_id(game->spaces[i])){
      if(player_get_object_id(game->player)==NO_ID && space_get_object(game->spaces[i])!=NO_ID){
        game_set_object_location(game,space_get_id(game->spaces[i]));
        return;
      }
    }
  }

  return;
}


void game_callback_drop(Game* game){
  int i;

  if(!game)
    return;

  for(i=0;game->spaces[i]!=NULL;i++){
    if(game_get_player_location(game)==space_get_id(game->spaces[i])){
      if(player_get_object_id(game->player)!=NO_ID && space_get_object(game->spaces[i])==NO_ID){
        game_set_object_location(game,space_get_id(game->spaces[i]));
        return;
      }
    }
  }

  return;
}
