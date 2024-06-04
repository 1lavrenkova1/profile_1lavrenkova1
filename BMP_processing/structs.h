#pragma once
#include "headers.h"

#pragma pack(push, 1)
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfBits;
} BMPHeaderFile;

typedef struct{
    unsigned int headerSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitperpixel;
    unsigned int compr;
    unsigned int imgSize;
    unsigned int x_pelspermetr;
    unsigned int y_pelspermetr;
    unsigned int clr_table;
    unsigned int clr_important;
} BMPHeaderInfo;

typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} RGB;

typedef struct{
    BMPHeaderFile BMPH;
    BMPHeaderInfo BMPH_inf;
    RGB **img;
    char *name;
} BMP;

typedef struct arguments{
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int x, y;
    int width;
    int color_r, color_g, color_b;
    int fill_r, fill_g, fill_b;
    int radius;
    int pour;
    char* output;
    int k;
} arguments;

#pragma pack(pop)