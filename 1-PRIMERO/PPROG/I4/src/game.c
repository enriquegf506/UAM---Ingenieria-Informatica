/** 
 * @brief It implements the game interface and all the associated calls
 * for each command
 * 
 * @file game.c
 * @author Iñigo Alvarez
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

/**
 * @brief Game
 *
 * This struct stores all the information of the game.
 */
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

typedef STATUS (*callback_fn)(Game* game);
/* Private functions */

/**
 * @brief It is executed if the command is unknown
 * @param game pointer to game
 */
STATUS game_command_unknown(Game *game);
/**
 * @brief It is executed if the command is exit
 * @param game pointer to game
 */
STATUS game_command_exit(Game *game);
/**
 * @brief It is executed if the command is next
 * @param game pointer to game
 */
STATUS game_command_next(Game *game);
/**
 * @brief It is executed if the command is back
 * @param game pointer to game
 */
STATUS game_command_back(Game *game);
/**
 * @brief It is executed if the command is left
 * @param game pointer to game
 */
STATUS game_command_left(Game *game);
/**
 * @brief It is executed if the command is right
 * @param game pointer to game
 */
STATUS game_command_right(Game *game);
/**
 * @brief It is executed if the command is take
 * @param game pointer to game
 */
STATUS game_command_take(Game *game);
/**
 * @brief It is executed if the command is drop
 * @param game pointer to game
 */
STATUS game_command_drop(Game *game);
/**
 * @brief It is executed if the command is attack
 * @param game pointer to game
 */
STATUS game_command_attack(Game *game);
/**
 * @brief It is executed if the command is move
 * @param game pointer to game
 */
STATUS game_command_move(Game *game);
/**
 * @brief It is executed if the command is inpect
 * @param game pointer to game
 */
STATUS game_command_inspect(Game *game);
/**
 * @brief It is executed if the command is turn on
 * @param game pointer to game
 */
STATUS game_command_turnon(Game *game);
/**
 * @brief It is executed if the command is turn on
 * @param game pointer to game
 */
STATUS game_command_turnoff(Game *game);

/**
 * @brief It is executed if the command is open
 * @param game pointer to game
 */
STATUS game_command_open(Game *game);

/**
 * @brief It is executed if the command is down
 * @param game pointer to game
 */
STATUS game_command_down(Game *game);

/**
 * @brief It is executed if the command is up
 * @param game pointer to game
 */
STATUS game_command_up(Game *game);

static callback_fn game_callback_fn_list[N_CMD]={
  game_command_unknown,      /*!< Call if the command is unknown(unknown) */
  game_command_exit,         /*!< Call if the command is to exit (exit) */
  game_command_take,         /*!< Call if the command is to take an object (take) */
  game_command_drop,         /*!< Call if the command is to drop an object (drop) */
  game_command_attack,         /*!< Call if the command is to move */
  game_command_move,         /*!< Call if the command is to move */
  game_command_inspect,      /*!< Call if the command is to inspect an object or space (inspect) */
  game_command_turnon,       /*!< Call if the command is to turn on an object (turnon) */
  game_command_turnoff,      /*!< Call if the command is to turn off an object (turnoff) */
  game_command_open        /*!< Call if the command is to open a link (open) */
};

/**
 * @brief It transforms capital letters into small letters
 * @param c 
 */
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c = c + 32;
	return (c);
}

/**
 * @brief It compares two strings, capital letter doesn't matter
 * @param s1 char
 * @param s2 char
 */
