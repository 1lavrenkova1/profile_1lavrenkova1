#pragma once
#include "structs.h"

void print_help();
arguments* set_null(arguments* args);
void parse_str(int** arr, int count, char* optarg);
arguments* choice(arguments *args, int opt);
void func_choice(BMP* file, int opt, int prev_opt, arguments* args);
void cli(int argc, char** argv);
