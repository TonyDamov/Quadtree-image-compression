#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "image.h"

#define ALLOC_ERROR(PTR) { \
    if(PTR == NULL) { \
        printf("\nError allocating memory."); \
        exit(1); \
    } \
}

typedef enum treestates {
    LEAF,
    BRANCH
} TreeState;

typedef union treedata {
    struct quadtreenode * child[4];
    Pixel pixel;
} TreeData;

typedef struct quadtreenode {
    long long id;
    TreeState state;
    /*
    Indexes of children and quadrants
    ---------
    | 0 | 1 |
    ---------
    | 3 | 2 |
    ---------
    */
    TreeData data;
    long long nx;
    long long px;
    long long py;
    long long ny;
} QuadtreeNode;

QuadtreeNode * init_node(Pixel ** pixels, long long nx, long long px, long long ny, long long py);
void split_tree(Pixel ** pixels , QuadtreeNode ** quadtree);
int is_similar_color(Pixel ** pixels, Pixel meancolor, QuadtreeNode quadtree);
void image_detail(Pixel ** pixels, QuadtreeNode ** quadtree);
QuadtreeNode * construct_quadtree(Pixel ** pixels, long long width, long long height);
void deconstruct_tree_helper(QuadtreeNode ** quadtree, Pixel ** pixels);
Pixel ** deconstruct_tree(QuadtreeNode ** quadtree);
void release_quadtree_helper(QuadtreeNode ** quadtree);
void release_quadtree(QuadtreeNode ** quadtree);

#endif