int		ft_strcasecmp(char const *s1, char const *s2)
{
	int		i;

	i = 0;
	while (s1[i] && s2[i] &&
			(unsigned char)ft_tolower(s1[i]) ==
			(unsigned char)ft_tolower(s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*Inicializes the game*/
Game *game_create()
{
  Game *game;
  int i;
  game = malloc(sizeof(Game));
  if (!game)
    return NULL;
  game->pl = NULL;
  game->enemy = NULL;
  for (i = 0; i < MAX_SPACES; i++)
  {
    game->spaces[i] = NULL;
  }
  for (i = 0; i < MAX_OBJECTS; i++)
  {
    game->obj[i] = NULL;
  }
  game->last_cmd = NO_CMD;
  for (i = 0; i < MAX_LINKS; i++)
  {
    game->link[i] = NULL;
  }
  game->description[0] = 0;
  game->state = -1;
  return game;
}

/**
   create the game form the file given
*/
Game *game_create_from_file(char *filename)
{
  Game *game = game_create();
  if(!game || !filename)
    return NULL;
 
  if (game_load_spaces(game, filename) == ERROR)
   return NULL;
  if (game_load_objects(game, filename) == ERROR)
    return NULL;
  if (game_load_players(game, filename) == ERROR)
    return ERROR;
  if (game_load_enemies(game, filename) == ERROR)
    return ERROR;
  if (game_load_links(game, filename) == ERROR)
    return NULL;

  return game;
}

/* Destroys the space of the game */
STATUS game_destroy(Game *game)
{
  int i = 0;
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    space_destroy(game->spaces[i]);
  }
  for (i = 0; i < MAX_OBJECTS && game->obj[i]; i++)
  {
    object_destroy(game->obj[i]);
  }
  for (i = 0; i < MAX_LINKS; i++)
  {
    link_destroy(game->link[i]);
  }
  enemy_destroy(game->enemy);
  player_destroy(game->pl);
  free(game);
  return OK;
}
   

/**
   Adds a player to the game
*/
STATUS game_add_player(Game *game, Player *player)
{
  if(!game || !player){
    return ERROR;
  }
  game->pl = player;

  return OK;
}

/**
   Adds an enemy to the game
*/
STATUS game_add_enemy(Game *game, Enemy *enemy)
{
  if(!game || !enemy){
    return ERROR;
  }
  game->enemy = enemy;

  return OK;
}

/*adds a space for the game*/
STATUS game_add_space(Game *game, Space *space)
{
  int i = 0;
  /*ERROR CONTROL*/
  if (space == NULL)
  {
    return ERROR;
  }

  while (i < MAX_SPACES && game->spaces[i] != NULL)
  {
    i++;
  }

  if (i >= MAX_SPACES)
  {
    return ERROR;
  }
  game->spaces[i] = space;

  return OK;
}

/**
   Adds an object to the game
*/
STATUS game_add_obj(Game *game, Object *obj)
{
  int i = 0;
  /*ERROR CONTROL*/
  if (obj == NULL)
  {
    return ERROR;
  }

  while (i < MAX_OBJECTS && game->obj[i] != NULL)
  {
    i++;
  }

  if (i >= MAX_OBJECTS)
  {
    return ERROR;
  }
  game->obj[i] = obj;

  return OK;
}

/**
   Gets an object of the game
*/
Object *game_get_obj(Game *game, Id id)
{
  int i;
  if (!game || id == NO_ID)
    return NULL;
  for (i = 0; i < MAX_OBJECTS && game->obj[i] != NULL; i++)
  {
    if (id == object_get_id(game->obj[i]))
      return game->obj[i];
  }
  return NULL;  
}


/**
   Adds a link to the game
*/
STATUS game_add_link(Game *game, Link *link)
{
  int i = 0;
  /*ERROR CONTROL*/
  if (link == NULL)
  {
    return ERROR;
  }

  while (i < MAX_LINKS && game->link[i] != NULL)
  {
    i++;
  }

  if (i >= MAX_LINKS)
  {
    return ERROR;
  }
  game->link[i] = link;

  return OK;
}

/*obtains the ID of the space in which it is located*/
Id game_get_space_id_at(Game *game, int position)
{
  if (position < 0 || position >= MAX_SPACES)
  {
    return NO_ID;
  }
  /*Obtiene la id de la posicion*/
  return space_get_id(game->spaces[position]);
}

/*obtains the game space that corresponds with the id*/
Space *game_get_space(Game *game, Id id)
{
  int i = 0;
  /*ERROR CONTROL*/
  if (id == NO_ID || !game)
  {
    return NULL;
  }
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    if (id == space_get_id(game->spaces[i]))
    {
      return game->spaces[i];
    }
  }

  return NULL;
}

/**
   gets a link of the game
*/
Link *game_get_link(Game *game, Id origin, DIRECTION direction)
{
  int i = 0;
  if(!game || !origin || !direction){
    return NULL;
  }
  for (i = 0; i < 22; i++)
  {
    if (origin == link_get_origin(game->link[i]))
    {
      if (direction == link_get_direction(game->link[i]))
      {
        return game->link[i];
      }
    }
  }
  return NULL;
}

/**
   gets the player of the game
*/
Player *game_get_player(Game *game)
{
   if (!game)
     return NULL;
  return game->pl;
}

/**
   gets the enemy of the game
*/
Enemy *game_get_enemy(Game *game)
{
  if (!game)
    return NULL;
  return game->enemy;
}

/**
   Gets the connection link status
*/
LINKSTATUS  game_get_connection_status(Game *game, Id id, DIRECTION direction)
{
  Link *link;
  if(!game || !id || !direction){
    return -1;
  }

  link = game_get_link(game, id, direction);
  return (link_get_state(link));
}

/**
   Gets the game connection links
*/
Id game_get_connection(Game *game, Id id, DIRECTION direction)
{
  Link *link;
  if(!game || !id || !direction){
    return NO_ID;
  }
  link = game_get_link(game, id, direction);

  return (link_get_dest(link));
}

/**
   Sets the game description
*/
STATUS game_set_description(Game *game, char *description)
{
  if (!game || !description)
    return ERROR;
  strcpy(game->description, description);
  return OK;
}

/**
   Gets the game description
*/
char *game_get_description(Game *game)
{
  if (!game)
    return NULL;
  return game->description;
}

STATUS game_set_state(Game *game, STATUS state){
  if(!game){
    return ERROR;
  }
  game->state = state;
  return OK;
}

STATUS game_get_state(Game *game){
  if(!game){
    return ERROR;
  }
  return game->state;
}

/*Actualices the game depending on the command introduced*/
STATUS game_update(Game *game, T_Command cmd)
{
  STATUS st = OK;
  game->last_cmd = cmd;
  st = (*game_callback_fn_list[cmd])(game);
  return st;
  /*switch (cmd)
  {
    case UNKNOWN:
      if (game_command_unknown(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;

    case EXIT:
      if (game_command_exit(game)==OK){
        game_set_state(game, OK);
      }
      else{ 
        game_set_state(game, ERROR);
      }
      break;

    case TAKE:
      if (game_command_take(game)==OK){
        game_set_state(game, OK);
      }
      else {
        game_set_state(game, ERROR);
      }
      break;

    case DROP:
      if (game_command_drop(game)==OK){
        game_set_state(game, OK);
      }
      else {
        game_set_state(game, ERROR);
      }
      break;
      
    case ATTACK:
      if (game_command_attack(game)==OK){
        game_set_state(game, OK);
      }
      else{
        game_set_state(game, ERROR);
      }
      break;
      
    case MOVE:
      if (game_command_move(game)==OK){
        game_set_state(game, OK);
      }
      else{
        game_set_state(game, ERROR);
      }
      break;

    case INSPECT:
      if (game_command_inspect(game)==OK){
        game_set_state(game, OK);
      }
      else{
        game_set_state(game, ERROR);
      }
      break;

    case TURNON:
      if (game_command_turnon(game)==OK){
        game_set_state(game, OK);
      }
      else{
        game_set_state(game, ERROR);
      }
      break;

    case TURNOFF:
      if (game_command_turnoff(game)==OK){
        game_set_state(game, OK);
      }
      else{
        game_set_state(game, ERROR);
      }
      break;

    case OPEN:
      if (game_command_open(game)==OK){
        game_set_state(game, OK);
      }
      else{
        game_set_state(game, ERROR);
      }
      break;

    default:
      break;
  }*/
}

/*Takes the last command*/
T_Command game_get_last_command(Game *game)
{
  return game->last_cmd;
}

/*Prints the game data*/
void game_print_data(Game *game)
{
  printf("\n\n-------------\n\n");

  printf("=> Spaces: \n");
  /*for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    space_print(game->spaces[i]);
  }
  Printea rhe location of the game and player
  printf("=> Object location: %d\n", (int)object_get_id(game->obj));*/
  printf("=> Player location: %d\n", (int)player_get_id(game->pl));
}

/*Determines that the game is finish*/
BOOL game_is_over(Game *game)
{
  if (player_get_health(game->pl) == 0){
    return TRUE;
  }
  return FALSE;
}

/**
   Calls implementation for each action 
*/

STATUS game_command_unknown(Game *game)
{
  return ERROR;
}
/**
   Calls implementation for each action 
*/
STATUS game_command_exit(Game *game)
{
  return OK;
}

/**
   Command to go to next position
*/
STATUS game_command_next(Game *game)
{
  /*Put all the variables to 0*/
  int i = 0;
  Id current_id = NO_ID;
  Id next_id = NO_ID;
  Id space_id = NO_ID;
  space_id = player_get_location(game->pl);  

  if (space_id == NO_ID)
  {
    return ERROR;
  }
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id && game_get_connection_status(game, current_id, S) == 1)
    {
      next_id = game_get_connection(game, current_id, S);
      if (next_id != NO_ID)
      {
        /*New player id*/
        player_set_location(game->pl, next_id);
        return OK;
      }
    }
  }
  return ERROR;
}

