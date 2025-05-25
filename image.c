#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quadtree.h"
// Working with .ppm files since its contents as easiest to read
//.ppm saves the width and height of the image as a string which represents a number 
unsigned long read_num(FILE * image) {
    char buff[8];
    int i = 0;
    do {
        buff[i] = fgetc(image);
        i++;
    } while(buff[i-1] != 10 && buff[i-1] != 32 && i < sizeof(buff));
    buff[i-1] = '\0';
    return strtoul(buff, NULL, 10);
}

unsigned long image_width(FILE * image) {
    fseek(image, 3, SEEK_SET);
    unsigned long width = read_num(image);
    return width;
}

unsigned long image_height(FILE * image) {
    fseek(image, 3, SEEK_SET);
    unsigned long width = read_num(image);
    unsigned long height = read_num(image);
    return height;
}

Pixel ** extract_image(FILE * image) {
    /*
    .ppm file structure:
    P6
    <width> <height>
    <max color value>
    <binary RGB data>
    */
    //Skip the beginning of the ppm file which holds the tag P6
    fseek(image, 3, SEEK_SET);
    unsigned long width = read_num(image);
    unsigned long height = read_num(image);
    unsigned long maxval = read_num(image);

    Pixel ** pixels = (Pixel**)malloc(sizeof(Pixel*) * height);
    if(pixels == NULL) {
        printf("\n\nError allocating memory.");
        exit(1);
    }
    
    for(int i = 0; i < height; i++) {
        pixels[i] = (Pixel*)malloc(sizeof(Pixel) * width);
        if(pixels[i] == NULL) {
            printf("\n\nError allocating memory.");
            exit(1);
        }

        for(int j = 0; j < width; j++) {
            pixels[i][j].R = fgetc(image);
            pixels[i][j].G = fgetc(image);
            pixels[i][j].B = fgetc(image);
        }
    }
    return pixels;
}

