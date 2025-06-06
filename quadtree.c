#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quadtree.h"

//Exagarated for the purpose of testing
#define COLOR_DIFF_TRESHOLD 24

int NODECOUNTER = 0;

//Counting the pixels of the image from (left;top) = (0;0) to (right; bottom) = (width; height)

QuadtreeNode * init_node(Pixel ** pixels, unsigned short nx, unsigned short px, unsigned short ny, unsigned short py) {
    QuadtreeNode * newnode = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
    ALLOC_ERROR(newnode)
    newnode->id = NODECOUNTER++;
    if(NODECOUNTER < 0) {
        printf("\nRan out of ids.");
        exit(1);
    }
    newnode->nx = nx;
    newnode->ny = ny;
    newnode->px = px;
    newnode->py = py;
    newnode->state = LEAF;

    int leafwidth = (newnode->px + 1) - newnode->nx;
    int leafheight = (newnode->ny + 1) - newnode->py;

    unsigned int pixelcount = leafheight * leafwidth;

    //find mean color
    unsigned long long pixelsumR = 0, pixelsumG = 0, pixelsumB = 0;
    for(int i = newnode->py; i <= newnode->ny; i++) {
        for(int j = newnode->nx; j <= newnode->px; j++) {
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

    unsigned short mwidth = floor(((*quadtree)->nx + (*quadtree)->px) / 2);
    unsigned short mheight = floor(((*quadtree)->ny + (*quadtree)->py) / 2);
    if((*quadtree)->nx < (*quadtree)->px && (*quadtree)->py < (*quadtree)->ny) {

        //top left
        QuadtreeNode * newnode1 = init_node(pixels, (*quadtree)->nx, mwidth, mheight, (*quadtree)->py);
        QuadtreeNode * newnode2 = init_node(pixels, mwidth + 1, (*quadtree)->px, mheight, (*quadtree)->py);
        QuadtreeNode * newnode3 = init_node(pixels, mwidth + 1, (*quadtree)->px, (*quadtree)->ny, mheight + 1);
        QuadtreeNode * newnode4 = init_node(pixels, (*quadtree)->nx, mwidth, (*quadtree)->ny, mheight + 1);

        (*quadtree)->data.child[0] = newnode1;
        (*quadtree)->data.child[1] = newnode2;
        (*quadtree)->data.child[2] = newnode3;
        (*quadtree)->data.child[3] = newnode4;
        (*quadtree)->state = BRANCH;
    }
    
    
}

int is_similar_color(Pixel ** pixels, Pixel meancolor, QuadtreeNode quadtree) {
    int is_similar_color = 1;
    for(int i = quadtree.py; i <= quadtree.ny && is_similar_color; i++) {
        for(int j = quadtree.nx; j <= quadtree.px && is_similar_color; j++) {
            double difference = abs(meancolor.R - pixels[i][j].R) + abs(meancolor.G - pixels[i][j].G) + abs(meancolor.B - pixels[i][j].B);
            if(difference > COLOR_DIFF_TRESHOLD) {
                is_similar_color = 0;
            }
        }
    }
    return is_similar_color;
}

void image_detail(Pixel ** pixels, QuadtreeNode ** quadtree) {
    if(!is_similar_color(pixels, (*quadtree)->data.pixel, **quadtree)) {
        split_tree(pixels, quadtree);
        if((*quadtree)->state == BRANCH) {
            image_detail(pixels, &(*quadtree)->data.child[0]);
            image_detail(pixels, &(*quadtree)->data.child[1]);
            image_detail(pixels, &(*quadtree)->data.child[2]);
            image_detail(pixels, &(*quadtree)->data.child[3]);
        }
    }
}

QuadtreeNode * construct_quadtree(Pixel ** pixels, unsigned short width, unsigned short height) {
    QuadtreeNode * quadtree = init_node(pixels, 0, width - 1, height - 1, 0);
    image_detail(pixels, &quadtree);
    return quadtree;
}

void deconstruct_tree_helper(QuadtreeNode ** quadtree, Pixel ** pixels) {
    if((*quadtree)->state == LEAF) {
        for(int i = (*quadtree)->py; i <= (*quadtree)->ny; i++) {
            for(int j = (*quadtree)->nx; j <= (*quadtree)->px; j++) {
                pixels[i][j].R = (*quadtree)->data.pixel.R;
                pixels[i][j].G = (*quadtree)->data.pixel.G;
                pixels[i][j].B = (*quadtree)->data.pixel.B;
            }
        }
    }
    else if((*quadtree)->state == BRANCH) {
        deconstruct_tree_helper(&(*quadtree)->data.child[0], pixels);
        deconstruct_tree_helper(&(*quadtree)->data.child[1], pixels);
        deconstruct_tree_helper(&(*quadtree)->data.child[2], pixels);
        deconstruct_tree_helper(&(*quadtree)->data.child[3], pixels);
    }
    else {
        printf("\nError. Unrecognized TreeState.");
        exit(1);
    }
}

Pixel ** deconstruct_tree(QuadtreeNode ** quadtree) {
    int width = (*quadtree)->px + 1, height = (*quadtree)->ny + 1;
    Pixel ** pixels = (Pixel**)malloc(sizeof(Pixel*) * height);
    ALLOC_ERROR(pixels)
    for(int i = 0; i < height; i++) {
        pixels[i] = (Pixel*)malloc(sizeof(Pixel) * width);
        ALLOC_ERROR(pixels[i])
    }
    
    deconstruct_tree_helper(quadtree, pixels);

    release_quadtree(quadtree);
    return pixels;
}

void release_quadtree_helper(QuadtreeNode ** quadtree){
    if((*quadtree)->state == LEAF) {
        return;
    }
    else if((*quadtree)->state == BRANCH) {
        release_quadtree_helper(&(*quadtree)->data.child[0]);
        free((*quadtree)->data.child[0]);
        release_quadtree_helper(&(*quadtree)->data.child[1]);
        free((*quadtree)->data.child[1]);
        release_quadtree_helper(&(*quadtree)->data.child[2]);
        free((*quadtree)->data.child[2]);
        release_quadtree_helper(&(*quadtree)->data.child[3]);
        free((*quadtree)->data.child[3]);
    }
    else {
        printf("\nError. Unrecognized TreeState.");
        exit(1);
    }
}

void release_quadtree(QuadtreeNode ** quadtree) {
    release_quadtree_helper(quadtree);
    free(*quadtree);
}
