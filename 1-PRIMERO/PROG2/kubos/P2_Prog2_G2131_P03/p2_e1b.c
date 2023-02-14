#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "point.h"
#include "stack_fDoble.h"

#define MAX_RAND 10
#define MAX_ELE 10

/** Private funcion
 */
Stack* stack_orderPoints(Stack* pila);

int main(int argc, char **argv) {
    int n = 0, i = 0, j = 0;
    double d = 0.;
    Point* origen = NULL, **p = NULL;
    Stack* pila = NULL, *pila_aux = NULL;

    if(argc < 2) {
        fprintf(stdout, "Faltan argumentos: ./exe n\n");
        return -1;
    }

    n = atoi(argv[1]);
    if (n < 0 || n > MAX_ELE) {
        fprintf(stdout, "N inválido (0 a 10)\n");
        return -1;
    }

    p = (Point**) malloc(n*sizeof(Point*));
    if (!p) {
        return -1;
    }

    srand(time(NULL));

    origen = point_new(0, 0, BARRIER);
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

    pila = stack_init();
    if (!pila) {
        fprintf(stdout, "\nError creando pila.\n");
        for(i=0;i<n;i++) {
                point_free(p[i]);
        }
        point_free(origen);
        free(p);
        return 0;
    }

    for(i=0;i<n;i++) {
        if (stack_push(pila, p[i]) == ERROR) {
            fprintf(stdout, "\nError insertando %d en la pila.\n", i+1);
            for(i=0;i<n;i++) {
                    point_free(p[i]);
                }
            free(p);
            point_free(origen);
            stack_free(pila);
            return 0;
        }
    }

    fprintf(stdout, "Original stack:\n");
    if (stack_print(stdout, pila, point_print) < 0) {
        for(i=0;i<n;i++) {
            point_free(p[i]);
        }
        free(p);
        point_free(origen);
        stack_free(pila);
        return 0;
    }

    pila_aux = stack_orderPoints(pila);
    if (!pila_aux) {
        fprintf(stdout, "\nError creando pila auxiliar\n");
        for(i=0;i<n;i++) {
            point_free(p[i]);
        }
        free(p);
        point_free(origen);
        stack_free(pila);
        return 0;
    }

    fprintf(stdout, "Ordered stack:\n");
    if (stack_print(stdout, pila_aux, point_print) < 0) {
        fprintf(stdout, "\n\nerror stackprint\n\n");
        for(i=0;i<n;i++) {
            point_free(p[i]);
        }
        free(p);
        point_free(origen);
        stack_free(pila);
        stack_free(pila_aux);
        return 0;
    }

    fprintf(stdout, "Original stack:\n");
    if (stack_print(stdout, pila, point_print) < 0) {
        for(i=0;i<n;i++) {
            point_free(p[i]);
        }
        free(p);
        point_free(origen);
        stack_free(pila);
        stack_free(pila_aux);
        return 0;
    }

    for(i=0;i<n;i++) {
        point_free(p[i]);
    }
    free(p);
    point_free(origen);
    stack_free(pila);
    stack_free(pila_aux);

    return 0;
}

Stack* stack_orderPoints(Stack* pila) {
    Stack* pila_aux = NULL;
    Point* p = NULL, *p_aux = NULL;

    if (!pila || stack_isEmpty(pila) == TRUE) {
        return NULL;
    }

    pila_aux = stack_init();
    if (!pila_aux) {
        return NULL;
    }

    while (stack_isEmpty(pila) == FALSE) {
        p = (Point*)stack_pop(pila);
        if (!p) {
            stack_free(pila_aux);
            return NULL;
        }

        while (stack_isEmpty(pila_aux) == FALSE && point_cmpEuDistance(p, stack_top(pila_aux)) == -1) {
            p_aux = (Point*)stack_pop(pila_aux);
            if (!p_aux) {
                while (stack_isEmpty(pila_aux) == FALSE) {
                    p = (Point*)stack_pop(pila_aux);
                    point_free(p);
                }
                free(pila_aux);
                return NULL;
            }
            if (stack_push(pila, p_aux) == ERROR) {
                while (stack_isEmpty(pila_aux) == FALSE) {
                    p = (Point*)stack_pop(pila_aux);
                    point_free(p);
                }
                point_free(p_aux);
                free(pila_aux);
                return NULL;
            }
        }

        if (stack_push(pila_aux, p) == ERROR) {
            while (stack_isEmpty(pila_aux) == FALSE) {
                p = (Point*)stack_pop(pila_aux);
                point_free(p);
            }
            point_free(p_aux);
            free(pila_aux);
            return NULL;
        }
    }

    return pila_aux;
}