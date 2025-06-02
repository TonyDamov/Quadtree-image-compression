#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "quadtree.h"
#include "codec.h"

void main() {
    char name[96] = "nightcity"; // no extension

    char file[128];
    strcpy(file, name);
    strcat(file, ".ppm");
    
    char binfile[128];
    strcpy(binfile, name);
    strcat(binfile, "_compressed.bin");
    
    char recoveredfile[128];
    strcpy(recoveredfile, name);
    strcat(recoveredfile, "_recovered.ppm");
    
    Pixel ** pixels = import_image(file);
    printf("\nImage successfully imported.");

    unsigned short width = image_width(file), height = image_height(file);
    printf("\nCompressing image. Resolution: %hux%hu", width, height);

    QuadtreeNode * quadtree = construct_quadtree(pixels, width, height);
    printf("\nTree successfully built.");

    encode(binfile, &quadtree);
    printf("\nImage successfully encoded.");

    QuadtreeNode * newtree = decode(binfile);
    printf("\nImage successfully decoded.");
    
    Pixel ** newpixels = deconstruct_tree(&newtree);
    export_image(recoveredfile, newpixels, quadtree->px + 1, quadtree->ny + 1);
    printf("\nImage sucessfully recovered.");
}
