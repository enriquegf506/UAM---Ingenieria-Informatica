/** 
 * @brief It implements the game interface and all the associated calls
 * for each command
 * 
 * @file game.c
 * @author Iñigo Alvarez
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

/* Private functions */

/**
 * @brief It is executed if the command is unknown
 * @param game pointer to game
 */
STATUS game_command_unknown(Game *game);
/**
 * @brief It is executed if the command is exit
 * @param game pointer to game
 */
STATUS game_command_exit(Game *game);
/**
 * @brief It is executed if the command is next
 * @param game pointer to game
 */
STATUS game_command_next(Game *game);
/**
 * @brief It is executed if the command is back
 * @param game pointer to game
 */
STATUS game_command_back(Game *game);
/**
 * @brief It is executed if the command is left
 * @param game pointer to game
 */
STATUS game_command_left(Game *game);
/**
 * @brief It is executed if the command is right
 * @param game pointer to game
 */
STATUS game_command_right(Game *game);
/**
 * @brief It is executed if the command is take
 * @param game pointer to game
 */
STATUS game_command_take(Game *game);
/**
 * @brief It is executed if the command is drop
 * @param game pointer to game
 */
STATUS game_command_drop(Game *game);
/**
 * @brief It is executed if the command is attack
 * @param game pointer to game
 */
STATUS game_command_attack(Game *game);


/*Inicializes the game*/
STATUS game_create(Game *game)
{
  int i;
  for (i = 0; i < MAX_SPACES; i++)
  {
    game->spaces[i] = NULL;
  }
  game->pl = player_create(1);
  player_set_location(game->pl, 11);
  for (i = 0; i < MAX_OBJECTS; i++)
  {
    game->obj[i] = NULL;
  }
  game->last_cmd = NO_CMD;
  game->enemy = enemy_create(1);

  return OK;
}

/* Destroys the space of the game */
STATUS game_destroy(Game *game)
{
  int i = 0;
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    space_destroy(game->spaces[i]);
  }

  return OK;
}

/*adds a space for the game*/
STATUS game_add_space(Game *game, Space *space)
{
  int i = 0;
  /*ERROR CONTROL*/
  if (space == NULL)
  {
    return ERROR;
  }

  while (i < MAX_SPACES && game->spaces[i] != NULL)
  {
    i++;
  }

  if (i >= MAX_SPACES)
  {
    return ERROR;
  }
  game->spaces[i] = space;

  return OK;
}

STATUS game_add_obj(Game *game, Object *obj)
{
  int i = 0;
  /*ERROR CONTROL*/
  if (obj == NULL)
  {
    return ERROR;
  }

  while (i < MAX_OBJECTS && game->obj[i] != NULL)
  {
    i++;
  }

  if (i >= MAX_OBJECTS)
  {
    return ERROR;
  }
  game->obj[i] = obj;

  return OK;
}

/*obtains the ID of the space in which it is located*/
Id game_get_space_id_at(Game *game, int position)
{
  if (position < 0 || position >= MAX_SPACES)
  {
    return NO_ID;
  }
  //Obtiene la id de la posicion
  return space_get_id(game->spaces[position]);
}

/*obtains the game space that corresponds with the id*/
Space *game_get_space(Game *game, Id id)
{
  int i = 0;
  /*ERROR CONTROL*/
  if (id == NO_ID)
  {
    return NULL;
  }
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    if (id == space_get_id(game->spaces[i]))
    {
      return game->spaces[i];
    }
  }

  return NULL;
}

STATUS game_set_state(Game *game, STATUS state){
  if(!game){
    return ERROR;
  }
  game->state = state;
  return OK;
}

STATUS game_get_state(Game *game){
  if(!game){
    return ERROR;
  }
  return game->state;
}


