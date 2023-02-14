/**
 * @brief It defines the game loop
 *
 * @file game_loop.c
 * @author Marcos Alonso
 * @version 2.0
 * @date 30-11-2020
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphic_engine.h"
#include "game.h"
#include "command.h"

struct _Game
{
  Player *pl;                   /*!< pl pointer to player, the player of the game*/
  Object *obj[MAX_OBJECTS];     /*!< Array of pointers to the object */
  Space *spaces[MAX_SPACES];    /*!< Array of pointers to the space */
  T_Command last_cmd;           /*!< Last comand used */
  Enemy *enemy;                 /*!< enemy pointer to enemy, the enemy of the game */
  Link *link[MAX_LINKS];        /*!< Array of pointers to the link */
  char description[WORD_SIZE];  /*!< char pointer, the description of the game*/
  STATUS state;                 /*!< The state of the command*/
};

/**
   * @brief inicilizes the game and verify it doesn't have errors
   * @author Marcos Alonso
   *
   * 
   * @param *game game variable used to create the game
   * @param **gengine game engine
	 * @param *file_name game of the file by which we create the game
	 * @return 1 if error, 0 if there are not errors
   */
int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name);
/**
   * @brief execute the game
   * @author Marcos Alonso 
   * 
   * @param game variable that contains the game
   * @param *gengine motor used for the game graphics
   */
void game_loop_run(Game **game, Graphic_engine *gengine, int log_flag, FILE *logfile);
/**
   * @brief destrois the game and the graphics created
   * @author Marcos Alonso
   *
   * 
   * @param *gengine the engine of the graphics we delete
   * @param game the game we delete
   */
void game_loop_cleanup(Game *game, Graphic_engine *gengine, FILE *logfile);

/** Main function*/
int main(int argc, char *argv[])
{
  Game *game = NULL;
  FILE *logfile;
  Graphic_engine *gengine;
  int log_flag = 0;

  if (argc < 2)
  {
    fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]);
    return 1;
  }
  
  if (argc > 2) {
    if (strcmp(argv[2], "-l")!=0 || !argv[3]) {
      fprintf(stderr, "Use: %s <game_data_file> -l <log_file>\n", argv[0]);
      return -1;
    }

    logfile = fopen(argv[3], "w");
    if (!logfile){
      return -1;
    }

    log_flag=1;
  }
  
  if (!game_loop_init(&game, &gengine, argv[1]))
  {
    game_loop_run(&game, gengine, log_flag, logfile);
    game_loop_cleanup(game, gengine, logfile);
  }

  if (log_flag ==1)
    fclose(logfile);

  return 0;
}

/**
  inicilizes the game and verify it doesn't have errors
*/
int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name)
{
  if ((*game = game_create_from_file(file_name)) == NULL)
  {
    fprintf(stderr, "Error while initializing game.\n");
    return 1;
  }

  if ((*gengine = graphic_engine_create()) == NULL)
  {
    fprintf(stderr, "Error while initializing graphic engine.\n");
    game_destroy(*game);
    return 1;
  }

  return 0;
}

/**
  execute the game
*/
void game_loop_run(Game **game, Graphic_engine *gengine, int log_flag, FILE *logfile)
{
  T_Command command = NO_CMD;
  int i;
  STATUS st;
  char cmd_to_str[WORD_SIZE];
  if (!*game)
    return ;

  while ((command != EXIT) && !game_is_over(*game))
  {
    graphic_engine_paint_game(gengine, *game);
    command = command_get_user_input();
    st = game_update(*game, command);
    game_set_state(*game, st);
    if (log_flag == 1)
    {
      if (command == 1)
        strcpy(cmd_to_str, "EXIT");
      else if (command == 2)
        strcpy(cmd_to_str, "TAKE");
      else if (command == 3)
        strcpy(cmd_to_str, "DROP");
      else if (command == 4)
        strcpy(cmd_to_str, "ATTACK");
      else if (command == 5)
        strcpy(cmd_to_str, "MOVE");
      else if (command == 6)
        strcpy(cmd_to_str, "INSPECT");
      else if (command == 7)
        strcpy(cmd_to_str, "TURN ON");
      else if (command == 8)
        strcpy(cmd_to_str, "TURN OFF");
      else if (command == 9)
        strcpy(cmd_to_str, "OPEN");
      else
        strcpy(cmd_to_str, "UNKNOWN");
      for (i = -1; i < N_CMD; i++) {
        if (i == command) {
          if (st == OK)
          {
            fprintf(logfile, "%s: OK\n", cmd_to_str);
            break;
          }
          else
          {
            fprintf(logfile, "%s: ERROR\n", cmd_to_str);
            break;
          }
        }
      }
    }
  }
  return;
}

/**
  destroys the game and the graphics created
*/
void game_loop_cleanup(Game *game, Graphic_engine *gengine, FILE *logfile)
{
  game_destroy(game);
  graphic_engine_destroy(gengine);
}
