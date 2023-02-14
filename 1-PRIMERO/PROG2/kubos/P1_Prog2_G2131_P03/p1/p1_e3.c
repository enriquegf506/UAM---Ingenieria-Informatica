#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "point.h"

int main(int argc, char **argv)
{

    FILE *f = NULL;
    Map *mp = NULL;
    Point *p = NULL, *aux = NULL, *aux1 = NULL;
    int i = 0, j = 0;

    if (argv[1] == NULL) {
        fprintf(stdout, "\nNo se ha introducido argumento\n");
        return -1;
    }

    f = fopen(argv[1], "r");
    if (!f) {
        return -1;
    }

    mp = map_readFromFile(f);
    if (!mp) {
        return -1;
    }

    fprintf(stdout, "Maze:\n");
    fprintf(stdout, "%d %d", map_getNrows(mp), map_getNcols(mp));
    fprintf(stdout, "\n");

    if (map_print(stdout, mp) < 0) {
        map_free(mp);
        return -1;
    }

    fprintf(stdout, "Get output neighboors:\n");

    for (i=0;i<map_getNrows(mp);i++) {
        for (j=0;j<map_getNcols(mp);j++) {
            p = point_new(j, i, OUTPUT);
            if(!p)
                return -1;
            aux = map_getPoint(mp, p);
            if(!aux)
                return -1;
            if (point_getSymbol(p) == point_getSymbol(aux)) {
                if(map_setOutput(mp, aux)==ERROR)
                    return -1;
                if(point_print(stdout, map_getNeighboor(mp, map_getOutput(mp), RIGHT)) < 0)
                    return -1;
                if(point_print(stdout, map_getNeighboor(mp, map_getOutput(mp), UP)) < 0)
                    return -1;
                if(point_print(stdout, map_getNeighboor(mp, map_getOutput(mp), LEFT)) < 0)
                    return -1;
                if(point_print(stdout, map_getNeighboor(mp, map_getOutput(mp), DOWN)) < 0)
                    return -1;
            }

            point_free(p);
        }
    }

    fprintf(stdout, "\nGet right inferior corner neighboors:");
    for (i=0;i<map_getNrows(mp);i++) {
        for(j=0;j<map_getNcols(mp);j++) {
            if ((i == (map_getNrows(mp) - 1)) && (j == (map_getNcols(mp) - 1))) {
                p = point_new(j, i, BARRIER);
                if(!p)
                    return -1;
                aux1 = map_getPoint(mp, p);
                if(!aux)
                    return -1;
                fprintf(stdout, "\n");
                if(point_print(stdout, map_getNeighboor(mp, aux1, UP)) < 0) 
                    return -1;
                if(point_print(stdout, map_getNeighboor(mp, aux1, LEFT)) < 0)
                    return -1;
            }
        }
        
    }
    fprintf(stdout, "\n");

    point_free(p);
    map_free(mp);
    fclose(f);

    return 0;
}