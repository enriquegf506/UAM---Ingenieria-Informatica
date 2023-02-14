#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "types.h"
#define ROWS 3
#define COLS 4
#define MAX_POINTS 12

int main() {
    int i = 0, j = 0, k = 0;
    Map* map = NULL;
    Point* point[MAX_POINTS];

    map = map_new(ROWS, COLS);
    if (!map) {
        fprintf(stdout, "Error creando mapa\n");
        return -1;
    }

    for(i=0, k=0;i<ROWS;i++) {
        for(j=0;j<COLS;j++, k++) {
            if (i == 1 && j == 1) {
                point[k] = point_new(j, i, INPUT);
                if (point[k] == NULL) {
                    fprintf(stdout, "Error creating input\n");
                    return -1;
                }
                if (map_insertPoint(map, point[k]) == NULL) {
                    fprintf(stdout, "Error input\n");
                    return -1;
                }
            } else if (i == 1 && j == 2) {
                point[k] = point_new(j, i, OUTPUT);
                if (point[k] == NULL) {
                    fprintf(stdout, "Error creating output\n");
                    return -1;
                }
                if (map_insertPoint(map, point[k]) == NULL) {
                    fprintf(stdout, "Error output\n");
                    return -1; 
                }
            } else {
                point[k] = point_new(j, i, BARRIER);
                if (point[k] == NULL) {
                    fprintf(stdout, "Error creating barrier\n");
                    return -1;
                }
                if (map_insertPoint(map, point[k]) == NULL) {
                    fprintf(stdout, "Error barrierr\n");
                    return -1;
                }
            }
        }
    }


    fprintf(stdout, "Map:\n");
    fprintf(stdout, "%d, %d\n", map_getNrows(map), map_getNcols(map));

    if (map_print(stdout, map) == -1) {
        map_free(map);
        return -1;
    }

    fprintf(stdout, "Get output neighboors:\n");

    if (point_print(stdout, map_getNeighboor(map, map_getOutput(map), RIGHT)) < 0) {
        fprintf(stdout, "Error neighboor right\n");
    }
    if (point_print(stdout, map_getNeighboor(map, map_getOutput(map), UP)) < 0) {
        fprintf(stdout, "Error neighboor up\n");

    }
    if (point_print(stdout, map_getNeighboor(map, map_getOutput(map), LEFT)) < 0) {
        fprintf(stdout, "Error neighboor left\n");
        
    }
    if (point_print(stdout, map_getNeighboor(map, map_getOutput(map), DOWN)) < 0) {
        fprintf(stdout, "Error neighboor down\n");
     
    }

    printf("\n");


    for(i=0;i<MAX_POINTS;i++) {
        point_free(point[i]); 
    }
    map_free(map);
    return 0;
}