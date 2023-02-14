#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "point.h"

#define MAX_RAND 10
#define MAX_ELE 100

int main(int argc, char **argv) {
    int n = 0, i = 0, j = 0, k = 0, cmp = 0;
    double d = 0.;
    Point* origen = NULL;
    Point** p = NULL;

    if(argc < 2) {
        fprintf(stdout, "Faltan argumentos: ./exe n\n");
        return -1;
    }

    n = atoi(argv[1]);
    if (n < 0 || n > MAX_ELE) {
        fprintf(stdout, "N inválido (0 a 100)\n");
        return -1;
    }

    p = (Point**) malloc(n*sizeof(Point*));
    if (!p) {
        return -1;
    }

    srand(time(NULL));

    origen = point_new (0, 0, BARRIER);
    if (!origen) {
        free(p);
        return 0;
    }

    for (i=0;i<n;i++) {
        p[i] = point_new(rand() % MAX_RAND , rand() % MAX_RAND, BARRIER);
        if (!p[i]) {
            for(j=0;j<i;j++) {
                point_free(p[j]);
            }
            point_free(origen);
            free(p);
            return -1;
        }
        fprintf(stdout, "Point p[%d]: ", i);
        point_print (stdout, p[i]);
        point_euDistance (p[i], origen, &d);
        fprintf (stdout, " distance: %lf\n", d);
    }

    for(i=0;i<n;i++) {
        for(j=i;j<n;j++) {
            cmp = point_cmpEuDistance(p[i], p[j]);
            if (cmp == INT_MIN) {
                fprintf(stdout, "\nError comparando distancia euclidea.\n");
                for(k=0;k<n;k++) {
                    point_free(p[k]);
                }
                free(p);
                point_free(origen);
                return 0;
            } else if (cmp == 0 || cmp == 1) {
                fprintf(stdout, "p[%d]  <    p[%d]: False\n", i, j);
            } else if (cmp == -1) {
                fprintf(stdout, "p[%d]  <    p[%d]: True\n", i, j);
            } else {
                fprintf(stdout, "\nError comparando distancia euclidea.\n");
                for(k=0;k<n;k++) {
                    point_free(p[k]);
                }
                free(p);
                point_free(origen);
                return 0;
            }
        }
    }

    for(i=0;i<n;i++) {
        point_free(p[i]);
    }
    free(p);
    point_free(origen);

    return 0;
}