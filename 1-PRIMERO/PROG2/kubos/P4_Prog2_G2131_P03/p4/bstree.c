#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "bstree.h"

#define MAX_BUFFER 64 // Maximum file line size

/* START [_BSTNode] */
typedef struct _BSTNode
{
    void *info;
    struct _BSTNode *left;
    struct _BSTNode *right;
    struct _BSTNode *parent;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree
{
    BSTNode *root;
    P_tree_ele_print print_ele;
    P_tree_ele_cmp cmp_ele;
};
/* END [_BSTree] */

/*** BSTNode TAD private functions ***/
BSTNode *tree_find_min_rec(BSTNode *node);
BSTNode *tree_find_max_rec(BSTNode *node);
BSTNode *tree_insert_rec(BSTree *tree, BSTNode *node, const void *elem);
BSTNode *tree_contains_rec(P_tree_ele_cmp f, const void *elem, BSTNode *node);
BSTNode *tree_remove_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f);


BSTNode *_bst_node_new()
{
    BSTNode *pn = NULL;

    pn = malloc(sizeof(BSTNode));
    if (!pn)
    {
        return NULL;
    }

    pn->left = pn->right = NULL;
    pn->parent = NULL;
    pn->info = NULL;
    return pn;
}

void _bst_node_free(BSTNode *pn)
{
    if (!pn)
        return;
    if (pn->info)
        free(pn->info);
    free(pn);
}

void _bst_node_free_rec(BSTNode *pn)
{
    if (!pn)
        return;

    _bst_node_free_rec(pn->left);
    _bst_node_free_rec(pn->right);
    _bst_node_free(pn);

    return;
}

int _bst_depth_rec(BSTNode *pn)
{
    int depthR, depthL;
    if (!pn)
        return 0;

    depthL = _bst_depth_rec(pn->left);
    depthR = _bst_depth_rec(pn->right);

    if (depthR > depthL)
    {
        return depthR + 1;
    }
    else
    {
        return depthL + 1;
    }
}

int _bst_size_rec(BSTNode *pn)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_size_rec(pn->left);
    count += _bst_size_rec(pn->right);

    return count + 1;
}

int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += print_ele(pf, pn->info);
    count += _bst_preOrder_rec(pn->left, pf, print_ele);
    count += _bst_preOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_inOrder_rec(pn->left, pf, print_ele);
    count += print_ele(pf, pn->info);
    count += _bst_inOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_postOrder_rec(pn->left, pf, print_ele);
    count += _bst_postOrder_rec(pn->right, pf, print_ele);
    count += print_ele(pf, pn->info);

    return count;
}

/*** BSTree TAD functions ***/
BSTree *tree_init(P_tree_ele_print print_ele, P_tree_ele_cmp cmp_ele)
{
    if (!print_ele || !cmp_ele)
        return NULL;

    BSTree *tree = malloc(sizeof(BSTree));
    if (!tree)
    {
        return NULL;
    }

    tree->root = NULL;
    tree->print_ele = print_ele;
    tree->cmp_ele = cmp_ele;

    return tree;
}

void tree_destroy(BSTree *tree)
{
    if (!tree)
        return;

    _bst_node_free_rec(tree->root);

    free(tree);
    return;
}

Bool tree_isEmpty(const BSTree *tree)
{
    if (!tree || !tree->root)
        return TRUE;
    return FALSE;
}

int tree_depth(const BSTree *tree)
{
    if (!tree)
        return -1;

    return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree *tree)
{
    if (!tree)
        return -1;

    return _bst_size_rec(tree->root);
}

int tree_preOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

void *tree_find_min(BSTree *tree)
{
    BSTNode *aux = NULL;

    if (!tree)
    {
        return NULL;
    }

    aux = tree_find_min_rec(tree->root);
    if (!aux)
    {
        return NULL;
    }

    return aux->info;
}

BSTNode *tree_find_min_rec(BSTNode *node)
{
    if (!node)
    {
        return NULL;
    }

    if (!node->left)
    {
        return node;
    }

    return tree_find_min_rec(node->left);
}

