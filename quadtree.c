#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quadtree.h"
#include "image.c"

//Exagarated for the purpose of testing
#define COLOR_DIFF_TRESHOLD 384

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

int is_similar_color(Pixel ** pixels, Pixel meancolor, QuadtreeNode quadtree) {
    int is_similar_color = 1;
    for(int i = quadtree.py; i <= quadtree.ny && is_similar_color; i++) {
        for(int j = quadtree.nx; j <= quadtree.px && is_similar_color; j++) {
            int difference = abs(meancolor.R - pixels[i][j].R) + abs(meancolor.G - pixels[i][j].G) + abs(meancolor.B - pixels[i][j].B);
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
        image_detail(pixels, &(*quadtree)->data.child[0]);
        image_detail(pixels, &(*quadtree)->data.child[1]);
        image_detail(pixels, &(*quadtree)->data.child[2]);
        image_detail(pixels, &(*quadtree)->data.child[3]);
    }
}

QuadtreeNode * build_quadtree(Pixel ** pixel) {
    FILE * image = fopen("paint.ppm", "r");
    Pixel ** pixels = extract_image(image);
    QuadtreeNode * quadtree = init_node(pixels, 0, image_width(image) - 1, image_height(image) - 1, 0);
    fclose(image);
    image_detail(pixels, &quadtree);
    return quadtree;
}

} 