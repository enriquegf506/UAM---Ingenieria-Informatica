#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "point.h"
#include "types.h"



struct _Point {
    int x, y;
    char symbol;

    Bool visited; // for DFS
};


Point * point_new (int x, int y, char symbol){
    Point *point =  NULL;

    if(x < 0 || y < 0 || symbol == '\0')
        return NULL;

    point = (Point*)malloc(sizeof(Point));
    if(!point)
        return NULL;

    point->x = x;
    point->y = y;
    point->symbol = symbol;

    return point;

}

void point_free (Point *p){

    if(!p)
        return;

    free(p);

}

int point_getCoordinateX (const Point *p){

    if(!p)
        return INT_MAX;

    return p->x;

}

int point_getCoordinateY (const Point *p){

    if(!p)
        return INT_MAX;

    return p->y;

}

char point_getSymbol (const Point *p){

    if(!p)
        return ERRORCHAR;

    return p->symbol;

}

Status point_setCoordinateX (Point *p, int x){

    if(!p || x < 0)
        return ERROR;

    p->x = x;

    return OK;

}

Status point_setCoordinateY (Point *p, int y){

    if(!p || y < 0)
        return ERROR;

    p->y = y;

    return OK;

}

Status point_setSymbol (Point *p, char symbol){

    if(!p || symbol == '\0')
        return ERROR;

    p->symbol = symbol;

    return OK;

}

Point *point_hardcpy (const Point *src){
    Point *point =  NULL;

    if(!src)
        return NULL;

    point = (Point*)malloc(sizeof(Point));
    if(!point)
        return NULL;

    point->x = src->x;
    point->y = src->y;
    point->symbol = src->symbol;

    return point;

}

Bool point_equal (const void *p1, const void *p2){

    Point *p_aux1 = NULL, *p_aux2 = NULL;

    if(!p1 || !p2)
        return FALSE;

    p_aux1 = (Point*)p1;
    p_aux2 = (Point*)p2;

    if(p_aux1->x == p_aux2->x)
    {
        if(p_aux1->y == p_aux2->y)
        {
            if(p_aux1->symbol == p_aux2->symbol)
                return TRUE;
        }
    }

    return FALSE;

}

int point_print (FILE *pf, const void *p){ 
    Point *point = NULL;

    if(!pf || !p)
        return -1;

    point = (Point*)p;

    return fprintf(pf, "[(%d, %d): %c]", point->x, point->y, point->symbol); 

}
