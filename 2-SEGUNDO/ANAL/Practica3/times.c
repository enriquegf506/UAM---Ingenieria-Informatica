/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include "times.h"
#include "sorting.h"
#include "permutations.h"
#include "search.h"
#include <stdlib.h>

/***************************************************/
/* Function: average_sorting_time  Date:17/09/2022 */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Calls the function saved in metodo N times and  */
/* save all the possible values in the struct PTIME*/
/*                                                 */
/* Input:                                          */
/* pfunc_sort metodo indicates the funtion to      */
/* call and save the data                          */
/* int n_perms: Number of permutations in generate */
/* perms                                           */
/* int N: Number of elements of the array          */
/* PTIME_AA ptime: The structure where data is     */
/* saved                                           */
/* Output:                                         */
/* OK                                              */
/* ERR in case of error                            */
/***************************************************/
short average_sorting_time(pfunc_sort metodo, int n_perms, int N, PTIME_AA ptime)
{
  int **permutations;
  int i = 0;
  clock_t start, end;
  int count = 0;
  int count_min = 0;
  int count_total = 0;
  int count_max = 0;
  double a = 0;

  if (!metodo || n_perms < 0 || N < 0 || ptime ==NULL)
    return ERR;
  permutations = generate_permutations(n_perms, N);
  if (permutations == NULL)
    return ERR;
  for (i = 0; i < n_perms; i++)
  {
    start = clock();
    count = metodo(permutations[i], 0, N - 1);
    if (count == ERR)
    {
      while (i < n_perms)
      {
        free(permutations[i]);
        i++;
      }
      free(permutations);
      return ERR;
    }
    end = clock();
    if(!end || !start){
      while (i < n_perms)
      {
        free(permutations[i]);
        i++;
      }
      free(permutations);
      return ERR;
    }
    count_total += count;
    if (count <= count_min || count_min == 0)
      count_min = count;
    if (count >= count_max)
      count_max = count;
    a += ((end - start));
  }
  i = 0;
  ptime->time = a / n_perms;
  ptime->n_elems = n_perms;
  ptime->N = N-1;
  ptime->average_ob = count_total/n_perms;
  ptime->min_ob = count_min;
  ptime->max_ob = count_max;
  while (i < n_perms)
  {
    free(permutations[i]);
    i++;
  }
  free(permutations);
  return OK;
}

/***************************************************/
/* Function: generate_sorting_times                */
/* Date:17/09/2022                                 */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Calls the function average sorting times to     */
/* store the data and save time table whith nums   */
/* between a num min and a num max                 */
/*                                                 */
/* Input:                                          */
/* pfunc_sort metodo indicates the funtion to      */
/* call and save the data                          */
/* int n_perms: Number of permutations in generate */
/* perms                                           */
/* char *file: The file in which the data is saved */
/* int num_min: Minimum num of N                   */
/* int num_max: Maximum num of N                   */
/* int incr: How N increase                        */
/* Output:                                         */
/* OK                                              */
/* ERR in case of error                            */
/***************************************************/
short generate_sorting_times(pfunc_sort method, char* file, int num_min, int num_max, int incr, int n_perms)
{
  PTIME_AA time;
  int i, N, j;

  if (!method || !file || num_min < 0 || num_max < 0 || num_min > num_max || incr <= 0 || n_perms < 0)
    return ERR;
  N = ((num_max-num_min)/incr) + 1;
  time = malloc(sizeof(TIME_AA)*N);
  if (!time)
    return ERR;
  for (i = 0, j = num_min; i < N; i++)
  {
    if(average_sorting_time(method, n_perms, j, &time[i])==ERR){
      free(time);
      return ERR;

    }
    j+=incr;
    if (j > num_max + 1)
      break;
  }
  if(save_time_table(file, time, i) == ERR){
    free(time);
    return ERR;
  }

  free(time);
  return OK;
}

/***************************************************/
/* Function: save_time_table                       */
/* Date:09/10/2022                                 */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Open a file and write on it the data saved      */
/*                                                 */
/* Input:                                          */
/* char* file: file we want to write on it         */
/* PTIME_AA ptime: data we want to write           */
/* int n_times: The number of tables of data we    */
/* have                                            */
/* Output:                                         */
/* OK                                              */
/* ERR in case of error                            */
/***************************************************/
short save_time_table(char* file, PTIME_AA ptime, int n_times)
{
  FILE *f;
  int i = 0;

  if (!file || !ptime || n_times <= 0)
    return ERR;
  f = fopen(file, "w");
  if (!f)
    return ERR;
  for (i = 0; i < n_times; i++)
  {
    fprintf(f, "Size: %d\nExecution time: %f\nAverage: %f\nMax: %d\nMin: %d\n", ptime[i].N, ptime[i].time, ptime[i].average_ob, ptime[i].max_ob, ptime[i].min_ob);
  }
  fclose(f);
  return OK;
}


