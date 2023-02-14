/**
 * @brief It defines a textual graphic engine
 *
 * @file graphic_engine.c
 * @author Marcos alonso
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */

#include <stdlib.h>
#include <stdio.h>
#include "graphic_engine.h"
#include "libscreen.h"
#include "command.h"
#include "player.h"
#include "object.h"
#include "space.h"
#include "types.h"

#define ROWS 28     /*!< Maximum number of rows in the game*/
#define COLUMNS 80  /*!< Maximum number of columns in the game*/

/**
 * @brief Graphic_engine
 *This struct defines the game grapghic engine information
 */
struct _Graphic_engine
{
  Area *map, /*!< Map area of the game*/ 
  *descript, /*!< Descript area of the gam*/ 
  *banner, /*!< Banner area of the gam*/ 
  *help, /*!< Help area of the gam*/ 
  *feedback;  /*!< Feedback area of the game*/ 
};

Graphic_engine *graphic_engine_create()
{
  static Graphic_engine *ge = NULL;

  if (ge)
    return ge;

  screen_init(ROWS, COLUMNS);
  ge = (Graphic_engine *)malloc(sizeof(Graphic_engine));
  if (ge == NULL)
    return NULL;

  ge->map = screen_area_init(1, 1, 48, 17);
  ge->descript = screen_area_init(50, 1, 29, 17);
  ge->banner = screen_area_init(28, 19, 23, 1);
  ge->help = screen_area_init(1, 20, 78, 2);
  ge->feedback = screen_area_init(1, 23, 78, 3);

  return ge;
}

