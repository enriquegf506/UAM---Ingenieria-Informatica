#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "stack_fDoble.h"

#define MAX_RAND 10
#define MAX_ELE 100

typedef int (*f_cmp)(const void *, const void *);

/** Private functions
 */
int int_print (FILE *pf, const void *a);
int int_cmp(const void *c1, const void *c2);
Stack* stack_order(Stack* pila, int (*f_cmp)(const void *, const void *));


int main(int argc, char **argv) {
    int* array = NULL;
    int n = 0, i = 0;
    Stack* pila = NULL, *pila_aux = NULL;

    if(argc < 2) {
        fprintf(stdout, "Faltan argumentos: ./exe n\n");
        return -1;
    }

    n = atoi(argv[1]);
    if (n < 0 || n > MAX_ELE) {
        fprintf(stdout, "N inválido (0 a 100)\n");
        return -1;
    }

    array = (int*) malloc(n*sizeof(int));
    if (!array) {
        fprintf(stdout, "Error reservando array\n");
    }

    srand(time(NULL));

    for (i=0;i<n;i++) {
        array[i] = rand() % MAX_RAND;
        if (!array[i]) {
            free(array);
            return -1;
        }
    }

    pila = stack_init();
    if (!pila) {
        fprintf(stdout, "\nError creando pila.\n");
        free(array);
        return 0;
    }

    for(i=0;i<n;i++) {
        if (stack_push(pila, &array[i]) == ERROR) {
            fprintf(stdout, "\nError insertando %d en la pila.\n", i+1);
            free(array);
            stack_free(pila);
            return 0;
        }
    }

    fprintf(stdout, "Original stack:\n");
    if (stack_print(stdout, pila, int_print) < 0) {
        free(array);
        stack_free(pila);
        return 0;
    }

    pila_aux = stack_order(pila, int_cmp);
    if (!pila_aux) {
        fprintf(stdout, "\nError creando pila auxiliar\n");
        free(array);
        stack_free(pila);
        return 0;
    }

    fprintf(stdout, "Ordered stack:\n");
    if (stack_print(stdout, pila_aux, int_print) < 0) {
        fprintf(stdout, "\n\nerror stackprint\n\n");
        free(array);
        stack_free(pila);
        stack_free(pila_aux);
        return 0;
    }

    fprintf(stdout, "Original stack:\n");
    if (stack_print(stdout, pila, int_print) < 0) {
        free(array);
        stack_free(pila);
        stack_free(pila_aux);
        return 0;
    }

    free(array);
    stack_free(pila);
    stack_free(pila_aux);

    return 0;
}

int int_print (FILE *pf, const void *a) {
    if (!pf || !a) 
        return -1;

    return fprintf(pf, "%d", *(int *)a);
}

int int_cmp(const void *c1, const void *c2) {
    if (!c1 || !c2)
        return INT_MIN;

    return (*(int *)c1 - *(int *)c2);
}

Stack* stack_order(Stack* pila, int (*f_cmp)(const void *, const void *)) {
    Stack* pila_aux = NULL;
    void *e = NULL, *e_aux = NULL;

    if (!pila || stack_isEmpty(pila) == TRUE) {
        return NULL;
    }

    pila_aux = stack_init();
    if (!pila_aux) {
        return NULL;
    }

    while (stack_isEmpty(pila) == FALSE) {
        e = stack_pop(pila);
        if (!e) {
            stack_free(pila_aux);
            return NULL;
        }

        while (stack_isEmpty(pila_aux) == FALSE && f_cmp(e, stack_top(pila_aux)) < 0) {
            e_aux = stack_pop(pila_aux);
            if (!e_aux) {
                while (stack_isEmpty(pila_aux) == FALSE) {
                    e = stack_pop(pila_aux);
                }
                free(pila_aux);
                return NULL;
            }
            if (stack_push(pila, e_aux) == ERROR) {
                while (stack_isEmpty(pila_aux) == FALSE) {
                    e = stack_pop(pila_aux);
                }
                free(pila_aux);
                return NULL;
            }
        }

        if (stack_push(pila_aux, e) == ERROR) {
            while (stack_isEmpty(pila_aux) == FALSE) {
                e = stack_pop(pila_aux);
            }
            free(pila_aux);
            return NULL;
        }
    }

    return pila_aux;
}