/***************************************************/
/* Function: average_search_time  Date:17/09/2022 */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Calls the function saved in metodo N times and  */
/* save all the possible values in the struct PTIME*/
/*                                                 */
/* Input:                                          */
/* pfunc_sort metodo indicates the funtion to      */
/* call and save the data                          */
/* pfunc_key_generator generator: function to call */
/* to generate keys                                */
/* int order: wether if the table is sorted or not */
/* int n_times: Number of keys to search in the dic*/
/* int N: Number of elements of the array          */
/* PTIME_AA ptime: The structure where data is     */
/* saved                                           */
/* Output:                                         */
/* OK                                              */
/* ERR in case of error                            */
/***************************************************/

short average_search_time(pfunc_search method, pfunc_key_generator generator, char order, int N, int n_times, PTIME_AA ptime)
{
  PDICT dictionary;
  int *permutation;
  int i = 0;
  clock_t start, end;
  int count = 0;
  int count_min = 0;
  double count_total = 0;
  int count_max = 0;
  double a = 0;
  int ppos = 0;
  int *keys_table = NULL;

  if (!method || !generator || N < 0 || ptime == NULL || n_times < 0 || order < 0 || order > 1)
    return ERR;

  dictionary = init_dictionary(N, order);
  if (!dictionary)
    return ERR;

  permutation = generate_perm(N);
  if (permutation == NULL){
    free_dictionary(dictionary);
    return ERR;
  }  

  count = massive_insertion_dictionary(dictionary, permutation, N);
  if (count == ERR){
    free_dictionary(dictionary);
    free(permutation);
    return ERR;
  }
    
  keys_table = malloc(sizeof(int) * (n_times * N));
  if (keys_table == NULL){
    free_dictionary(dictionary);
    free(permutation);
    return ERR;
  }
    
  generator(keys_table, n_times * N, N);
  if(!keys_table){
    free(keys_table);
    free_dictionary(dictionary);
    free(permutation);
    return ERR;
  }

  for (i = 0; i < n_times * N; i++)
  {
    start = clock();
    count = search_dictionary(dictionary, keys_table[i], &ppos, method);
    if (count == ERR || ppos<0 || ppos > N)
    {
      free(keys_table);
      free_dictionary(dictionary);
      free(permutation);
      return ERR;
    }
    end = clock();
    if (!end || !start)
    {
      free(keys_table);
      free_dictionary(dictionary);
      free(permutation);
      return ERR;
    }
    count_total += count;
    if (count <= count_min || count_min == 0)
      count_min = count;
    if (count >= count_max)
      count_max = count;
    a += ((end - start));
  }

  ptime->time = a / (n_times * N);
  ptime->n_elems = n_times * N;
  ptime->N = N;
  ptime->average_ob = count_total / (n_times * N);
  ptime->min_ob = count_min;
  ptime->max_ob = count_max;

  free_dictionary(dictionary);
  free(keys_table);
  free(permutation);
  return OK;
}

/***************************************************/
/* Function: generate_search_times                */
/* Date:17/09/2022                                 */
/* Authors: Íñigo Álvarez and Enrique Gómez        */
/*                                                 */
/* Calls the function average sorting times to     */
/* store the data and save time table whith nums   */
/* between a num min and a num max                 */
/*                                                 */
/* Input:                                          */
/* pfunc_sort metodo indicates the funtion to      */
/* call and save the data                          */
/* pfunc_key_generator generator: function to call */
/* to generate keys                                */
/* int order: wether if the table is sorted or not */
/* char *file: The file in which the data is saved */
/* int num_min: Minimum num of N                   */
/* int num_max: Maximum num of N                   */
/* int incr: How N increase                        */
/* Output:                                         */
/* OK                                              */
/* ERR in case of error                            */
/***************************************************/

short generate_search_times(pfunc_search method, pfunc_key_generator generator, int order, char* file, int num_min, int num_max, int incr, int n_times)
{
  PTIME_AA time;
  int i, N, j;

  if (!method || !file || !generator || num_min < 0 || num_max < 0 || num_min > num_max || incr <= 0 || n_times < 0 || order < 0 || order > 1)
    return ERR;
  N = ((num_max - num_min) / incr) + 1;
  time = malloc(sizeof(TIME_AA) * N);
  if (!time)
    return ERR;
  for (i = 0, j = num_min; i < N; i++)
  {
    if (average_search_time(method, generator, order, j, n_times, &time[i]) == ERR)
    {
      free(time);
      return ERR;
    }
    j += incr;
    if (j > num_max + 1)
      break;
  }
  if (save_time_table(file, time, i) == ERR)
  {
    free(time);
    return ERR;
  }

  free(time);
  return OK;
}