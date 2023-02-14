/***********************************************/
/* Program: exercise1     Date:                */
/* Authors:                                    */
/*                                             */
/* Program that generates two random nunmbers  */
/* between two given numbers                   */
/*                                             */
/* Input: Command Line                         */
/* -limInf: lower limit                        */
/* -limSup: upper limit                        */
/* -numN: ammount of numbers                   */
/* Output: 0: OK, -1: ERR                      */
/***********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "permutations.h"

int main(int argc, char** argv)
{
  int i;
  unsigned int inf, sup, num, k = 0;
  int *n;

  srand(time(NULL));

  if (argc != 7) {
    fprintf(stderr, "Input parameter error:\n\n");
    fprintf(stderr, "%s -limInf <int> -limSup <int> -numN <int>\n", argv[0]);
    fprintf(stderr, "Where:\n");
    fprintf(stderr, " -limInf : Lower limit.\n");
    fprintf(stderr, " -limSup : Upper limit.\n");
    fprintf(stderr, " -numN : ammout of mumbers to generate.\n");
    exit(-1);
  }
  /*printf("Practice no 1, Section 1\n");
  printf("Done by: Your names\n");
  printf("Grupo: Your group\n");*/

  /* check command line */
  for(i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-limInf") == 0) {
      inf = atoi(argv[++i]);
    } else if (strcmp(argv[i], "-limSup") == 0) {
      sup = atoi(argv[++i]);
    } else if (strcmp(argv[i], "-numN") == 0) {
      num = atoi(argv[++i]);
    } else {
      fprintf(stderr, "Wrong Parameter %s \n", argv[i]);
    }
  }

  n = malloc(sizeof(int)*(sup-inf+1));
  if (!n)
    return ERR;
  
  for (i = 0; i < sup-inf+1; i++)
    n[i] = 0;
  

  for (i = 0; i < num; i++)
  {
    k = random_num(inf, sup);
    if (k < 0)
      return ERR;
    k = k - inf;
    n[k]++;
  }
  for (i = 0; i < sup-inf; i++)
    printf("%d  %d\n", i+inf, n[i]);
  
  free(n);
  return 0;
}

