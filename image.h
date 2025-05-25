#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>
#include <stdlib.h>

typedef struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Pixel;

unsigned long read_num(FILE * image);
unsigned long image_width(FILE * image);
unsigned long image_height(FILE * image);
Pixel ** extract_image(FILE * image);

#endif