/*Actualices the game depending on the command introduced*/
STATUS game_update(Game *game, T_Command cmd)
{
  game->last_cmd = cmd;
  switch (cmd)
  {
    case UNKNOWN:
      if (game_command_unknown(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;

    case EXIT:
      if (game_command_exit(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;

    case NEXT:
      if (game_command_next(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;

    case BACK:
      if (game_command_back(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;
    
    case LEFT:
      if (game_command_left(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;

    case RIGHT:
      if (game_command_right(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;

    case TAKE:
      if (game_command_take(game)==OK){
        game_set_state(game, OK);
      }
      else {
        game_set_state(game, ERROR);
      }
      break;

    case DROP:
      if (game_command_drop(game)==OK){
        game_set_state(game, OK);
      }
      else {
        game_set_state(game, ERROR);
      }
      break;
      
    case ATTACK:
      if (game_command_attack(game)==OK){
        game_set_state(game, OK);
      }
      else{
        game_set_state(game, ERROR);
      }
      break;

    default:
      break;
  }

  return OK;
}

/*Takes the last command*/
T_Command game_get_last_command(Game *game)
{
  return game->last_cmd;
}

/*Prints the game data*/
void game_print_data(Game *game)
{
  printf("\n\n-------------\n\n");

  printf("=> Spaces: \n");
  // for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  // {
  //   space_print(game->spaces[i]);
  // }
  /*Printea rhe location of the game and player*/
  //printf("=> Object location: %d\n", (int)object_get_id(game->obj));
  printf("=> Player location: %d\n", (int)player_get_id(game->pl));
}

/*Determines that the game is finish*/
BOOL game_is_over(Game *game)
{
  if (player_get_health(game->pl) == 0){
    return TRUE;
  }
  return FALSE;
}

/**
   Calls implementation for each action 
*/

STATUS game_command_unknown(Game *game)
{
  return ERROR;
}

STATUS game_command_exit(Game *game)
{
  return OK;
}

STATUS game_command_next(Game *game)
{
  /*Put all the variables to 0*/
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;
  space_id = player_get_location(game->pl);
  if (space_id == NO_ID)
  {
    return ERROR;
  }
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id)
    {
      current_id = space_get_south(game->spaces[i]);
      if (current_id != NO_ID)
      {
        /*New player id*/
        player_set_location(game->pl, current_id);
        return OK;
      }
    }
  }
  return ERROR;
}

/*Lo same that next but on the contrary*/

STATUS game_command_back(Game *game)
{
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = player_get_location(game->pl);

  if (NO_ID == space_id)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id)
    {
      current_id = space_get_north(game->spaces[i]);
      if (current_id != NO_ID)
      {
        player_set_location(game->pl, current_id);
        return OK;
      }
    }
  }
  return ERROR;
}


STATUS game_command_left(Game *game)
{
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = player_get_location(game->pl);

  if (NO_ID == space_id)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id)
    {
      current_id = space_get_west(game->spaces[i]);
      if (current_id != NO_ID)
      {
        player_set_location(game->pl, current_id);
        return OK;
      }
    }
  }
  return ERROR;
}


STATUS game_command_right(Game *game)
{
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = player_get_location(game->pl);

  if (NO_ID == space_id)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id)
    {
      current_id = space_get_east(game->spaces[i]);
      if (current_id != NO_ID)
      {
        player_set_location(game->pl, current_id);
        return OK;
      }
    }
  }
  return ERROR;
}

/*Take the object*/
STATUS game_command_take(Game *game)
{
  Id space_id = player_get_location(game->pl);
  Id obj;
  char input;

  if(player_get_object(game->pl) != NO_ID)
    return ERROR;
  scanf(" %c", &input);
  if(input != 'O'){
    return ERROR;
  }
  scanf(" %ld", &obj);

  
  if (space_contain_object(game_get_space(game, space_id), obj))
  {
    player_set_object(game->pl, obj);
    space_del_object(game_get_space(game, space_id), obj);
  } 
  else
    return ERROR;
  
  return OK;
}

/*Drop the object*/
STATUS game_command_drop(Game *game)
{
  if(player_get_object(game->pl) == NO_ID)
    return ERROR;
  
  Id space_id = player_get_location(game->pl);

  space_add_object(game_get_space(game, space_id), player_get_object(game->pl));
  player_set_object(game->pl,NO_ID);
  return OK;
}

/**
 * game_command_attack make a fight between enemy and player */
STATUS game_command_attack(Game *game){
  int x;
/*Error control*/
  if (!game)
    return ERROR;
  if(player_get_location(game->pl)!=enemy_get_location(game->enemy))
    return ERROR;
  if (enemy_get_health(game->enemy) <= 0)
    return ERROR;
  x = rand() % MAX_RAND;
/*set new health*/
  if (x < 5){
    if (player_set_health(game->pl, player_get_health(game->pl) - 1) == ERROR)
      return ERROR;
  }

  else{
    if (enemy_set_health(game->enemy, enemy_get_health(game->enemy) - 1) == ERROR)
      return ERROR;
  }
  return OK;
}