STATUS game_command_up(Game *game)
{
  /*Put all the variables to 0*/
  int i = 0;
  Id current_id = NO_ID;
  Id next_id = NO_ID;
  Id space_id = NO_ID;
  space_id = player_get_location(game->pl);  

  if (space_id == NO_ID)
  {
    return ERROR;
  }
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id && game_get_connection_status(game, current_id, U) == 1)
    {
      next_id = game_get_connection(game, current_id, U);
      if (next_id != NO_ID)
      {
        /*New player id*/
        player_set_location(game->pl, next_id);
        return OK;
      }
    }
  }
  return ERROR;
}

STATUS game_command_down(Game *game)
{
  /*Put all the variables to 0*/
  int i = 0;
  Id current_id = NO_ID;
  Id next_id = NO_ID;
  Id space_id = NO_ID;
  space_id = player_get_location(game->pl);  

  if (space_id == NO_ID)
  {
    return ERROR;
  }
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id && game_get_connection_status(game, current_id, D) == 1)
    {
      next_id = game_get_connection(game, current_id, D);
      if (next_id != NO_ID)
      {
        /*New player id*/
        player_set_location(game->pl, next_id);
        return OK;
      }
    }
  }
  return ERROR;
}

/**
   Command to go to back position
*/
STATUS game_command_back(Game *game)
{
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = player_get_location(game->pl);

  if (NO_ID == space_id)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id)
    {
      current_id = game_get_connection(game, current_id, N);
      if (current_id != NO_ID)
      {
        player_set_location(game->pl, current_id);
        return OK;
      }
    }
  }
  return ERROR;
}

