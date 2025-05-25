#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>
#include <stdlib.h>

typedef struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Pixel;

long long read_num(FILE * image);
long long image_width(FILE * image);
long long image_height(FILE * image);
Pixel ** import_image(FILE * image);
void export_image(FILE * image, Pixel ** pixels);

#endif