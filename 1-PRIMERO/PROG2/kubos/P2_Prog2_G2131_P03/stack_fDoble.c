#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "stack_fDoble.h"


struct _Stack {
    void **item;     /*!<Dynamic array of elements*/
    int top;         /*!<index of the top element in the stack*/
    int capacity;    /*!<xcapacity of the stack*/
};



Stack* stack_init() {
    int i = 0;
    Stack* pila = NULL;

    pila = (Stack*) malloc(sizeof(Stack));
    if (!pila) {
        return NULL;
    }

    pila->item = (void**) malloc(INIT_CAPACITY * sizeof(void*));
    if (!pila->item) {
        stack_free(pila);
        return NULL;
    }

    for(i=0;i<INIT_CAPACITY;i++) {
        pila->item[i] = NULL;
    }

    pila->top = -1;
    pila->capacity = INIT_CAPACITY;

    return pila;
}

void stack_free (Stack *s) {
    if (!s) {
        return;
    }

    if (s->item) {
        free(s->item);
    }
    free(s);

    return;
}

Status stack_push (Stack *s, const void *ele) {
    int i = 0;
    
    if (!s || !ele) {
        return ERROR;
    }

    if (s->top == s->capacity - 1) {
        s->item = (void**) realloc(s->item, FCT_CAPACITY * s->capacity * sizeof(void*));
        if (!s->item) {
            return ERROR;
        }

        s->capacity *= FCT_CAPACITY;

        for (i=s->top+1;i<s->capacity;i++){
            s->item[i] = NULL;
        }
    }
    
    s->top++;
    s->item[s->top] = (void*) ele;

    return OK;
}

void* stack_pop (Stack *s) {
    void* pop = NULL;

    if (!s || stack_isEmpty(s)) {
        return NULL;
    }

    pop = s->item[s->top];
    if (!pop) {
        return NULL;
    }
    s->item[s->top] = NULL;
    s->top--;

    return pop;
}

void* stack_top (const Stack *s) {
    if (!s) {
        return NULL;
    }

    return s->item[s->top];
}

Bool stack_isEmpty (const Stack *s) {
    if (!s) {
        return TRUE; /** To avoid accumulate errors in stack TAD and e1 */
    }

    if (s->top != -1) {
        return FALSE;
    }

    return TRUE;
}

size_t stack_size (const Stack *s) {
    if (!s) {
        return INT_MAX;
    }

    return s->top;
}

int stack_print(FILE* fp, const Stack *s,  P_stack_ele_print f) {
    int c = 0, aux = 0, i = 0;
    size_t size = 0;

    if (!fp || !s || !f) {
        return -1;
    }

    size  = stack_size(s);
    if (size == INT_MAX) {
        return -1;
    }

    c = fprintf(stdout, "SIZE: %ld\n", size);
    if (c < 0) {
        return -1;
    }

    for (i=0;i<=size;i++) {
        aux = f(fp, s->item[i]);
        if (aux < 0) {
            return -1;
        }
        c += aux;
    }

    return c;
}