void *tree_find_max(BSTree *tree)
{
    BSTNode *aux = NULL;

    if (!tree)
    {
        return NULL;
    }

    aux = tree_find_max_rec(tree->root);
    if (!aux)
    {
        return NULL;
    }

    return aux->info;
}

BSTNode *tree_find_max_rec(BSTNode *node)
{
    if (!node)
    {
        return NULL;
    }

    if (!node->right)
    {
        return node;
    }

    return tree_find_max_rec(node->right);
}

Status tree_insert(BSTree *tree, const void *elem)
{
    BSTNode *aux = NULL;
    BSTNode *n = NULL;

    if (!tree || !elem)
    {
        return ERROR;
    }

    if (tree_contains(tree, elem))
    {
        return OK;
    }

    aux = tree->root;

    n = tree_insert_rec(tree, aux, elem);
    if (!n)
    {
        return ERROR;
    }

    tree->root = n;

    return OK;
}

BSTNode *tree_insert_rec(BSTree *tree, BSTNode *node, const void *elem)
{
    BSTNode *aux = NULL;
    int dst = 0;

    if (!node)
    {
        node = _bst_node_new();
        if (!node)
        {
            return NULL;
        }
        node->info = (void*) elem;
        return node;
    }

    dst = tree->cmp_ele(node->info, elem);
    if (dst == INT_MIN)
    {
        return NULL;
    }

    if (dst == 1)
    {
        aux = tree_insert_rec(tree, node->left, elem);
        if (!aux)
        {
            return NULL;
        }
        node->left = aux;
        aux->parent = node;
    }
    if (dst == -1)
    {
        aux = tree_insert_rec(tree, node->right, elem);
        if (!aux)
        {
            return NULL;
        }
        node->right = aux;
        aux->parent = node;
    }

    return node;
}

Bool tree_contains(BSTree *tree, const void *elem)
{
    BSTNode *aux = NULL;

    if (!tree || !elem)
    {
        return FALSE;
    }

    aux = tree_contains_rec(tree->cmp_ele, elem, tree->root);
    if (!aux)
    {
        return FALSE;
    }

    return TRUE;
}

BSTNode *tree_contains_rec(P_tree_ele_cmp f, const void *elem, BSTNode *node)
{
    if (!f || !elem || !node)
    {
        return NULL;
    }

    if (f(node->info, elem) == 0)
    {
        return node;
    }
    if (f(node->info, elem) < 0)
    {
        
        return tree_contains_rec(f, elem, node->right);
    }

    return tree_contains_rec(f, elem, node->left);
}

Status tree_remove(BSTree *tree, const void *elem)
{
    BSTNode* aux = NULL;
    
    if (!tree || !elem)
    {
        return ERROR;
    }

    if(tree_contains(tree, elem) == FALSE){
        return OK;
    }

    aux = tree_remove_rec(tree->root, elem, tree->cmp_ele);
    if (!aux) {
        return ERROR;
    }

    return OK;
}

BSTNode *tree_remove_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f)
{
    BSTNode *aux = NULL;

    if (!f || !elem || !node)
    {
        return NULL;
    }

    if (f(node->info, elem) > 0)
    {
        node->left = tree_remove_rec(node->left, elem, f);
    }
    else if (f(node->info, elem) < 0)
    {
        node->right = tree_remove_rec(node->right, elem, f);
    }
    else if (f(node->info, elem) == 0)
    {
        if (!node->right && !node->left)
        {
            _bst_node_free(node);
            return NULL;
        }
        if (node->right && !node->left)
        {
            aux = node->right;
            _bst_node_free(node);
            return aux;
        }
        if (!node->right && node->left)
        {
            aux = node->left;
            _bst_node_free(node);
            return aux;
        }
        else
        {
            aux = tree_find_min_rec(node->right);
            if (!aux)
            {
                return NULL;
            }
            node->info = aux->info;
            node->right = tree_remove_rec(node->right, aux->info, f);
            if (!node->right)
            {
                return NULL;
            }
            return node;
        }
    }

    return node;
}

/**** TODO: find_min, find_max, insert, contains, remove ****/