#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "pow.h"
#ifndef MINING_H
#define MINING_H
#define MAX_THREAD 100




typedef struct _Space_t Space_t;


void* solve(void *t);

int mining(int num_hilos, int target);

#endif