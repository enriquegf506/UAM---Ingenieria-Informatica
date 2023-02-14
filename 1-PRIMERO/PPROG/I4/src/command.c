/** 
 * @brief It implements the command interpreter
 * 
 * @file command.c
 * @author Marcos Alonso
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <strings.h>
#include "command.h"

#define CMD_LENGHT 30 /*!< Macro for length*/


char *cmd_to_str[N_CMD] /*!< bidimensional variable that cotains the commands*/
[N_CMDT] = {{"", "No command"}, /*!< NO command*/
			{"", "Unknown"},          /*!< Unknown command*/
			{"e", "Exit"},            /*!< Exit command*/
      {"t", "Take"},            /*!< Command to take an object*/
			{"d", "Drop"},            /*!< Command to move drop an object*/
			{"a", "Attack"},          /*!< Command to attack*/
			{"m", "Move"},            /*!< Command to move */
      {"i", "Inspect"},         /*!< Command to inspect*/
      {"Ton", "Turnon"},        /*!< Command ti turn on*/
      {"Toff", "Turnoff"},      /*!<Command to turn off*/
      {"o", "Open"}             /*!<Command to open a link*/
			};

/* obtains de user instruction*/

T_Command command_get_user_input()
{
  T_Command cmd = NO_CMD;
  char input[CMD_LENGHT] = "";
  int i = UNKNOWN - NO_CMD + 1;
  
  if (scanf("%s", input) > 0)
  {
    cmd = UNKNOWN;
    while (cmd == UNKNOWN && i < N_CMD)
    {
      if (!strcasecmp(input, cmd_to_str[i][CMDS]) || !strcasecmp(input, cmd_to_str[i][CMDL]))
      {
        cmd = i + NO_CMD;
      }
      else
      {
        i++;
      }
    }
  }
  
  return cmd;
}
