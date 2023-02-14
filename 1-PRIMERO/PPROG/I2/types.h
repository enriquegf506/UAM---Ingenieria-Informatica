/**
 * @brief It defines common types for the whole project
 *
 * @file types.h
 * @author Profesores PPROG
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */

#ifndef TYPES_H
#define TYPES_H

#define WORD_SIZE 1000  /*!< Size of any array*/
#define NO_ID -1  /*!< Macro represents objects, player, space... that don't have identification*/

typedef long Id;  /*!< Identification of objects, spaces...*/ 

typedef enum
{
  FALSE,  /*!< Whether what we want is incorrect*/ 
  TRUE    /*!< Whether what we want is correct*/ 
} BOOL;

typedef enum
{
  ERROR,  /*!< When the function can't be executed*/ 
  OK      /*!< When the function can be executed*/
} STATUS;

typedef enum
{
  N,    /*!< North direction*/
  S,    /*!< South direction*/
  E,    /*!< East direction*/
  W     /*!< West direction*/
} DIRECTION;

#endif
