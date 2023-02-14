#include <stdio.h>
#include <stdlib.h>
#include "map.h"


#define MAX_NCOLS 64  // Maximum map cols
#define MAX_NROWS 64  // Maximum map rows
#define MAX_BUFFER 64 // Maximum file line size

struct _Map {
    unsigned int nrows, ncols;
    Point *array[MAX_NROWS][MAX_NCOLS]; // array with the Map points
    Point *input, *output;              // points input/output
};

Map * map_new (unsigned int nrows,  unsigned int ncols) {
    int i = 0, j=0;
    Map *map = NULL;

    if (nrows < 1 || nrows > MAX_NROWS || ncols < 1 || ncols > MAX_NCOLS)
        return NULL;

    map = (Map*) malloc(sizeof(Map));
    if (!map)
        return NULL;

    map->nrows = nrows;
    map->ncols = ncols;
    for(i=0;i<MAX_NROWS;i++) {
        for(j=0;j<MAX_NCOLS;j++) {
            map->array[j][i] = NULL;
        }
    }
    map->input = NULL;
    map->output = NULL;

    return map;
}

void map_free (Map* mp) {
    int i=0, j=0;

    if (!mp)
        return;

    for(i=0;i<mp->nrows;i++){
        for(j=0;j<mp->ncols;j++){
            if (!mp->array[i][j])
                free(mp->array[i][j]);
        }
    }

    if(!mp->input)
        free(mp->input);
    if(!mp->output)
        free(mp->output);

    free(mp);
}

Point *map_insertPoint (Map *mp, Point *p){
    int x = 0, y = 0;
    if (!mp || !p) {
        fprintf(stdout, "error arg\n");
        return NULL;
    }

    x = point_getCoordinateX(p);
    y = point_getCoordinateY(p);

    if (x < 0 || x > mp->ncols || y < 0 || y > mp->nrows) {
        fprintf(stdout, "Error getters x y\n");
        return NULL;
    }

    if (point_getSymbol(p) == INPUT){
        if(!mp->input)
            if(map_setInput(mp, p) == ERROR) {
                fprintf(stdout, "Error set input\n");
                return NULL;
            }
    }

    if (point_getSymbol(p) == OUTPUT){
        if(!mp->output)
            if(map_setOutput(mp, p) == ERROR) {
                fprintf(stdout, "Error set output\n");
                return NULL;
            }
    }

    mp->array[y][x] = p;
    if ((mp->array[y][x]) == NULL) {
        fprintf(stdout, "error mp array\n");
        return NULL;
    }

    return mp->array[point_getCoordinateY(p)][point_getCoordinateX(p)];
}

int map_getNcols (const Map *mp){
    if (!mp)
        return -1;

    return mp->ncols;
}

int map_getNrows (const Map *mp){
    if (!mp)
        return -1;

    return mp->nrows;
}

Point * map_getInput(const Map *mp){
    if (!mp)
        return NULL;

    return mp->input;
}

Point * map_getOutput(const Map *mp){
    if (!mp)
        return NULL;

    return mp->output;
}

Point *map_getPoint (const Map *mp, const Point *p){
    if(!mp || !p)
        return NULL;

    return mp->array[point_getCoordinateY(p)][point_getCoordinateX(p)];
}

Point *map_getNeighboor(const Map *mp, const Point *p, Position pos){
    int x = 0, y = 0;
    if(!mp || !p || pos < 0 || pos > 4) {
        fprintf(stdout, "error arg\n");
        return NULL;
    }

    x = point_getCoordinateX(p);
    y = point_getCoordinateY(p);

    if(x < 0 || x > mp->ncols || y < 0 || y > mp->nrows) {
        fprintf(stdout, "error arg 2\n");
        return NULL; 
    }

    switch(pos){
        case RIGHT:
            if(x+1 < mp->ncols && x != -1)
                return mp->array[y][x+1];
        case UP:
            if(y-1 >= 0 && y != -1)
                return mp->array[y-1][x];
        case LEFT:
            if(x-1 >= 0 && x != -1)
                return mp->array[y][x-1];
        case DOWN:
            if(y+1 < mp->nrows && y != -1)
                return mp->array[y+1][x];
        case STAY:
            return mp->array[y][x];
    }

    return NULL;
}

Status map_setInput(Map *mp, Point *p){
    if(!mp || !p)
        return ERROR;

    if(mp->input)
        return ERROR;
        
    mp->input = p;

    return OK;
}

Status map_setOutput (Map *mp, Point *p){
    if(!mp || !p)
        return ERROR;

    if(mp->output)
        return ERROR;
        
    mp->output = p;

    return OK;
}

Map *map_readFromFile(FILE *pf) {
    int i = 0, j = 0, k = 0, nrows, ncols;
    char map_line[MAX_NCOLS];
    Map *mp = NULL;
    Point *p = NULL;
    
    if (!pf) {
        return NULL;
    }

    if (fscanf(pf, "%d %d", &nrows, &ncols) != 2) {
            return NULL;
    }

    mp = map_new(nrows, ncols);
    if (!mp) {
        return NULL;
    }


    for (i=0;i<nrows;i++) {
        fscanf(pf, "%s", map_line);
        for (j=0;j<ncols;j++, k++) {     
            p = point_new(j, i, map_line[j]);
            if(!p) {
                point_free(p);
            }
            map_insertPoint(mp, p);
        }
    }

    return mp;
}

Bool map_equal (const void *_mp1, const void *_mp2){
    Map *aux1 = NULL, *aux2 = NULL;
    int i=0, j=0;

    if(!_mp1 || !_mp2)
        return FALSE;

    aux1 = (Map*)_mp1;
    aux2 = (Map*)_mp2;

    if(aux1->nrows == aux2->nrows){
        if(aux1->ncols == aux2->ncols){
            if(point_equal(aux1->input, aux2->input) == TRUE){
                if(point_equal(aux1->output, aux2->output) == TRUE){
                    for(i=0;i<aux1->nrows;i++){
                        for(j=0;j<aux1->ncols;j++){
                            if(point_equal(aux1->array[i][j], aux2->array[i][j]) == FALSE){
                                return FALSE;
                            }
                        }
                    }
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

int map_print (FILE*pf, Map *mp){
    int c = 0, i = 0, j=0, aux = 0;

    if(!pf || !mp)
        return -1;

    for(i=0;i<mp->nrows;i++){
        for(j=0;j<mp->ncols;j++){
            aux = point_print(pf, mp->array[i][j]);
            if(aux < 0){
                return -1;
            }

            c += aux;
        }
    }

    fprintf(pf, "\n");

    return c;
}

Point * map_dfs (Map *mp){
    return NULL;
}