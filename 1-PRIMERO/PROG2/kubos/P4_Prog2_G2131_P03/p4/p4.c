#include <stdlib.h>
#include <stdio.h>
#include "bstree.h"
#include "point.h"

BSTree *tree_read_points_from_file(FILE *pf);

int main(int argc, char *argv[])
{
    BSTree* t = NULL;
    FILE *f = fopen(argv[1], "r");
    Point *p1 = NULL, *p2 = NULL;
    int i = 0;

    if (argc != 2 || !argv || !f) 
    {
        fprintf(stdout, "Faltan argumentos: ./exe file.txt o error abriendo file");
        return 1;
    }

    t = tree_read_points_from_file(f);
    if (!t) {
        return 1;
    }

    void* min = tree_find_min(t);
    void* max = tree_find_max(t);


    printf("\nMin node: ");
    point_print(stdout, min);

    printf("\nMax node: ");
    point_print(stdout, max);


    p1 = point_new(0, 0, '+');

    p2 = point_new(6, 4, '+');

    printf("\nInserting new node p1:");
    if(tree_insert(t, p1) == ERROR)
        printf("Error inserting new node.\n");
    point_print(stdout, p1);

    printf("\nNew min node: ");
    point_print(stdout, tree_find_min(t));

    if(tree_contains(t, p1) == TRUE){
        printf("\nThe point ");
        point_print(stdout, p1);
        printf(" is contained in the tree.\n");
    }

    printf("Removing p1 from the tree.\n");
    if(tree_remove(t, p1) == ERROR)
        printf("Error removing the node.\n");
    printf("p1 has been removed.\n");


    printf("New min node: ");
    point_print(stdout, tree_find_min(t));

    printf("\n");

    if(tree_contains(t, p2) == TRUE){
        printf("Error, p2 should not be in the tree.\n");
    } 

    if(tree_size(t) != 11){
        printf("Error size.\n");
        return 0;
    }
    printf("Tree size: %ld.\n", tree_size(t));

    tree_destroy(t);
    if(!t){
        printf("Arbol eliminado.\n");
    }

    printf("Creating new tree: ");

    t = tree_init(point_print, point_cmpEuDistance);
    if (!t)
    {
        printf("Error creando nuevo arbol.\n");
        return 0;
    }

    printf("OK.\nRemoving p1: ");

    if(tree_remove(t, p1) == ERROR){
        printf("Error p1 is not in the tree already.\n");
    }

    printf("OK.\nInserting p2.\n");

    if(tree_insert(t, p2) == ERROR){
        printf("Error inserting p2.\n");
    }

    printf("Tree size now is %ld.\n", tree_size(t));

    if(tree_contains(t, p2) == TRUE){
        printf("Now p2 is in the tree.\n");
    }


    fclose(f);
    tree_destroy(t);

    return 0;
}

BSTree *tree_read_points_from_file(FILE *pf)
{
    BSTree *t;
    int nnodes = 0, i;
    Status st = OK;
    int x, y;
    char symbol;
    Point *p;

    if (!pf)
    {
        return NULL;
    }

    /* Read number of nodes */
    if (fscanf(pf, "%d\n", &nnodes) != 1)
    {
        return NULL;
    }

    /* Create tree */
    t = tree_init(point_print, point_cmpEuDistance);
    if (!t)
    {
        return NULL;
    }

    /* Read nodes */
    for (i = 0; i < nnodes && st == OK; i++)
    {
        if (fscanf(pf, "%d %d %c", &x, &y, &symbol) != 3)
        {
            return NULL;
        }
        p = point_new(x, y, symbol);
        if (!p)
        {
            tree_destroy(t);
            return NULL;
        }

        st = tree_insert(t, p);
        if (st == ERROR)
        {
            tree_destroy(t);
            point_free(p);
            return NULL;
        }
    }

    return t;
}