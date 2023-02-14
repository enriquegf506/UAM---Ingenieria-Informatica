/**
 * @brief It implements the command interpreter interface
 *
 * @file command.h
 * @author Marcos Alonso
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */

#ifndef COMMAND_H
#define COMMAND_H

#define N_CMDT 2 /*!< Macro*/
#define N_CMD 11 /*!< Macro*/

typedef enum enum_CmdType {
  CMDS,
  CMDL} T_CmdType; /*!< typedef enum*/

typedef enum enum_Command {
  NO_CMD = -1, /*!< NO command*/
  UNKNOWN, /*!< Unknown command*/
  EXIT, /*!< Exit command*/
  TAKE, /*!< Command to take an object*/
  DROP, /*!< Command to move drop an object*/
  ATTACK, /*!< Command to attack*/
  MOVE, /*!< Command to move */
  INSPECT, /*!< Command to inspect*/
  TURNON, /*!< Command ti turn on*/
  TURNOFF,  /*!<Command to turn off*/
  OPEN /*!<Command to open a link*/
  } T_Command; /*!< typedef enum*/

/**
 * @brief obtains the user instruction
 * @author Marcos Alonso
 * @return the command introduced
*/
T_Command command_get_user_input();

#endif