void graphic_engine_destroy(Graphic_engine *ge)
{
  if (!ge)
    return;

  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);

  screen_destroy();
  free(ge);
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game)
{
  Id id_act = NO_ID, id_back = NO_ID, id_next = NO_ID, obj1_loc = NO_ID, player_loc = NO_ID;
  Id obj1_id = 1, obj2_id = 2, obj2_loc = NO_ID, id_right = NO_ID, obj3_id = 3, obj3_loc = NO_ID;
  Id obj4_id = 4, obj4_loc = NO_ID, enemy_loc = NO_ID, id_left = NO_ID;
  Space *space_act = NULL;
  char obj1 = '\0', obj2 = '\0', obj3 = '\0', obj4 = '\0';
  char str[255];
  T_Command last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];
  int i = 0;

  /* Paint the in the map area */
  screen_area_clear(ge->map);
  if ((id_act = player_get_location(game->pl)) != NO_ID)
  {
    space_act = game_get_space(game, player_get_location(game->pl));
    id_back = space_get_north(space_act);
    id_next = space_get_south(space_act);
    id_right = space_get_east(space_act);
    id_left = space_get_west(space_act);


    if (space_contain_object(game_get_space(game, id_back), 1))
      obj1 = '*';
    else
      obj1 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 2))
      obj2 = '.';
    else
      obj2 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 3))
      obj3 = '+';
    else
      obj3 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 4))
      obj4 = '&';
    else
      obj4 = ' ';

    if (id_back != NO_ID)
    {
      sprintf(str, "  |            %2d|", (int)id_back);
      screen_area_puts(ge->map, str);
      sprintf(str, "  | %c %c %c %c       |", obj1, obj2, obj3, obj4);
      screen_area_puts(ge->map, str);
      sprintf(str, "  +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "        ^");
      screen_area_puts(ge->map, str);
    }

    if (space_contain_object(space_act, 1))
      obj1 = '*';
    else
      obj1 = ' ';
    if (space_contain_object(space_act, 2))
      obj2 = '.';
    else
      obj2 = ' ';
    if (space_contain_object(space_act, 3))
      obj3 = '+';
    else
      obj3 = ' ';
    if (space_contain_object(space_act, 4))
      obj4 = '&';
    else
      obj4 = ' ';

    if (id_act != NO_ID)
    {
      if (id_right != NO_ID)
        sprintf(str, "  +---------------+                 +-----------");
      else
        sprintf(str, "  +---------------+");
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID)
        sprintf(str, "  | m0^         %2d|         >      |           ", (int)id_act);
      else
        sprintf(str, "  | m0^         %2d|", (int)id_act);
      screen_area_puts(ge->map, str);
      if (enemy_get_location(game->enemy) == id_act)
      {
        if(enemy_get_health(game->enemy)!=0)
        {
          if (id_right == NO_ID) {
            sprintf(str, "  |  /\\00/\\       |");
          }
        }
        else
        {
          sprintf(str, "  |               |");
        }
      }
      else if (id_right != NO_ID) {
        sprintf(str, "  |               |                 |           ");
      }
      else {
        sprintf(str, "  |               |");
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s      |                 |           ", space_get_gdesc(space_act, 0));
      }
      else {
        sprintf(str, "  |%s      |", space_get_gdesc(space_act, 0));
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s      |                 |           ", space_get_gdesc(space_act, 1));
      }
      else {
        sprintf(str, "  |%s      |", space_get_gdesc(space_act, 1));
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s      |                 |           ", space_get_gdesc(space_act, 2));

      }
      else {
        sprintf(str, "  |%s      |", space_get_gdesc(space_act, 2));
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s      |                 |           ", space_get_gdesc(space_act, 3));
      }
      else {
        sprintf(str, "  |%s      |", space_get_gdesc(space_act, 3));
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID && id_left !=NO_ID) {
        sprintf(str, "< |%s      |                 |           ", space_get_gdesc(space_act, 4));
      }
      else if(id_right != NO_ID){
        sprintf(str, "  |%s      |                 |           ", space_get_gdesc(space_act, 4));
      }
      else if (id_left != NO_ID){
        sprintf(str, "< |%s      |", space_get_gdesc(space_act, 4));
      }
      else {
        sprintf(str, "  |%s      |", space_get_gdesc(space_act, 4));
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID)
        sprintf(str, "  | %c %c %c %c       |                 |          ", obj1, obj2, obj3, obj4);
      else
        sprintf(str, "  | %c %c %c %c       |", obj1, obj2, obj3, obj4);
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID)
        sprintf(str, "  +---------------+                 +-----------");
      else
        sprintf(str, "  +---------------+");
      screen_area_puts(ge->map, str);
    }

    if (space_contain_object(game_get_space(game, id_next), 1))
      obj1 = '*';
    else
      obj1 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 2))
      obj2 = '.';
    else
      obj2 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 3))
      obj3 = '+';
    else
      obj3 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 4))
      obj4 = '&';
    else
      obj4 = ' ';

    if (id_next != NO_ID)
    {
      sprintf(str, "        v");
      screen_area_puts(ge->map, str);
      sprintf(str, "  +---------------+");
      screen_area_puts(ge->map, str);
      if((int)id_next>=100)
        sprintf(str, "  |            %2d|", (int)id_next);
      else
        sprintf(str, "  |             %2d|", (int)id_next);
      screen_area_puts(ge->map, str);
      sprintf(str, "  | %c %c %c %c       |", obj1, obj2, obj3, obj4);
      screen_area_puts(ge->map, str);
    }
  }

  /* Paint in the description area */
  screen_area_clear(ge->descript);
  // if ((obj_loc = object_get_id(game->obj)) != NO_ID)
  // {
  //   sprintf(str, "  Object location:%d", (int)obj_loc);
  //   screen_area_puts(ge->descript, str);
  // }
  player_loc = player_get_location(game->pl);
  while (!space_contain_object(game->spaces[i], obj1_id) && (player_get_object(game->pl) == NO_ID || player_get_object(game->pl) == obj2_id || player_get_object(game->pl) == obj3_id || player_get_object(game->pl) == obj4_id) && i <= 127) 
    i++;
  obj1_loc = space_get_id(game->spaces[i]);
  i = 0;
  while (!space_contain_object(game->spaces[i], obj2_id) && (player_get_object(game->pl) == NO_ID || player_get_object(game->pl) == obj1_id || player_get_object(game->pl) == obj3_id || player_get_object(game->pl) == obj4_id) && i <= 127)
    i++;
  obj2_loc = space_get_id(game->spaces[i]);
  i = 0;
  while (!space_contain_object(game->spaces[i], obj3_id) && (player_get_object(game->pl) == NO_ID || player_get_object(game->pl) == obj2_id || player_get_object(game->pl) == obj1_id || player_get_object(game->pl) == obj4_id) && i <= 127)
    i++;
  obj3_loc = space_get_id(game->spaces[i]);
  i = 0;
  while (!space_contain_object(game->spaces[i], obj4_id) && (player_get_object(game->pl) == NO_ID || player_get_object(game->pl) == obj2_id || player_get_object(game->pl) == obj3_id || player_get_object(game->pl) == obj1_id) && i <= 127)
    i++;
  obj4_loc = space_get_id(game->spaces[i]);
  i = 0;

  if (player_get_object(game->pl) != obj1_id)
    sprintf(str, " Grano(1) location:%d", (int)(obj1_loc));
  else
    sprintf(str, " Grano(1) location: (taked) %d", (int)(player_loc));
  screen_area_puts(ge->descript, str);
  if (player_get_object(game->pl) != obj2_id)
    sprintf(str, " Pipa(2) location:%d", (int)(obj2_loc));
  else
    sprintf(str, " Pipa(2) location: (taked) %d", (int)(player_loc));
  screen_area_puts(ge->descript, str);
  if (player_get_object(game->pl) != obj3_id)
    sprintf(str, " Miga(3) location:%d", (int)(obj3_loc));
  else
    sprintf(str, " Miga(3) location: (taked) %d", (int)(player_loc));
  screen_area_puts(ge->descript, str);
  if (player_get_object(game->pl) != obj4_id)
    sprintf(str, " Nuez(4) location:%d", (int)(obj4_loc));
  else
    sprintf(str, " Nuez(4) location: (taked) %d", (int)(player_loc));
  screen_area_puts(ge->descript, str);
  if ((player_loc = player_get_location(game->pl)) != NO_ID)
  {
    sprintf(str, " Player location:%d", (int)player_loc);
    screen_area_puts(ge->descript, str);
  }
  sprintf(str, " Player health: %d", player_get_health(game->pl));
  screen_area_puts(ge->descript, str);
  if ((enemy_loc = enemy_get_location(game->enemy)) != NO_ID)
  {
    sprintf(str, " Enemy location:%d", (int)enemy_loc);
    screen_area_puts(ge->descript, str);
  }
  sprintf(str, " Enemy health: %d", enemy_get_health(game->enemy));
  screen_area_puts(ge->descript, str);

  /* Paint in the banner area */
  screen_area_puts(ge->banner, "    The anthill game ");

  /* Paint in the help area */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, "     next or n, back or b, exit or e, take or t, drop or d, right or r, left or l, attack or a");
  screen_area_puts(ge->help, str);

  /* Paint in the feedback area */
  last_cmd = game_get_last_command(game);
  if(game_get_state(game) == ERROR){
    sprintf(str, " %s (%s): ERROR", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
    screen_area_puts(ge->feedback, str);
  }
  else{
    sprintf(str, " %s (%s): OK", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
    screen_area_puts(ge->feedback, str);
  }

  /* Dump to the terminal */
  screen_paint();
  printf("prompt:> ");
}
