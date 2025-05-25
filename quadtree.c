#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quadtree.h"
#include "readimage.c"


//Counting the pixels of the image from (left;top) = (0;0) to (right; bottom) = (width; height)

QuadtreeNode * init_node(Pixel ** pixels, long long nx, long long px, long long ny, long long py) {
    QuadtreeNode * newnode = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
    newnode->id = NODECOUNTER++;
    newnode->nx = nx;
    newnode->ny = ny;
    newnode->px = px;
    newnode->py = py;
    newnode->state = LEAF;

    long long leafwidth = (newnode->px + 1) - newnode->nx;
    long long leafheight = (newnode->ny + 1) - newnode->py;

    unsigned long long pixelcount = leafheight * leafwidth;

    //find mean color
    unsigned long long pixelsumR = 0, pixelsumG = 0, pixelsumB = 0;
    for(int i = newnode->py; i < newnode->ny; i++) {
        for(int j = newnode->nx; j < newnode->px; j++) {
            pixelsumR += pixels[i][j].R;
            pixelsumG += pixels[i][j].G;
            pixelsumB += pixels[i][j].B;
        }
    }
    newnode->data.pixel.R = pixelsumR / pixelcount;
    newnode->data.pixel.G = pixelsumG / pixelcount;
    newnode->data.pixel.B = pixelsumB / pixelcount;

    return newnode;
}

void split_tree(Pixel ** pixels , QuadtreeNode ** quadtree) {
    //using floor to prevent non integers
    /*
    Ex. Odd number of boxes - newnode1 => px = floor((0+4)/2) = 2; newnode2 => nx = floor((0+4)/2) + 1 = 3
    ========================
    || 0 | 1 | 2 || 3 | 4 ||
    ------------------------
    ||   |   |   ||   |   ||
    ------------------------
    ||   |   |   ||   |   ||
    ========================
    |    |   |    |   |    |
    ------------------------

    Ex. Even number of boxes - newnode1 => px = floor((0+3)/2) = 1; newnode2 => nx = floor((o+3)/2) + 1 = 2;
    ====================
    || 0 | 1 || 2 | 3 ||
    --------------------
    ||   |   ||   |   ||
    ====================
    |    |    |   |    |
    --------------------
    |    |    |   |    |
    --------------------
    */

    long long mwidth = floor(((*quadtree)->nx + (*quadtree)->px) / 2);
    long long mheight = floor(((*quadtree)->ny + (*quadtree)->py) / 2);
    //top left
    QuadtreeNode * newnode1 = init_node(pixels, (*quadtree)->nx, mwidth, mheight, (*quadtree)->py);
    QuadtreeNode * newnode2 = init_node(pixels, mwidth + 1, (*quadtree)->px, mheight, (*quadtree)->py);
    QuadtreeNode * newnode3 = init_node(pixels, mwidth + 1, (*quadtree)->px, (*quadtree)->ny, mheight + 1);
    QuadtreeNode * newnode4 = init_node(pixels, (*quadtree)->nx, mwidth, (*quadtree)->ny, mheight + 1);

    (*quadtree)->state = BRANCH;
    (*quadtree)->data.child[0] = newnode1;
    (*quadtree)->data.child[1] = newnode2;
    (*quadtree)->data.child[2] = newnode3;
    (*quadtree)->data.child[3] = newnode4;
}