/**
   Command to go to left position
*/
STATUS game_command_left(Game *game)
{
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = player_get_location(game->pl);

  if (NO_ID == space_id)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id)
    {
      current_id = game_get_connection(game, current_id, W);
      if (current_id != NO_ID)
      {
        player_set_location(game->pl, current_id);
        return OK;
      }
    }
  }
  return ERROR;
}

/**
   Command to go to right position
*/
STATUS game_command_right(Game *game)
{
  int i = 0;
  Id current_id = NO_ID;
  Id space_id = NO_ID;

  space_id = player_get_location(game->pl);

  if (NO_ID == space_id)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    current_id = space_get_id(game->spaces[i]);
    if (current_id == space_id)
    {
      current_id = game_get_connection(game, current_id, E);
      if (current_id != NO_ID)
      {
        player_set_location(game->pl, current_id);
        return OK;
      }
    }
  }
  return ERROR;
}

STATUS game_command_open(Game *game)
{
  int obj;
  char input1[WORD_SIZE], input2[WORD_SIZE], input3[WORD_SIZE];
  int i;
  
  scanf("%s", input1);
  scanf("%s", input2);
  scanf("%s", input3);

  if(ft_strcasecmp(input2, "with")!=0) {
    return ERROR;
  }

  for(i=0; game->obj[i]!=NULL; i++){
    if(ft_strcasecmp(object_get_name(game->obj[i]),input3)==0){
      break;
    }
  }
  obj=object_get_id(game->obj[i]);

  for(i=0; game->link[i]!=NULL; i++){
    if(ft_strcasecmp(link_get_name(game->link[i]),input1)==0){
      break;
    }
  }
  if(player_contain_object(game->pl, obj) && object_get_open(game_get_obj(game, obj)))
  {
    link_set_state(game->link[i], OPENED);
  }
  return OK;
}

