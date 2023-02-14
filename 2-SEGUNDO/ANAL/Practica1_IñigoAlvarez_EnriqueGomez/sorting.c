/**
 *
 * Descripcion: Implementation of sorting functions
 *
 * Fichero: sorting.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */


#include "sorting.h"
#include <stdio.h>

/*****************/
/* Function: SelectSort   Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that order the array from least        */
/* to gratest                                      */
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/
int SelectSort(int* array, int ip, int iu)
{
  
  int i = 0, counter=0, minm=0, swap=0;
  if (!array || iu < ip || ip<0)
    return ERR;
  
  for (i = ip; i <= iu-1; i++)
  {
    minm = i;
    minm= min(array, i, iu, &counter);
    if (minm == ERR)
      return ERR;
    swap= array[i];
    array[i]= array[minm];
    array[minm]=swap;
  }
  return counter;
}

/***************************************************/
/* Function: SelectSort   Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that order the array from gratest      */
/* to least                                        */
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/***************************************************/
int SelectSortInv(int* array, int ip, int iu)
{
  int i = 0, counter=0, maxm=0, swap=0;
  if (!array || iu < ip || ip<0)
    return ERR;
  
  for (i = ip; i <= iu-1; i++)
  {
    maxm = i;
    maxm= max(array, i, iu, &counter);
    if (maxm == ERR)
      return ERR;
    swap= array[i];
    array[i]= array[maxm];
    array[maxm]=swap;
  }
  return counter;
}

/***************************************************/
/* Function: SelectSort   Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that determins the array minimum       */
/* number                                          */
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int *counter: Number the ob has been executed   */
/* Output:                                         */
/* int: Lower number                               */
/* ERR in case of error                            */
/***************************************************/
int min(int* array, int ip, int iu, int *counter)
{
  int min=0, j=0;

  if (!array || iu<ip || ip<0 || !counter){
    return ERR;
  }

  min=ip;
  for(j=ip+1; j<=iu; j++){
    if(array[j]<array[min]){
      min=j;
    }
    (*counter)++;
  }
  
  return min;
}


/***************************************************/
/* Function: SelectSort   Date:17/09/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that determins the array maximum       */
/* number                                          */
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int *counter: Number the ob has been executed   */
/* Output:                                         */
/* int: Max number                                 */
/* ERR in case of error                            */
/***************************************************/
int max(int* array, int ip, int iu, int *counter)
{
  int max=0, j=0;

  if (!array || iu<ip || ip<0 || !counter){
    return ERR;
  }

  max=ip;
  for(j=ip+1; j<=iu; j++){
    if(array[j]>array[max]){
      max=j;
    }
    (*counter)++;
  }
  return max;
}