#include "mining.h"

static int flag = 0;
static int res = 0;

struct _Space_t
{
    int s_ini;
    int s_fin;
    int target;
};

void* solve(void *t)
{
  int i=0;
  int num = 0;
  Space_t *space = t;
  for(i = space->s_ini; i < space->s_fin; i++){
    num = pow_hash(i);
    if(num == space->target )
    {
        flag = 1;
        res = i;
    }
    if(flag == 1) return NULL;
  }
  return NULL;
}

int mining(int num_hilos, int target)
{
    int i = 0;
    int error = 0;
    pthread_t *threads;
    Space_t spaces[MAX_THREAD];

    flag = 0;

    threads = (pthread_t*) malloc (sizeof (pthread_t)*num_hilos);

    for(i = 0;i < num_hilos; i++)
    {

        spaces[i].target= target;
        spaces[i].s_ini = ((i * POW_LIMIT) / num_hilos );
        spaces[i].s_fin = (((i +1) * POW_LIMIT )/ num_hilos);

        error = pthread_create(&threads[i], NULL, solve, &spaces[i]);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }


    for( i = 0;i < num_hilos; i++)
    {
        error = pthread_join(threads[i], NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }

    printf("target: %d \t sol: %d \n", target, res);
    free(threads);

    return res;
}


