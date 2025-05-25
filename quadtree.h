#ifndef QUADTREE_H
#define QUADTREE_H

typedef struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Pixel;


typedef enum treestates {
    BRANCH,
    LEAF
} TreeState;

typedef union treedata {
    struct quadtreenode * child[4];
    Pixel pixel;
} TreeData;

long long NODECOUNTER = 0;

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

#endif