#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"


// Working with .ppm files since its contents as easiest to read
//.ppm saves the width and height of the image as a string which represents a number 
unsigned short read_num(FILE * image) {
    char buff[8];
    int i = 0;
    do {
        buff[i] = fgetc(image);
        i++;
    } while(buff[i-1] != 10 && buff[i-1] != 32 && i < sizeof(buff));
    buff[i-1] = '\0';
    return strtoll(buff, NULL, 10);
}

unsigned short image_width(const char * filename) {
    FILE * image = fopen(filename, "rb");
    FOPEN_ERROR(image)
    fseek(image, 3, SEEK_SET);
    unsigned short width = read_num(image);
    fclose(image);
    return width;
}

unsigned short image_height(const char * filename) {
    FILE * image = fopen(filename, "rb");
    FOPEN_ERROR(image)
    fseek(image, 3, SEEK_SET);
    unsigned short width = read_num(image);
    unsigned short height = read_num(image);
    fclose(image);
    return height;
}

Pixel ** import_image(const char * filename) {
    FILE * image = fopen(filename, "rb");
    FOPEN_ERROR(image)
    /*
    .ppm file structure:
    P6
    <width> <height>
    <max color value>
    <binary RGB data>
    */
    //Skip the beginning of the ppm file which holds the tag P6
    fseek(image, 3, SEEK_SET);
    unsigned short width = read_num(image);
    unsigned short height = read_num(image);
    unsigned short maxval = read_num(image);

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
    fclose(image);
    return pixels;
}

void export_image(const char * filename, Pixel ** pixels, unsigned short width, unsigned short height) {
    FILE * image = fopen(filename, "wb");
    FOPEN_ERROR(image)
    fputs("P6\n", image);
    char widthstr[64];
    char heightstr[64];
    char maxvalstr[64];
    snprintf(widthstr, 64, "%lld", width);
    snprintf(heightstr, 64, "%lld", height);
    snprintf(maxvalstr, 64, "%lld", 255);

    fprintf(image, "%s %s\n%s\n", widthstr, heightstr, maxvalstr);

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            fprintf(image, "%c%c%c", pixels[i][j].R, pixels[i][j].G, pixels[i][j].B);
        }
    }
    fclose(image);
}


