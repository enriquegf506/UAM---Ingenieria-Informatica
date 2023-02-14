/**
 *
 * Description: Implementation of functions for search
 *
 * File: search.c
 * Author: Carlos Aguirre and Javier Sanz-Cruzado
 * Version: 1.0
 * Date: 14-11-2016
 *
 */

#include "search.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 *  Key generation functions
 *
 *  Description: Receives the number of keys to generate in the n_keys
 *               parameter. The generated keys go from 1 to max. The
 * 				 keys are returned in the keys parameter which must be 
 *				 allocated externally to the function.
 */
  
/**
 *  Function: uniform_key_generator
 *               This function generates all keys from 1 to max in a sequential
 *               manner. If n_keys == max, each key will just be generated once.
 */
void uniform_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for(i = 0; i < n_keys; i++) keys[i] = 1 + (i % max);

  return;
}

/**
 *  Function: potential_key_generator
 *               This function generates keys following an approximately
 *               potential distribution. The smaller values are much more 
 *               likely than the bigger ones. Value 1 has a 50%
 *               probability, value 2 a 17%, value 3 the 9%, etc.
 */
void potential_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for(i = 0; i < n_keys; i++) 
  {
    keys[i] = .5+max/(1 + max*((double)rand()/(RAND_MAX)));
  }

  return;
}

/*****************/
/* Function: init_dictionary                       */
/*                        Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that allocatrs the memory used in a    */
/* dictionary and inicializes it                   */
/*                                                 */
/* Input:                                          */
/* int size: the maximum number of keys in the arr */
/* char oder:If the table needs to be sorted or no */
/* Output:                                         */
/* PDICT: The inicialized dictionart               */
/* NULL in case of error                            */
/*****************/

PDICT init_dictionary (int size, char order)
{
	DICT *dict;
  int i = 0;

  if (size < 0 || order < 0)
    return NULL;
  dict = (PDICT)malloc(sizeof(DICT) * 1);
  if (!dict)
    return NULL;
  dict->n_data = 0;
  dict->order = order;
  dict->size = size;
  dict->table = (int *)malloc(sizeof(int) * size);
  if (!dict->table)
  {
    free(dict);
    return NULL;
  }
  while (i < size)
  {
    dict->table[i] = 0;
    i++;
  }
  return dict;
}

/*****************/
/* Function: free_dictionary                       */
/*                        Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that free the memory used in a         */
/* dictionary                                      */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: Dictionary wanted to free memory   */
/* Output:                                         */
/* void: No output                                    */
/*****************/

void free_dictionary(PDICT pdict)
{
	if (!pdict)
    return;
  free(pdict->table);
  free(pdict);
  return;
}

/*****************/
/* Function: insert_dictionary                     */
/*                        Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that insert the key in the dicctionary */
/*                                                 */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: Dictionary with info needed to     */
/*              insert the key                     */
/* int key: number wanted to insert                */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/

int insert_dictionary(PDICT dict, int key)
{
	int j=0, A=0, count=0;
  if (key < 0 || !dict->table || dict->n_data < 0 || dict->order < 0 || !dict->size)
  {
    return ERR;
  }

  if (dict->n_data == dict->size - 1)
  {
    dict->table = realloc(dict->table, ((dict->size) * 4) + 400);
    if (!dict->table)
    {
      return ERR;
    }
    dict->size = dict->size + 100;
  }

  dict->table[dict->n_data] = key;
  dict->n_data++;

  if (dict->order == NOT_SORTED)
  {
    return count;
  }
  A = dict->table[dict->n_data -1];
  j = (dict->n_data - 2);
  while (j >= 0 && dict->table[j] > A)
  {
    dict->table[j + 1] = dict->table[j];
    j--;
    count++;
  }
  dict->table[j + 1] = A;
  return count;
}

/*****************/
/* Function: massive_insertion_dictionary          */
/*                        Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that insert n_keys in the dicctionary  */
/*                                                 */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: Dictionary with info needed to     */
/*              insert the keys                    */
/* int *keys: Array of numbers wanted to insert    */
/* int n_keys: number of keys wanted to insert     */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/

int massive_insertion_dictionary (PDICT pdict,int *keys, int n_keys)
{
	int i = 0;
  int c = 0;
  int caux = 0;

  if (!pdict || !keys || n_keys < 1)
    return ERR;
  while (i < n_keys)
  {
    caux = insert_dictionary(pdict, keys[i]);
    if (caux == ERR)
      return ERR;
    c += caux;
    i++;
  }
  return c;
}

/*****************/
/* Function: lin_search   Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that calls a function to search a      */
/* number in the permutation                       */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: Dictionary with info needed to find*/
/*              the key                            */
/* pfunc_search method: Function to call           */
/* int key: Number wanted to search                */
/* int *ppos: index of the key                     */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/

int search_dictionary(PDICT pdict, int key, int *ppos, pfunc_search method)
{
	if (!pdict || key < 0 || *ppos < 0 || !method)
    return ERR;
  return method(pdict->table, 0, pdict->n_data - 1, key, ppos);
}

/*****************/
/* Function: bin_search   Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that select the index of the key in    */
/* the permutation                                 */
/*                                                 */
/* Input:                                          */
/* int* table: Array pointer we want to find       */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int key: Number wanted to search                */
/* int *ppos: index of the key                     */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/
int bin_search(int *table,int F,int L,int key, int *ppos)
{
	int ind_mid=0, mid=0, count=0;

  if (F > L || F < 0 || !table || key < 0 )
  {
    return ERR;
  }
  while (F <= L)
  {
    ind_mid = (F + L) / 2;

    mid = table[ind_mid];
    count++;
    if (key == mid)
    {
      (*ppos) = ind_mid;
      return count;
    }

    if (key < mid)
    {
      L = ind_mid - 1;
    }
    else
    {
      F = ind_mid + 1;
    }
  }
  (*ppos) = NOT_FOUND;
  return count;
}

/*****************/
/* Function: lin_search   Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that select the index of the key in    */
/* the permutation                                 */
/*                                                 */
/* Input:                                          */
/* int* table: Array pointer we want to find       */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int key: Number wanted to search                */
/* int *ppos: index of the key                     */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/

int lin_search(int *table,int F,int L,int key, int *ppos)
{
	int i = F;
  int c = 0;

  if (!table || L < F || key < 0 || F < 0)
    return ERR;

  while (i <= L)
  {
    c++;
    if (table[i] == key)
    {
      (*ppos) = i;
      return c;
    }
    i++;
  }
  (*ppos) = NOT_FOUND;
  return c;
}

/*****************/
/* Function: lin_auto_search   Date:17/09/2022     */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that select the index of the key in    */
/* the permutation and swap this number            */
/* whith the one before                            */
/*                                                 */
/* Input:                                          */
/* int* table: Array pointer we want to find       */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int key: Number wanted to search                */
/* int *ppos: index of the key                     */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/

int lin_auto_search(int *table,int F,int L,int key, int *ppos)
{
	int i = F;
  int c = 0;
  int aux = 0;

  if (!table || L < F || key < 0 || F < 0)
    return ERR;
  while (i <= L)
  {
    c++;
    if (i == 0 && table[i] == key)
    {
      (*ppos) = i;
      return c;
    }
    if (table[i] == key)
    {
      (*ppos) = i;
      aux = table[i]; 
      table[i] = table[i - 1];
      table[i - 1] = aux;
      return c;
    }
    i++;
  }
  (*ppos) = NOT_FOUND;
  return c;
}


