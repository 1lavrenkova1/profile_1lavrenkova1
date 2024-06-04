#pragma once
#include "headers.h"
#include "BMPwork.h"

int maxi(int a, int b);
int mini(int a, int b);
void paintPixel(BMP* bmp_data, int x, int y, RGB color);
void drawLine_pr(BMP* bmp_data, int x1, int y1, int x2, int y2, int wid, RGB color);
void drawLine(BMP* bmp_data, arguments* args);
void drawTriangle(BMP* bmp, int x1, int y1, int x2, int y2, int x3, int y3, int wid, RGB color);
int checkPoint(int x, int y, int x1, int x2, int x3, int y1, int y2, int y3);
void fillTriangle(BMP* bmp, int x1, int y1, int x2, int y2, int x3, int y3, RGB fill);
void drawTriangleFill(BMP* bmp, arguments* args);
void cropimg(BMP* bmp, arguments* args);
int checkIntoCircle(int x, int y, int x0, int y0, int r);
void invertPixel(BMP* bmp, int x, int y);
void invertColorinCircle(BMP* bmp, arguments* args);