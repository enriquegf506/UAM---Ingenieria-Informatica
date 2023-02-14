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
#include "game_reader.h"

/* load the spaces using the hormiguero.dat */
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
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID;
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
      north = atol(toks);
      toks = strtok(NULL, "|");
      east = atol(toks);
      toks = strtok(NULL, "|");
      south = atol(toks);
      toks = strtok(NULL, "|");
      west = atol(toks);
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
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld|%ld|%ld|%ld\n", id, name, north, east, south, west);
#endif
      space = space_create(id);
      if (space != NULL)
      {
        space_set_name(space, name);
        space_set_north(space, north);
        space_set_east(space, east);
        space_set_south(space, south);
        space_set_west(space, west);
        space_set_gdesc(space, gdesc1, 0);
        space_set_gdesc(space, gdesc2, 1);
        space_set_gdesc(space, gdesc3, 2);
        space_set_gdesc(space, gdesc4, 3);
        space_set_gdesc(space, gdesc5, 4);
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

STATUS game_create_from_file(Game *game, char *filename, unsigned long i)
{

  if (game_create(game) == ERROR)
    return ERROR;
  if (i == 0)
  {
    if (game_load_spaces(game, filename) == ERROR)
      return ERROR;
    if (game_load_objects(game, filename) == ERROR)
      return ERROR;
  }
   player_set_location(game->pl, 11);
   enemy_set_location(game->enemy, 123);

  return OK;
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
      
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld\n", id, name, space_id);
#endif
      object = object_create(id);
      
      /* Error handling*/
      if (object != NULL)
      {
        object_set_name(object, name);
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
