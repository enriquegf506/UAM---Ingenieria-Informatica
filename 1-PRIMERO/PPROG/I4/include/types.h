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

#define WORD_SIZE 1000  /*!< The maximum size of a char string*/
#define NO_ID -1  /*!< Don't have identification*/

typedef long Id;  /*!< Identification of objects, spaces...*/ 

typedef enum
{
  FALSE,  /*!< Whether what we want is incorrect*/ 
  TRUE    /*!< Whether what we want is correct*/
} BOOL;   /*!< typedef enum*/

typedef enum
{
  ERROR,  /*!< When the function can't be executed*/ 
  OK      /*!< When the function can be executed*/
} STATUS; /*!< typedef enum*/

typedef enum
{
  N = 1,  /*!< North direction*/
  S,      /*!< South direction*/
  E,      /*!< East direction*/
  W,      /*!< West direction*/
  U,      /*!< Up direction*/
  D       /*!< Down direction*/
} DIRECTION;/*!< typedef enum*/

typedef enum
{
  OPENED = 1, /*!< When a link is opened and you can reach the next space*/
  CLOSE       /*!< When a link is closed and you can't reach the next space*/
} LINKSTATUS; /*!< typedef enum*/

#endif
