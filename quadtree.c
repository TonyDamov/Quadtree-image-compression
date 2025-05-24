#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quadtree.h"
#include "readimage.c"


//Counting the pixels of the image from (left;top) = (0;0) to (right; bottom) = (width; height)

QuadtreeNode * init_node(char * filename, Pixel ** pixels, long long nx, long long px, long long ny, long long py) {
    QuadtreeNode * newnode = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
    newnode->id = NODECOUNTER++;
    newnode->nx = nx;
    newnode->ny = ny;
    newnode->px = px;
    newnode->py = py;
    newnode->state = LEAF;

    long long leafwidth = (newnode->px + 1) - newnode->nx;
    long long leafheight = (newnode->ny + 1) - newnode->py;


    FILE * image = fopen(filename, "r");
    unsigned long long pixelcount = leafheight * leafwidth;
    fclose(image);

    //find mean color
    unsigned long long pixelsumR = 0, pixelsumG = 0, pixelsumB = 0;
    for(int i = 0; i < leafheight; i++) {
        for(int j = 0; j < leafwidth; j++) {
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

void split_tree(QuadtreeNode ** quadtree) {


}