/**Take the object*/
STATUS game_command_take(Game *game)
{
  Id space_id = player_get_location(game->pl);
  int obj;
  char input[WORD_SIZE];
  int i;

  scanf("%s", input);
  for(i=0; game->obj[i]!=NULL; i++){
    if(ft_strcasecmp(object_get_name(game->obj[i]),input)==0){
      break;
    }
  }
  obj=object_get_id(game->obj[i]);

  if(player_contain_object(game->pl, obj))
    return ERROR;
  if (object_get_dependency(game_get_obj(game, obj))!= NO_ID && player_contain_object(game_get_player(game), object_get_dependency(game_get_obj(game, obj)))== FALSE)
    return ERROR;

  if (space_contain_object(game_get_space(game, space_id), (long)obj) && object_get_movable(game_get_obj(game, obj))==TRUE)
  {
    player_add_object(game->pl, (long)obj);
    space_del_object(game_get_space(game, space_id), (long)obj);
    return OK;
  } else
  return ERROR;
  
}

/**Drop the object*/
STATUS game_command_drop(Game *game)
{
  int obj;
  char input[WORD_SIZE];
  int i;
  Id obj1=NO_ID;
  Id space_id = NO_ID;

  scanf("%s", input);
  for(i=0; game->obj[i]!=NULL; i++){
    if(ft_strcasecmp(object_get_name(game->obj[i]),input)==0){
      break;
    }
  }
  obj=object_get_id(game->obj[i]);

  for(i=0; game->obj[i]!=NULL; i++){
    if(object_get_dependency(game->obj[i])==obj && player_contain_object(game->pl, object_get_id(game->obj[i]))){
      obj1=object_get_id(game->obj[i]);
      break;
    }
  }
  if(!player_contain_object(game->pl, obj))
    return ERROR;

  space_id = player_get_location(game->pl);

  space_add_object(game_get_space(game, space_id), obj);
  player_del_object(game->pl, obj);
  if(obj1!=NO_ID){
    space_add_object(game_get_space(game, space_id), obj1);
    player_del_object(game->pl, obj1);
  } 
  return OK;
}

/**
 * game_command_attack make a fight between enemy and player */
STATUS game_command_attack(Game *game){
  int x;
/*Error control*/
  if (!game)
    return ERROR;
  if(player_get_location(game->pl)!=enemy_get_location(game->enemy))
    return ERROR;
  if (enemy_get_health(game->enemy) <= 0)
    return ERROR;
  srand(time(NULL));
  x = rand() % MAX_RAND;
/*set new health*/
  if (x < player_get_health(game->pl)/2){
    if (player_set_health(game->pl, player_get_health(game->pl) - 1) == ERROR)
      return ERROR;
  }

  else{
    if (enemy_set_health(game->enemy, enemy_get_health(game->enemy) - 1) == ERROR)
      return ERROR;
  }
  return OK;
}

