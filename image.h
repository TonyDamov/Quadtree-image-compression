#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>

#define FOPEN_ERROR(FILE) { \
    if(FILE == NULL) { \
        printf("\nError opening file."); \
        exit(1); \
    } \
} 

typedef struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Pixel;

long long read_num(FILE * image);
long long image_width(const char * filename);
long long image_height(const char * filename);
Pixel ** import_image(const char * filename);
void export_image(const char * filename, Pixel ** pixels, long long width, long long height);

#endif