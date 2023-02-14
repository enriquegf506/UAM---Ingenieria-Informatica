#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "point.h"

struct _Point
{
    int x, y;
    char symbol;
    Bool visited;
};

Point *point_new(int x, int y, char symbol)
{
    Point *point = NULL;

    if (x < 0 || y < 0 || symbol == '\0')
        return NULL;

    point = (Point *)malloc(sizeof(Point));
    if (!point)
        return NULL;

    point->x = x;
    point->y = y;
    point->symbol = symbol;
    point->visited = FALSE;

    return point;
}

void point_free(Point *p)
{

    if (!p)
        return;

    free(p);

    p = NULL;
}

int point_getCoordinateX(const Point *p)
{

    if (!p)
        return INT_MAX;

    return p->x;
}

int point_getCoordinateY(const Point *p)
{

    if (!p)
        return INT_MAX;

    return p->y;
}

char point_getSymbol(const Point *p)
{

    if (!p)
        return ERRORCHAR;

    return p->symbol;
}

Bool point_getVisited(const Point *p)
{

    if (!p)
        return FALSE;

    return p->visited;
}

Status point_setCoordinateX(Point *p, int x)
{

    if (!p || x < 0)
        return ERROR;

    p->x = x;

    return OK;
}

Status point_setCoordinateY(Point *p, int y)
{

    if (!p || y < 0)
        return ERROR;

    p->y = y;

    return OK;
}

Status point_setSymbol(Point *p, char symbol)
{

    if (!p || symbol == '\0')
        return ERROR;

    p->symbol = symbol;

    return OK;
}

Status point_setVisited(Point *p, Bool bol)
{

    if (!p || (bol != TRUE && bol != FALSE))
        return ERROR;

    p->visited = bol;

    return OK;
}

Point *point_hardcpy(const Point *src)
{
    Point *point = NULL;

    if (!src)
        return NULL;

    point = (Point *)malloc(sizeof(Point));
    if (!point)
        return NULL;

    point->x = src->x;
    point->y = src->y;
    point->symbol = src->symbol;

    return point;
}

Bool point_equal(const void *p1, const void *p2)
{

    Point *p_aux1 = NULL, *p_aux2 = NULL;

    if (!p1 || !p2)
        return FALSE;

    p_aux1 = (Point *)p1;
    p_aux2 = (Point *)p2;

    if (p_aux1->x == p_aux2->x)
    {
        if (p_aux1->y == p_aux2->y)
        {
            if (p_aux1->symbol == p_aux2->symbol)
                return TRUE;
        }
    }

    return FALSE;
}

Status point_euDistance(const Point *p1, const Point *p2, double *distance)
{
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    if (!p1 || !p2)
    {
        return ERROR;
    }

    x1 = point_getCoordinateX(p1);
    if (x1 == INT_MAX)
    {
        return ERROR;
    }
    y1 = point_getCoordinateY(p1);
    if (y1 == INT_MAX)
    {
        return ERROR;
    }
    x2 = point_getCoordinateX(p2);
    if (x2 == INT_MAX)
    {
        return ERROR;
    }
    y2 = point_getCoordinateY(p2);
    if (y2 == INT_MAX)
    {
        return ERROR;
    }

    (*distance) = sqrt((pow((x1 - x2), 2)) + (pow((y1 - y2), 2)));
    if (distance == NULL)
    {
        return ERROR;
    }

    return OK;
}

int point_cmpEuDistance(const void *p1, const void *p2)
{
    double distancia1 = 0., distancia2 = 0.;
    Point *p = NULL;

    if (!p1 || !p2)
    {
        return INT_MIN;
    }

    p = point_new(0, 0, BARRIER);
    if (!p)
    {
        return INT_MIN;
    }

    if (point_euDistance((Point *)p1, p, &distancia1) == ERROR)
    {
        point_free(p);
        return INT_MIN;
    }
    if (point_euDistance((Point *)p2, p, &distancia2) == ERROR)
    {
        point_free(p);
        return INT_MIN;
    }

    if (distancia1 < distancia2)
    {
        point_free(p);
        return -1;
    }
    else if (distancia1 == distancia2)
    {
        if (point_getCoordinateX(p1) < point_getCoordinateX(p2))
        {
            point_free(p);
            return -1;
        }
        else if (point_getCoordinateX(p1) == point_getCoordinateX(p2))
        {
            point_free(p);
            return 0;
        }
    }
    else
    {
        point_free(p);
        return 1;
    }

    point_free(p);
    return INT_MIN;
}

int point_print(FILE *pf, const void *p)
{
    Point *point = NULL;

    if (!pf || !p)
        return -1;

    point = (Point *)p;

    return fprintf(pf, "[(%d, %d): %c]", point->x, point->y, point->symbol);
}