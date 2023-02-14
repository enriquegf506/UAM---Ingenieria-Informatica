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
#include <string.h>
#include <stdlib.h>

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

/*****************/
/* Function: MergeSort   Date:11/11/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that order the array from least        */
/* to gratest                                      */
/*                                                 */
/* Input:                                          */
/* int* tabla: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*****************/
int mergesort(int* tabla, int ip, int iu)
{
  int mid = 0;
  int count = 0;
  int c = 0;

  if (!tabla || iu < ip || ip<0)
      return ERR;
  if (ip == iu)
    return OK;
  mid = (iu+ip)/2;
  c = mergesort(tabla, ip, mid);
  if (c == ERR)
    return ERR;
  count += c;
  c = mergesort(tabla, mid+1, iu);
  if (c == ERR)
    return ERR;
  count += c;
  c = merge(tabla, ip, iu, mid);
  if (c == ERR)
    return ERR;
  count += c;
  return count;
}

/***************************************************/
/* Function: merge   Date:11/11/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Function that order the short arrays            */
/*                                                 */
/* Input:                                          */
/* int* tabla: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/***************************************************/
int merge(int* tabla, int ip, int iu, int imedio)
{
  int *aux;
  int i, j, k;
  int count = 0;

  if (!tabla || ip<0 || iu<ip || imedio<ip || iu<imedio)
    return ERR;
  aux = malloc(sizeof(int*) * (iu-ip)+1);
  if (aux == NULL)
    return ERR;
  i = ip;
  j = imedio+1;
  k = 0;
  while (i <= imedio && j <= iu)
  {
    if (tabla[i] < tabla[j])
    {
      aux[k]=tabla[i];
      i++;
    }
    else
    {
      aux[k] = tabla[j];
      j++;
    }
    count++;
    k++;
  }
  if (i > imedio)
  {
    while (j <= iu)
    {
      aux[k] = tabla[j];
      j++;
      k++;
    }
  }  
  else if(j > iu)
  {
    while (i <= imedio)
    {
      aux[k] = tabla[i];
      i++;
      k++;
    }
  }
  i = ip;
  j = iu;
  k = 0;
  while (i <= j)
  {
    tabla[i] = aux[k];
    i++;
    k++;
  }
  
  free(aux);
  return count;
}


/*******/
/* Function: QuickSort   Date:25/10/2022          */
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
/*******/
int quicksort(int* tabla, int ip, int iu){
  int M=0, pos=0, counter=0, ob=0;
  if(ip>iu || !tabla || ip<0){
    return ERR;
  }
  if(ip==iu){
    return 0;
  }
  else{
    counter=partition(tabla,ip,iu, &pos);
    if(counter==ERR){
      return ERR;
    } 
    ob+=counter;
    M=pos;
    if (ip<M-1){
      counter=quicksort(tabla,ip,M-1);
      if(counter==ERR){
      return ERR;
      }
      ob+=counter;
    }
    if (M+1 < iu){
      counter=quicksort(tabla,M+1,iu);
      if(counter==ERR){
      return ERR;
      }
      ob+=counter;
    }
  }
  return ob;
}


/*******/
/* Function: partition   Date:25/10/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Divides the array, on the left the numbers      */
/* those whixh are lower and on the right the higher*/
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*******/
int partition(int* tabla, int ip, int iu,int *pos){
  int k,M,i, swap, counter=0;
  if(ip<0 || iu<ip || !tabla || !pos){
    return ERR;
  }
  counter+= median(tabla,ip, iu, pos);
  if(counter==ERR){
    return ERR;
  }
  M=(*pos);
  k= tabla[M];
  swap= tabla[ip];
  tabla[ip]= tabla[M];
  tabla[M]=swap;
  
  M=ip;
  for(i=ip+1; i<=iu; i++){
    if (tabla[i]<k){
      M++;
      swap= tabla[i];
      tabla[i]= tabla[M];
      tabla[M]=swap;
    }
    counter++;
  }
  swap= tabla[ip];
  tabla[ip]= tabla[M];
  tabla[M]=swap;
  (*pos)=M;
  return counter;
}

/*******/
/* Function: median   Date:25/10/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Return the pivot as pos (the first number)      */
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int *pos: The pivot                             */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*******/

int median(int *tabla, int ip, int iu,int *pos){
  if(ip<0 || iu<ip || !tabla || !pos){
    return ERR;
  }
  (*pos)=ip;
  return 0;
}
/*******/
/* Function: median_avg   Date:4/11/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Return the pivot as pos (the mid number)        */
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int *pos: The pivot                             */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*******/
int median_avg(int *tabla, int ip, int iu, int *pos){
  
  if(ip<0 || iu<ip || !tabla || !pos){
    return ERR;
  }

  (*pos)=(ip + iu)/2;
  return 0;
}

/*******/
/* Function: median_stat  Date:4/11/2022          */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Return the pivot as pos (The median between     */ 
/* iu,ip and the mid)                              */
/*                                                 */
/* Input:                                          */
/* int* array: Array pointer we want to order      */
/* int ip: First array element                     */
/* int iu: Last array element                      */
/* int *pos: The pivot                             */
/* Output:                                         */
/* int: Number of times OB has been executed       */
/* ERR in case of error                            */
/*******/
int median_stat(int *tabla, int ip, int iu, int *pos){

  int m=0;

  if(ip<0 || iu<ip || !tabla || !pos){
    return ERR;
  }
  m=(ip + iu)/2;
  if((tabla[ip]<tabla[iu] && tabla[ip]>tabla[m]) || (tabla[ip]>tabla[iu] && tabla[ip]<tabla[m])){
    *pos=ip;
    return 4;
  }
  else if((tabla[m]<tabla[iu] && tabla[m]>tabla[ip]) || (tabla[m]>tabla[iu] && tabla[m]<tabla[ip])){
    *pos=m;
  }
  else{
    *pos=iu;
  }
  return 8;
}