#include "headers.h"
#include "structs.h"
#include "BMPwork.h"
#include "functions.h"

#pragma pack(push, 1)
const struct option long_options[] = {
    {"invert", no_argument, NULL, 'I'},
    {"crop", no_argument, NULL, 'C'},
    {"triangle", no_argument, NULL, 'T'},
    {"line", no_argument, NULL, 'L'},

    {"first", required_argument, NULL, 'f'},
    {"second", required_argument, NULL, 's'},
    {"third", required_argument, NULL, 't'},
    {"ring", required_argument, NULL, 'R'},
    {"radius", required_argument, NULL, 'r'},
    {"width", required_argument, NULL, 'w'},
    {"color", required_argument, NULL, 'c'},
    {"pouring", required_argument, NULL, 'p'},
    {"fill", required_argument, NULL, 'F'},
    {"square", required_argument, NULL, 'k'},

    {"output", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'},
    {"info", no_argument, NULL, 'i'},
    {NULL, no_argument, NULL, 0}
};
#pragma pack(pop)

void print_help(){
    printf("Руководство по использованию программы:\n");
    printf("Программа обрабатывает BMP-файлы версии BMP3. С глубиной изображения 24 бита на пиксель\n");
    printf("Формат ввода: [имя исполняемого файла] [имя BMP-файла для обработки] [функция] -[ключ]/--[полный ключ] [аргументы и их ключи]...\n");
    printf("Функции/ключи:\n");

    printf("-I/--invert -- инвертирование цвета в заданной окружности. \n");
    printf("\t-k/--square -- [1(true)/0(false)] - задана окружность квадратом(true) или задана центром и радиусом окружности(false).\n ");
    printf("\t-f/--first  [<x-координата>.<y-координата>] - координата левого верхнего угла квадрата.\n");
    printf("\t-s/--second [<x-координата>.<y-координата>] - координата правого нижнего угла квадрата.\n");
    printf("\t-R/--ring [<х - координата>.<y - координата>] - координаты цента окружности.\n");
    printf("\t-r/--radius [число]- длина радиуса окружности.\n");
    

    printf("-C/--crop -- обрезка изоображения по заданной области.\n");
    printf("\t-f/--first  [<x-координата>.<y-координата>] - координата левого верхнего угла области.\n");
    printf("\t-s/--second [<x-координата>.<y-координата>] - координата правого нижнего угла области.\n");

    printf("-T/--triangle -- рисование треугольника.\n");
    printf("\t-f/--first  [<x-координата>.<y-координата>] - координата первой вершины.\n");
    printf("\t-s/--second [<x-координата>.<y-координата>] - координата второй вершины.\n");
    printf("\t-t/--third [<x-координата>.<y-координата>] - координата третьей вершины.\n");
    printf("\t-w/--width - [число] толщина линий треугольника.\n");
    printf("\t-c/--color - [число.число.число] цвет линий.\n");
    printf("\t-p/--pouring [1(true)/0(false)] - залит треугольник или нет\n");
    printf("\t-F/--fill - [число.число.число] цвет заливки треугольника.\n");

    printf("-L/--line -- рисование отрезка.\n");
    printf("\t-f/--first  [<x-координата>.<y-координата>] - координата начала отрезка.\n");
    printf("\t-s/--second [<x-координата>.<y-координата>] - координата конца отрезка.\n");
    printf("\t-w/--width [число] - толщина линии.\n");
    printf("\t-c/--color [число.число.число] - цвет линии.\n");

    printf("-h/--help -- справка о работе программы.\n");
    printf("-i/--info -- информация о bmp-файле.\n");
    printf("-o/--output -- файл для записи полученного изображения.\n");
}

arguments* set_null(arguments* args){
    args->color_b = -1; args->color_g = -1; args->color_r = -1;
    args->fill_b = -1; args->fill_g = -1; args->fill_r = -1;
    args->pour = -1;
    args->k = -1;
    args->radius = -1;
    args->width = -1;
    args->x1 = -1; args->x2 = -1; args->x3 = -1;
    args->y1 = -1; args->y2 = -1; args->y3 = -1;
    args->x = -1; args->y = -1;
    args->output = NULL;
    return args;
}

void parse_str(int** arr, int count, char* optarg){
    char* istr = strtok(optarg, ".");
    *(arr[0]) = atoi(istr);
    for (int i = 1; i < count; i++){
        istr = strtok(NULL, ".");
        if (istr != NULL) *(arr[i]) = atoi(istr);
        else {
            printf("Введено надостаточно аргументов для реализации функции. Работа программы завершена.\n");
            exit(-1);
        }   
    }
}

arguments* choice(arguments *args, int opt){
    int** arr = malloc(3*sizeof(int*));

    switch (opt){
        case 'R':
            arr[0] = &(args->x);
            arr[1] = &(args->y);
            parse_str(arr, 2, optarg);
            break;
        case 'f':
            arr[0] = &(args->x1);
            arr[1] = &(args->y1);
            parse_str(arr, 2, optarg);
            break;
        case 's':
            arr[0] = &(args->x2);
            arr[1] = &(args->y2);
            parse_str(arr, 2, optarg);
            break;
        case 't':
            arr[0] = &(args->x3);
            arr[1] = &(args->y3);
            parse_str(arr, 2, optarg);
            break;
        case 'w':
            args->width = atoi(optarg);
            break;
        case 'r':
            args->radius = atoi(optarg);
            break;
        case 'p':
            args->pour = atoi(optarg);
            break;
        case 'c':
            arr[0] = &(args->color_b);
            arr[1] = &(args->color_g);
            arr[2] = &(args->color_r);
            parse_str(arr, 3, optarg);
            break;
        case 'F':
            arr[0] = &(args->fill_b);
            arr[1] = &(args->fill_g);
            arr[2] = &(args->fill_r);
            parse_str(arr, 3, optarg);
            break;
        case 'o':
            args->output = malloc(strlen(optarg)+1);
            strcpy(args->output, optarg);
            break;
        case 'h':
            print_help();
            break;
        case 'k':
            args->k = atoi(optarg);
            break;
        default:
            break;
        }
    free(arr);
    return args;
}

BMP func_choice(BMP file, int opt, int prev_opt, arguments* args){
    if (opt == 'I' || opt == 'L' || opt == 'C' || opt == 'T'){
        switch (prev_opt){
        case 'I':
            invertColorinCircle(&file, args);
            break;
        case 'L':
            drawLine(&file, args);
            break;
        case 'C':
            cropimg(&file, args);
            break;
        case 'T':
            drawTriangleFill(&file, args);
            break;
        default:
            break;
        }
    }
    return file;
}

void cli(int argc, char** argv){
    char *opts = "ICTLf:s:t:k:R:r:w:c:p:F:hio:";
    arguments *args = malloc(sizeof(arguments));
    args = set_null(args);
    int indexes;
    char* file_name = argv[1];
    int opt;
    int prev_opt = 'N';

    if (argc == 1){
        print_help();
        exit(-1);
    }

    BMP bmp_file = readBmp(file_name);
    args->output = file_name;

    opt = getopt_long(argc, argv, opts, long_options, &indexes);
    while (opt != -1){
        bmp_file = func_choice(bmp_file, opt, prev_opt, args);
        if (opt == 'I' || opt == 'L' || opt == 'C' || opt == 'T') prev_opt = opt;
        if (opt == 'i') print_bmp_info(&bmp_file);

        args = choice(args ,opt);

        opt = getopt_long(argc, argv, opts, long_options, &indexes);
    }
    bmp_file = func_choice(bmp_file, 'I', prev_opt, args);
    saveBmp(args->output, bmp_file);
}