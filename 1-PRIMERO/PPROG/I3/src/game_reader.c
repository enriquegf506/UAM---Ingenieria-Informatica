/** 
 * @brief It implements the game reader and all the associated calls
 * for each command
 * 
 * @file game_reader.c
 * @author Iñigo Alvarez
 * @version 2.0 
 * @date 21-02-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

/* load the spaces using the hormiguero.h */
STATUS game_load_spaces(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  char gdesc1[WORD_SIZE] = "";
  char gdesc2[WORD_SIZE] = "";
  char gdesc3[WORD_SIZE] = "";
  char gdesc4[WORD_SIZE] = "";
  char gdesc5[WORD_SIZE] = "";
  char spacedesc[WORD_SIZE] = "";
  Id id = NO_ID;
  Space *space = NULL;
  STATUS status = OK;

  if (!filename)
  {
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#s:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      strcpy(gdesc1, toks);
      toks = strtok(NULL, "|");
      strcpy(gdesc2, toks);
      toks = strtok(NULL, "|");
      strcpy(gdesc3, toks);
      toks = strtok(NULL, "|");
      strcpy(gdesc4, toks);
      toks = strtok(NULL, "|");
      strcpy(gdesc5, toks);
      toks = strtok(NULL, "|");
      strcpy(spacedesc, toks);
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld|%ld|%ld|%ld\n", id, name, north, east, south, west);
#endif
      space = space_create(id);
      if (space != NULL)
      {
        space_set_name(space, name);
        space_set_gdesc(space, gdesc1, 0);
        space_set_gdesc(space, gdesc2, 1);
        space_set_gdesc(space, gdesc3, 2);
        space_set_gdesc(space, gdesc4, 3);
        space_set_gdesc(space, gdesc5, 4);
        space_set_description(space, spacedesc);
        game_add_space(game, space);
      }
    }
  }

  if (ferror(file))
  {
    status = ERROR;
  }

  fclose(file);

  return status;
}

/**
 *It load the objects.
 */
STATUS game_load_objects(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, space_id = NO_ID;
  Object *object = NULL;
  STATUS status = OK;
  char objdescr[WORD_SIZE] = "";

/*Error handling returning ERROR if there are failures*/
  if (!filename)
  {
    return ERROR;
  }

/* Error handling*/
  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }
/* Copying the space file*/

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#o:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      space_id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(objdescr, toks);
      
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld\n", id, name, space_id);
#endif
      object = object_create(id);
      
      /* Error handling*/
      if (object != NULL)
      {
        object_set_name(object, name);
        object_set_description(object, objdescr);
        space_add_object(game_get_space(game, space_id), id);
        game_add_obj(game, object);
      }
    }
  }

/* Error handling*/
  if (ferror(file))
  {
    status = ERROR;
  }
/* Free memory*/

  fclose(file);

  return status;
}

/**
   load the game players
*/
STATUS game_load_players(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  int health = 0, max = 0;
  Id id = NO_ID, location = NO_ID;
  Player *player = NULL;
  STATUS status = OK;

/*Error handling returning ERROR if there are failures*/
  if (!filename)
  {
    return ERROR;
  }

/* Error handling*/
  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }
/* Copying the space file*/

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#p:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      location = atol(toks);
      toks = strtok(NULL, "|");
      health = atol(toks);
      toks = strtok(NULL, "|");
      max = atol(toks);
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld\n", id, name, space_id);
#endif
      player = player_create(id);
      
      /* Error handling*/
      if (player != NULL)
      {
        player_set_name(player, name);
        player_set_location(player, location);
        player_set_health(player, health);
        player_set_max(player, max);
        game_add_player(game, player);
      }
    }
  }

/* Error handling*/
  if (ferror(file))
  {
    status = ERROR;
  }
/* Free memory*/

  fclose(file);

  return status;
}
/**
   load the game enemies
*/
STATUS game_load_enemies(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  int health = 0;
  Id id = NO_ID, location = NO_ID;
  Enemy *enemy = NULL;
  STATUS status = OK;

/*Error handling returning ERROR if there are failures*/
  if (!filename)
  {
    return ERROR;
  }

/* Error handling*/
  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }
/* Copying the space file*/

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#e:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      location = atol(toks);
      toks = strtok(NULL, "|");
      health = atol(toks);
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld\n", id, name, space_id);
#endif
      enemy = enemy_create(id);
      
      /* Error handling*/
      if (enemy != NULL)
      {
        enemy_set_name(enemy, name);
        enemy_set_location(enemy, location);
        enemy_set_health(enemy, health);
        game_add_enemy(game, enemy);
      }
    }
  }

/* Error handling*/
  if (ferror(file))
  {
    status = ERROR;
  }
/* Free memory*/

  fclose(file);

  return status;
}
/**
   load the game links
*/
STATUS game_load_links(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, origin = NO_ID, dest = NO_ID;
  LINKSTATUS state;
  DIRECTION direction;
  STATUS status = OK;
  Link *link = NULL;

  if (!filename)
  {
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#l:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      origin = atol(toks);
      toks = strtok(NULL, "|");
      dest = atol(toks);
      toks = strtok(NULL, "|");
      direction = atol(toks);
      toks = strtok(NULL, "|");
      state = atol(toks);
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld|%ld|%ld|%ld\n", id, name, north, east, south, west);
#endif
      link = link_create(id);
      if (link != NULL)
      {
        link_set_name(link, name);
        link_set_origin(link, origin);
        link_set_dest(link, dest);
        link_set_direction(link, direction);
        link_set_state(link, state);
        game_add_link(game, link);
      }
    }
  }

  if (ferror(file))
  {
    status = ERROR;
  }

  fclose(file);

  return status;
}