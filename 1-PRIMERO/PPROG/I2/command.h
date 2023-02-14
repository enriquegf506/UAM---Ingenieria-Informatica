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

#define N_CMDT 2  /*!< No command*/
#define N_CMD 10  /*!< Number of comands*/

typedef enum enum_CmdType {
  CMDS,
  CMDL} T_CmdType;

typedef enum enum_Command {
  NO_CMD = -1,  /*!< No command*/
  UNKNOWN,      /*!< No valid command*/
  EXIT,         /*!< command that stop the simulation*/
  NEXT,         /*!< Command that moves the ant to the south*/
  BACK,         /*!< Command that moves the ant to the north*/
  LEFT,         /*!< Command that moves the ant to the west*/
  RIGHT,        /*!< Command that moves the ant to the east*/
  TAKE,         /*!< Command that makes the ant get an object*/
  DROP,         /*!< Command that makes the ant drop an object*/
  ATTACK        /*!< Command that that simulates a fight between player and enemy*/
  } T_Command;

/**
 * @brief obtains the user instruction
 * @author Marcos Alonso
 * @return the command introduced
*/
T_Command command_get_user_input();

#endif
