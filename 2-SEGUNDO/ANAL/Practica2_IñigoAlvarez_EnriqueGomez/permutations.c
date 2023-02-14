/**
 *
 * Descripcion: Implementation of function that generate permutations
 *
 * File: permutations.c
 * Autor: Carlos Aguirre
 * Version: 1.1
 * Fecha: 21-09-2019
 *
 */


#include "permutations.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/***************************************************/
/* Function: random_num Date:                      */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Rutine that generates a random number           */
/* between two given numbers                       */
/*                                                 */
/* Input:                                          */
/* int inf: lower limit                            */
/* int sup: upper limit                            */
/* Output:                                         */
/* int: random number                              */
/***************************************************/
int random_num(int inf, int sup)
{
  int n = 0;

  if (inf > sup || inf < 0 || sup < 0)
    return ERR;
  n = rand () % (sup-inf) + inf;
  
  return n;
}

/***************************************************/
/* Function: generate_perm Date:                   */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Rutine that generates a random permutation      */
/*                                                 */
/* Input:                                          */
/* int n: number of elements in the permutation    */
/* Output:                                         */
/* int *: pointer to integer array                 */
/* that contains the permitation                   */
/* or NULL in case of error                        */
/***************************************************/
int* generate_perm(int N)
{
  int i = 0;
  int j = 0;
  int aux = 0;
  int *perm = NULL;

  if (N < 0)
    return NULL;
  perm = (int*)malloc(sizeof(int) * N);
  if (!perm)
    return NULL;

  for (i = 0; i < N; i++)
    perm[i] = i;
  for (i = 0; i < N; i++)
  {
    aux = perm[i];
    j = random_num(i, N);
    if (j == ERR) {
      free(perm);
      return NULL;
    }
    perm[i] = perm[j];
    perm[j] = aux;
  }
  return perm;
}

/***************************************************/
/* Function: generate_permutations Date:17/09/2022 */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that generates n_perms random          */
/* permutations with N elements                    */
/*                                                 */
/* Input:                                          */
/* int n_perms: Number of permutations             */
/* int N: Number of elements in each permutation   */
/* Output:                                         */
/* int**: Array of pointers to integer that point  */
/* to each of the permutations                     */
/* NULL en case of error                           */
/***************************************************/
int** generate_permutations(int n_perms, int N)
{
  int i = 0;
  int **perm = NULL;

  if (N < 0 || n_perms < 0)
    return NULL;
  perm = malloc(sizeof(int*) * n_perms);
  if (!(perm))
    return NULL;
  for (i = 0; i < n_perms; i++)
  {
    perm[i] = generate_perm(N);
    if (perm[i] == NULL)
    {
      while (i >= 0)
      {
        i--;
        free(perm[i]);
      }
      free(perm);
      return NULL;
    }
  }
  return perm;
}
