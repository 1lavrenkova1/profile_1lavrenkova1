#pragma once
#include "structs.h"

BMP readBmp(const char* file_name);
void saveBmp(const char* file_name, BMP bmp_data);
void print_bmp_info(BMP* bmp_data);
BMP createBMP(size_t width, size_t height);
void freeBMP(BMP* bmp_file);