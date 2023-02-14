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

#define ROWS 30     /*!< Number of the game has*/
#define COLUMNS 84  /*!< Number of columns the game has*/

/**
 * @brief Graphic_engine
 *
 * This struct stores all the information of the graphic engine.
 */
struct _Graphic_engine
{
  Area *map, *descript, *banner, *help, *feedback;
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
  ge->descript = screen_area_init(50, 1, 33, 17);
  ge->banner = screen_area_init(28, 19, 24, 1);
  ge->help = screen_area_init(1, 20, 79, 3);
  ge->feedback = screen_area_init(1, 24, 79, 4);

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
  Id id_act = NO_ID, id_back = NO_ID, id_next = NO_ID, player_loc = NO_ID;
  Id id_right = NO_ID;
  Id enemy_loc = NO_ID;
  Space *space_act = NULL;
  char obj1 = '\0', obj2 = '\0', obj3 = '\0', obj4 = '\0', obj5 = '\0',obj6 = '\0', obj7 = '\0', obj8 = '\0', obj9 = '\0', obj10 = '\0',obj11 = '\0', obj12 = '\0', obj13 = '\0', obj14 = '\0', obj15 = '\0',obj16 = '\0', obj17 = '\0', obj18 = '\0', obj19 = '\0', obj20 = '\0';
  char str[255];
  T_Command last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];
  int j=0, i=0, ilu=0;

  /* Paint the in the map area */
  screen_area_clear(ge->map);
  if ((id_act = player_get_location(game_get_player(game))) != NO_ID)
  {
    space_act = game_get_space(game, player_get_location(game_get_player(game)));
    id_back = game_get_connection(game, id_act, N);
    id_next = game_get_connection(game, id_act, S);
    id_right = game_get_connection(game, id_act, E);


    if (space_contain_object(game_get_space(game, id_back), 21))
      obj1 = '*';
    else
      obj1 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 22))
      obj2 = '.';
    else
      obj2 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 23))
      obj3 = '+';
    else
      obj3 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 24))
      obj4 = '&';
    else
      obj4 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 25))
      obj5 = 'B';
    else
      obj5 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 26))
      obj6 = '0';
    else
      obj6 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 27))
      obj7 = 'm';
    else
      obj7 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 28))
      obj8 = 'i';
    else
      obj8 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 29))
      obj9 = 'T';
    else
      obj9 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 30))
      obj10 = '^';
    else
      obj10 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 31))
      obj11 = '[';
    else
      obj11 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 32))
      obj12 = '9';
    else
      obj12 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 33))
      obj13 = 'Q';
    else
      obj13 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 34))
      obj14 = 't';
    else
      obj14 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 35))
      obj15 = 'n';
    else
      obj15 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 36))
      obj16 = '_';
    else
      obj16 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 37))
      obj17 = '#';
    else
      obj17 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 38))
      obj18 = 'o';
    else
      obj18 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 39))
      obj19 = 'H';
    else
      obj19 = ' ';
    if (space_contain_object(game_get_space(game, id_back), 40))
      obj20 = 'C';
    else
      obj20 = ' ';

    if (id_back != NO_ID)
    {
      sprintf(str, "  |            %2d|", (int)id_back);
      screen_area_puts(ge->map, str);
      sprintf(str, "  | %c%c%c%c  %c %c %c %c |", obj1, obj2, obj3, obj4, obj8, obj9, obj10, obj18);
      screen_area_puts(ge->map, str);
      sprintf(str, "  +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "        ^");
      screen_area_puts(ge->map, str);
    }

    if (space_contain_object(game_get_space(game, id_act), 21))
      obj1 = '*';
    else
      obj1 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 22))
      obj2 = '.';
    else
      obj2 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 23))
      obj3 = '+';
    else
      obj3 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 24))
      obj4 = '&';
    else
      obj4 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 25))
      obj5 = 'B';
    else
      obj5 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 26))
      obj6 = '0';
    else
      obj6 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 27))
      obj7 = 'm';
    else
      obj7 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 28))
      obj8 = 'i';
    else
      obj8 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 29))
      obj9 = 'T';
    else
      obj9 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 30))
      obj10 = '^';
    else
      obj10 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 31))
      obj11 = '[';
    else
      obj11 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 32))
      obj12 = '9';
    else
      obj12 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 33))
      obj13 = 'Q';
    else
      obj13 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 34))
      obj14 = 't';
    else
      obj14 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 35))
      obj15 = 'n';
    else
      obj15 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 36))
      obj16 = '_';
    else
      obj16 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 37))
      obj17 = '#';
    else
      obj17 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 38))
      obj18 = 'o';
    else
      obj18 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 39))
      obj19 = 'H';
    else
      obj19 = ' ';
    if (space_contain_object(game_get_space(game, id_act), 40))
      obj20 = 'C';
    else
      obj20 = ' ';

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
        sprintf(str, "  | m0^      %c  %2d|", obj7, (int)id_act);
      screen_area_puts(ge->map, str);
      if (enemy_get_location(game_get_enemy(game)) == id_act)
      {
        if(enemy_get_health(game_get_enemy(game))!=0)
        {
          if (id_right == NO_ID) {
            sprintf(str, "  |  /\\00/\\   %c   |", obj15);
          }
        }
        else
        {
          sprintf(str, "  |           %c   |", obj15);
        }
      }
      else if (id_right != NO_ID) {
        sprintf(str, "  |           %c%c  |                 |           ", obj5, obj20);
      }
      else {
        sprintf(str, "  |           %c%c  |", obj5, obj20);
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s   %c%c |                 |           ", space_get_gdesc(space_act, 0), obj12, obj11);
      }
      else {
        sprintf(str, "  |%s   %c%c |", space_get_gdesc(space_act, 0), obj12, obj11);
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s %c    |                 |           ", space_get_gdesc(space_act, 1), obj17);
      }
      else {
        sprintf(str, "  |%s %c    |", space_get_gdesc(space_act, 1), obj17);
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s   %c%c |                 |           ", space_get_gdesc(space_act, 2), obj14, obj13);

      }
      else {
        sprintf(str, "  |%s   %c%c |", space_get_gdesc(space_act, 2), obj14, obj13);
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s     %c|                 |           ", space_get_gdesc(space_act, 3), obj6);
      }
      else {
        sprintf(str, "  |%s     %c|", space_get_gdesc(space_act, 3), obj6);
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID) {
        sprintf(str, "  |%s    %c%c|                 |           ", space_get_gdesc(space_act, 4), obj16, obj19);
      }
      else {
        sprintf(str, "  |%s    %c%c|", space_get_gdesc(space_act, 4), obj16, obj19);
      }
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID)
        sprintf(str, "  | %c%c%c%c  %c %c %c %c |                 |          ", obj1, obj2, obj3, obj4, obj8, obj9, obj10, obj18);
      else
        sprintf(str, "  | %c%c%c%c  %c %c %c %c |", obj1, obj2, obj3, obj4, obj8, obj9, obj10, obj18);
      screen_area_puts(ge->map, str);
      if (id_right != NO_ID)
        sprintf(str, "  +---------------+                 +-----------");
      else
        sprintf(str, "  +---------------+");
      screen_area_puts(ge->map, str);
    }

    if (space_contain_object(game_get_space(game, id_next), 21))
      obj1 = '*';
    else
      obj1 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 22))
      obj2 = '.';
    else
      obj2 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 23))
      obj3 = '+';
    else
      obj3 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 24))
      obj4 = '&';
    else
      obj4 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 28))
      obj8 = 'i';
    else
      obj8 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 29))
      obj9 = 'T';
    else
      obj9 = ' ';
    if (space_contain_object(game_get_space(game, id_next), 30))
      obj10 = '^';

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
      sprintf(str, "  | %c%c%c%c  %c %c %c %c |", obj1, obj2, obj3, obj4, obj8, obj9, obj10, obj18);
      screen_area_puts(ge->map, str);
    }
  }

  /* Paint in the description area */
  screen_area_clear(ge->descript);
  /*if ((obj_loc = object_get_id(game->obj)) != NO_ID)
  {
    sprintf(str, "  Object location:%d", (int)obj_loc);
    screen_area_puts(ge->descript, str);
  }*/

  if (id_act == 129)
  {
    sprintf(str, " YOU HAVE FALLEN INTO A TRAP, YOU LOOSE");
    screen_area_puts(ge->descript, str);
  }
  player_loc = player_get_location(game_get_player(game));

  sprintf(str, " OBJECTS IN THE SPACE:");
  screen_area_puts(ge->descript, str);

  for (i=1; i<=MAX_OBJECTS; i++){
    if(player_contain_object(game_get_player(game), 20+i)==TRUE){
      if(object_get_illuminate(game_get_obj(game, 20+i))== 1 && object_get_turnedon(game_get_obj(game, 20+i))== 1){
        ilu=1;
        break;
      }
    }
  }
  
  if(space_get_illumination(space_act) == 1)
  {
    ilu = 1;
  }

  if(ilu==1){
    for(j=1; j<=MAX_OBJECTS; j++){
      if(space_contain_object(game_get_space(game, id_act), 20+j)){
        sprintf(str, " %s(%d) ", object_get_name(game_get_obj(game, 20+j)), 20+j);
        screen_area_puts(ge->descript, str);
      }
    }
  }
  else{
    sprintf(str, " Space not illuminated, cant see objects");
    screen_area_puts(ge->descript, str);
  } 
  
  sprintf(str, " OBJECTS IN THE INVENTORY:");
  screen_area_puts(ge->descript, str);
  for(j=1; j<=MAX_OBJECTS; j++){
    if(player_contain_object(game_get_player(game), 20+j)){
      sprintf(str, " %s(%d) ", object_get_name(game_get_obj(game, 20+j)), 20+j);
      screen_area_puts(ge->descript, str);
    }
  }
  sprintf(str, " CLOSED LINKS:");
  if(link_get_state(game_get_link(game, id_act, S)) == CLOSE){
    sprintf(str, " CLOSED LINKS: %s ", link_get_name(game_get_link(game, id_act, S)));
  }
  screen_area_puts(ge->descript, str);

  sprintf(str, " UP LINK: no");
  if(link_get_state(game_get_link(game, id_act, U)) == OPENED){
    sprintf(str, " UP LINK: yes ");
  }
  screen_area_puts(ge->descript, str);

  sprintf(str, " DOWN LINK: no");
  if(link_get_state(game_get_link(game, id_act, D)) == OPENED){
    sprintf(str, " DOWN LINK: yes ");
  }
  screen_area_puts(ge->descript, str);


  if ((player_loc = player_get_location(game_get_player(game))) != NO_ID)
  {
    sprintf(str, " PLAYER LOCATION:%d", (int)player_loc);
    screen_area_puts(ge->descript, str);
  }
  sprintf(str, " PLAYER HEALTH: %d", player_get_health(game_get_player(game)));
  screen_area_puts(ge->descript, str);
  if ((enemy_loc = enemy_get_location(game_get_enemy(game))) != NO_ID)
  {
    sprintf(str, " ENEMY LOCATION:%d", (int)enemy_loc);
    screen_area_puts(ge->descript, str);
  }
  sprintf(str, " ENEMY HEALTH: %d", enemy_get_health(game_get_enemy(game)));
  screen_area_puts(ge->descript, str);

  sprintf(str, " SPACE DESCRIPTION: %s", space_get_desc((game_get_space(game, player_get_location(game_get_player(game))))));
  screen_area_puts(ge->descript, str);
  
  sprintf(str, " DESCRIPTION:");
  if(game_get_last_command(game)== INSPECT){
    sprintf(str, " DESCRIPTION: %s", game_get_description(game));
  }
  screen_area_puts(ge->descript, str);
  
  /* Paint in the banner area */
  screen_area_puts(ge->banner, "    The anthill game ");

  /* Paint in the help area */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, "     exit or e, take or t, drop or d, attack or a, move or m, inspect or i, Ton or Turnon, Toff or Turnoff, o or open");
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
