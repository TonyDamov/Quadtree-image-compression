/*
TreeState is a bool: 0-leaf, 1-branch and will be saved in place of the
sign bit of the long long variable for id since ids can only be positive.
<TreeState><id><coordinates><(if leaf)pixel color>
If TreeState is branch it means the next node is going down into a deeper level.
*/
#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "codec.h"


void encode_helper(FILE * file, QuadtreeNode ** quadtree) {
    long long stateandid = 0;
    stateandid = (unsigned long long)stateandid | (*quadtree)->id;
    if((*quadtree)->state == BRANCH) {
        stateandid = stateandid | ((unsigned long long)1 << 63);
    }
    else {
        stateandid = stateandid & ((unsigned long long)0 << 63);
    }
    fwrite(&stateandid, sizeof(long long), 1, file);
    
    fwrite(&(*quadtree)->nx, sizeof(long long), 1, file);
    fwrite(&(*quadtree)->px, sizeof(long long), 1, file);
    fwrite(&(*quadtree)->ny, sizeof(long long), 1, file);
    fwrite(&(*quadtree)->py, sizeof(long long), 1, file);

    if((*quadtree)->state == LEAF) {
        fwrite(&(*quadtree)->data.pixel.R, sizeof(unsigned char), 1, file);
        fwrite(&(*quadtree)->data.pixel.G, sizeof(unsigned char), 1, file);
        fwrite(&(*quadtree)->data.pixel.B, sizeof(unsigned char), 1, file);
    }
    else if((*quadtree)->state == BRANCH) {
        encode_helper(file, &(*quadtree)->data.child[0]);
        encode_helper(file, &(*quadtree)->data.child[1]);
        encode_helper(file, &(*quadtree)->data.child[2]);
        encode_helper(file, &(*quadtree)->data.child[3]);
    }
    else {
        printf("\nError. Unrecognized TreeState.");
        exit(1);
    }
}

void encode(const char * filename, QuadtreeNode ** quadtree) {
    FILE * file = fopen(filename, "wb");
    FOPEN_ERROR(file)
    
    encode_helper(file, quadtree);
    
    fclose(file);
}

void decode_helper(FILE * file, QuadtreeNode ** node) {
    long long stateandid;
    fread(&stateandid, sizeof(long long), 1, file);
    (*node)->id = stateandid & ~((unsigned long long)1 << 63);
    (*node)->state = ((unsigned long long)stateandid & ((unsigned long long)1 << 63)) >> 63;
    
    fread(&(*node)->nx, sizeof(long long), 1, file);
    fread(&(*node)->px, sizeof(long long), 1, file);
    fread(&(*node)->ny, sizeof(long long), 1, file);
    fread(&(*node)->py, sizeof(long long), 1, file);

    if((*node)->state == LEAF) {
        fread(&(*node)->data.pixel.R, sizeof(unsigned char), 1, file);
        fread(&(*node)->data.pixel.G, sizeof(unsigned char), 1, file);
        fread(&(*node)->data.pixel.B, sizeof(unsigned char), 1, file);
    }
    else if((*node)->state == BRANCH) {
        QuadtreeNode *newnode1 = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
        ALLOC_ERROR(newnode1);
        (*node)->data.child[0] = newnode1;
        QuadtreeNode *newnode2 = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
        ALLOC_ERROR(newnode2);
        (*node)->data.child[1] = newnode2;
        QuadtreeNode *newnode3 = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
        ALLOC_ERROR(newnode3);
        (*node)->data.child[2] = newnode3;
        QuadtreeNode *newnode4 = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
        ALLOC_ERROR(newnode4);
        (*node)->data.child[3] = newnode4;

        decode_helper(file, &(*node)->data.child[0]);
        decode_helper(file, &(*node)->data.child[1]);
        decode_helper(file, &(*node)->data.child[2]);
        decode_helper(file, &(*node)->data.child[3]);
    }
    else {
        printf("\nError. Unrecognized TreeState.");
        exit(1);
    }
}

QuadtreeNode * decode(const char * filename) {
    FILE * file = fopen(filename, "rb");
    FOPEN_ERROR(file)
    QuadtreeNode * quadtree = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));
    ALLOC_ERROR(quadtree)

    decode_helper(file, &quadtree);

    fclose(file);
    return quadtree;
}
