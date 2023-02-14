/** 
 * @brief It implements the link module
 * 
 * @file space.c
 * @author Iñigo Alvarez
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"


/**
 * @brief Link
 *
 * This struct stores all the information of the link.
 */
struct _Link {
  Id id;                    /*!< Id number of the link, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the link */
  Id origin;                 /*!< Id of the origin of the link */
  Id dest;                 /*!< Id of the dest of the link*/
  LINKSTATUS state;                  /*!< Determines if the link is open or close */
  DIRECTION dir;            /*!< Determines the link direction */
};

/** link_create allocates memory for a new link
  *  and initializes it
  */
Link* link_create(Id id) {
  Link *newLink = NULL;

  /* Error control */
  if (id == NO_ID)
    return NULL;

  newLink = (Link *) malloc(sizeof (Link));
  if (newLink == NULL) {
    return NULL;
  }

  /* Initialization of an empty space*/
  newLink->id = id;
  newLink->name[0] = '\0';
  newLink->origin = NO_ID;
  newLink->dest = NO_ID;
  newLink->state = -1;
  newLink->dir = -1;
  
  return newLink;
}

/** link_destroy frees the previous memory allocation 
  *  for a link
  */
STATUS link_destroy(Link* link) {
  if (!link) {
    return ERROR;
  }

  free(link);
  link = NULL;
  return OK;
}

/** It gets the id of a link
  */
Id link_get_id(Link* link) {
  if (!link) {
    return NO_ID;
  }
  return link->id;
}

/** It sets the name of a lnk
  */
STATUS link_set_name(Link* link, char* name) {
  if (!link || !name) {
    return ERROR;
  }

  if (!strcpy(link->name, name)) {
    return ERROR;
  }
  return OK;
}

/** It gets the name of a link
  */
const char * link_get_name(Link* link) {
  if (!link) {
    return NULL;
  }
  return link->name;
}

/** It sets the origin of a lnk
  */
STATUS link_set_origin(Link* link, Id id)
{
    if (!link)
        return ERROR;
    link->origin = id;
    return OK;
}

/** It gets the origin of a link
  */
Id link_get_origin(Link* link)
{
    if (!link)
        return NO_ID;
    return link->origin;
}

/** It sets the destination of a lnk
  */
STATUS link_set_dest(Link* link, Id id)
{
    if (!link)
        return ERROR;
    link->dest = id;
    return OK;
}

/** It gets the destination of a link
  */
Id link_get_dest(Link* link)
{
    if (!link)
        return NO_ID;
    return link->dest;
}

/** It sets the state of a lnk
  */
STATUS link_set_state(Link* link, LINKSTATUS state)
{
    if (!link|| !state)
        return ERROR;
    link->state = state;
    return OK;
}

/** It gets the state of a link
  */
LINKSTATUS link_get_state(Link* link)
{
    if (!link)
        return -1;
    return link->state;
}

/** It sets the direction of a lnk
  */
STATUS link_set_direction(Link* link, DIRECTION direction)
{
    if (!link || !direction)
        return ERROR;
    link->dir = direction;
    return OK;
}

/** It gets the direction of a link
  */
DIRECTION link_get_direction(Link* link)
{
    if (!link)
        return -1;
    return link->dir;    
}

/** It prints the link information
  */
STATUS link_print(Link* link) {
  Id idaux = NO_ID;

  /* Error Control */
  if (!link) {
    return ERROR;
  }

  /* 1. Print the id and the name of the link */
  fprintf(stdout, "--> Link (Id: %ld; Name: %s)\n", link->id, link->name);
 
  /* 2. Print the origin, destination, direction and status if there is a link */
  idaux = link_get_origin(link);
  if (idaux != NO_ID) {
    fprintf(stdout, "---> Origin of the link: %ld\n", link->origin);
  } else {
    fprintf(stdout, "---> No origin for the link.\n");
  }
  idaux = link_get_dest(link);
  if (idaux != NO_ID) {
    fprintf(stdout, "---> Destination of the link: %ld\n", link->dest);
  } else {
    fprintf(stdout, "---> No destination for the link.\n");
  }
  
  if (link_get_direction(link))
  {
      fprintf(stdout, "---> Direction of the link: ");
      if (link->dir == N)
        fprintf(stdout, "N\n");
      else if (link->dir == S)
        fprintf(stdout, "S\n");
      else if (link->dir == E)
        fprintf(stdout, "E\n");
      else
        fprintf(stdout, "W\n");
  }
  else
    fprintf(stdout, "---> No direction for the link.\n");

  if (link_get_state(link))
  {
      fprintf(stdout, "---> State of the link: ");
      if (link->dir == 0)
        fprintf(stdout, "OPEN\n");
      else
        fprintf(stdout, "CLOSE\n");
  }
  else
    fprintf(stdout, "---> No state for the link.\n");
  
  return OK;
}