/**
 * game_command_move make player move
*/
STATUS game_command_move(Game *game){
  char input[WORD_SIZE];  
  
  scanf("%s", input);
  if(!game)
    return ERROR;
  if(ft_strcasecmp(input, "n") == 0 || ft_strcasecmp(input, "north") == 0){
    if (game_command_back(game)==OK)
      return OK;
  }
  else if (ft_strcasecmp(input, "s") == 0 || ft_strcasecmp(input, "south") == 0){
    if (game_command_next(game)==OK)
      return OK;
  }
  else if (ft_strcasecmp(input, "w") == 0 || ft_strcasecmp(input, "west") == 0){
    if (game_command_left(game)==OK)
      return OK;
  }
  else if (ft_strcasecmp(input, "e") == 0 || ft_strcasecmp(input, "east") == 0){
    if (game_command_right(game)==OK)
      return OK;
  }
  else if (ft_strcasecmp(input, "u") == 0 || ft_strcasecmp(input, "up") == 0){
    if (game_command_up(game)==OK)
      return OK;
  }
  else if (ft_strcasecmp(input, "d") == 0 || ft_strcasecmp(input, "down") == 0){
    if (game_command_down(game)==OK)
      return OK;
  }
  return ERROR;
}  

/**
 * game_command_inspect shows a description
*/
STATUS game_command_inspect(Game *game) {
  char input[WORD_SIZE];
  Space *space;
  Object *object;
  int i, obj, ilu=0;
  
  if (!game)
    return ERROR;
  scanf("%s", input);

  for (i=0; game->obj[i]!=NULL; i++){
    if(player_contain_object(game->pl, object_get_id(game->obj[i]))==TRUE){
      if(object_get_illuminate(game->obj[i])== 1 && object_get_turnedon(game->obj[i])== 1){
        ilu=1;
        break;
      }
    }
  }


  if(ft_strcasecmp(input, "s") == 0 || ft_strcasecmp(input, "space") == 0)
  {
    space = game_get_space(game, player_get_location(game->pl));
    if (space_get_illumination(space)==TRUE || (space_get_illumination(space) == FALSE && ilu==1))
      game_set_description(game, space_get_full_description(space));
    else if(space_get_illumination(space)==FALSE)
      game_set_description(game, "This space is not illuminated");
  }
  else
  {
    space = game_get_space(game, player_get_location(game->pl));
    for(i=0; game->obj[i]!=NULL; i++){
      if(ft_strcasecmp(object_get_name(game->obj[i]),input)==0){
        break;
      }
    }
    obj=object_get_id(game->obj[i]);
    object = game_get_obj(game, obj);
    
    if(space_contain_object(space, obj)==TRUE && (space_get_illumination(space)==TRUE || (space_get_illumination(space) == FALSE && ilu==1))){
      game_set_description(game, object_get_description(object));
    }
    else if(space_get_illumination(space) == FALSE && ilu == 0){
      game_set_description(game, "This space is not illuminated, so you can't see the description");
    }
    else{
      game_set_description(game, "That object is not in the space");
    }
  }
  return OK;
}

STATUS game_command_turnon(Game *game){
  char input[WORD_SIZE];
  int i;

  scanf("%s", input);
  for(i=0; game->obj[i]!=NULL; i++){
    if(ft_strcasecmp(object_get_name(game->obj[i]),input)==0){
      break;
    }
  }
  if (object_get_illuminate(game->obj[i]) == FALSE)
    return ERROR;
  if (player_contain_object(game_get_player(game), object_get_dependency(game->obj[i])) == FALSE)
    return ERROR;

  if (object_get_illuminate(game->obj[i])==TRUE){
    if (object_get_turnedon(game->obj[i])==FALSE){
      object_set_turnedon(game->obj[i], TRUE);
      return OK;
    }
    else{
      return ERROR;
    }
  }
  return ERROR;
}

STATUS game_command_turnoff(Game *game){
  char input[WORD_SIZE];
  int i;

  scanf("%s", input);
  for(i=0; game->obj[i]!=NULL; i++){
    if(ft_strcasecmp(object_get_name(game->obj[i]),input)==0){
      break;
    }
  }
  if (object_get_illuminate(game->obj[i])==TRUE){
    if (object_get_turnedon(game->obj[i])==TRUE){
      object_set_turnedon(game->obj[i], FALSE);
      return OK;
    }
    else{
      return ERROR;
    }
  }
  else{
    return ERROR;
  }
}
