#ifndef CODEC_H
#define CODEC_H
#include "quadtree.h"

void encode_helper(FILE * file, QuadtreeNode ** quadtree);
void encode(const char * filename, QuadtreeNode ** quadtree);
void decode_helper(FILE * file, QuadtreeNode ** node);
QuadtreeNode * decode(const char * filename);

#endif