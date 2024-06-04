#include "headers.h"
#include "structs.h"
#include "BMPwork.h"

void print_not_enough_or_not_valid_args() {
    printf("Было введено недостаточное количество аргументов, либо данные не являются валидными. Работа программы принудительна завершена.\n");
    exit(-1);
}

void print_not_valid_args() {
    printf("Данные аргументов не являются валидными. Работа программы принудительна завершена.\n");
    exit(-1);
}

int check_color(int color) {
    return !(0 <= color && color <= 255);
}

int maxi(int a, int b){
    if (a > b) return a;
    else return b;
}
int mini(int a, int b){
    if (a < b) return a;
    else return b;
}

//drawpix
void paintPixel(BMP* bmp_data, int x, int y, RGB color){
    if (x < 0 || y < 0 || x >= bmp_data->BMPH_inf.width || y >= bmp_data->BMPH_inf.height){
        return;
    }
    else{
        bmp_data->img[y][x].b = color.b;
        bmp_data->img[y][x].g = color.g;
        bmp_data->img[y][x].r = color.r;
    }
}

//drawline
void drawLine_pr(BMP* bmp_data, int x1, int y1, int x2, int y2, int wid, RGB color){
    y1 = bmp_data->BMPH_inf.height - y1;
    y2 = bmp_data->BMPH_inf.height - y2;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx, sy;
    if (x1 < x2) sx = 1;
    else sx = -1;
    if (y1 < y2) sy = 1;
    else sy = -1;
    int err = dx - dy;
    int e2 = 0;
    while (1) {
        for (int i = 0; i < wid; i++) {
            for (int j = 0; j < wid; j++) {
                int x = x1 - (wid / 2) + i;
                int y = y1 - (wid / 2) + j;
                paintPixel(bmp_data, x, y, color);
            }
        }
        if (x1 == x2 && y1 == y2) {
            break;
        }
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawLine(BMP* bmp, arguments* args){
    int *x1 = &(args->x1);
    int *y1 = &(args->y1); 
    int *x2 = &(args->x2); 
    int *y2 = &(args->y2); 
    int *wid = &(args->width);
    int *c_r = &(args->color_r);
    int *c_b = &(args->color_b);
    int *c_g = &(args->color_g);
    RGB color = {*c_b, *c_g, *c_r};

    if (*x1 < 0 || *x2 < 0 || *y1 < 0 || *y2 < 0 || *wid <= 0){
        print_not_valid_args();
        exit(-1);
    }

    if (*x1 > bmp->BMPH_inf.width || *x2 > bmp->BMPH_inf.width || *y1 > bmp->BMPH_inf.height || *y2 > bmp->BMPH_inf.height){
        print_not_valid_args();
        exit(-1);
    }

    if (check_color(*c_b) || check_color(*c_g) || check_color(*c_r)){
        print_not_enough_or_not_valid_args();
        exit(-1);
    }

    drawLine_pr(bmp, *x1, *y1, *x2, *y2, *wid, color);
    *x1= -1; *x2 = -1; *y1 = -1; *y2 = -1; *wid = -1; *c_b = -1; *c_g = -1; *c_r = -1;
}
//triangle
void drawTriangle(BMP* bmp, int x1, int y1, int x2, int y2, int x3, int y3, int wid, RGB color){
    drawLine_pr(bmp, x1, y1, x2, y2, wid, color);
    drawLine_pr(bmp, x2, y2, x3, y3, wid, color);
    drawLine_pr(bmp, x1, y1, x3, y3, wid, color);
}

//fillTriangle
int checkPoint(int x, int y, int x1, int x2, int x3, int y1, int y2, int y3){
    int a1,a2,a3;
    a1 = (x1-x)*(y2-y1) - (x2-x1)*(y1-y);
    a2 = (x2-x)*(y3-y2) - (x3-x2)*(y2-y);
    a3 = (x3-x)*(y1-y3) - (x1-x3)*(y3-y);
    if ( (a1 < 0 && a2 < 0 && a3 < 0 ) || (a1 > 0 && a2 > 0 && a3 > 0)) return 1;
    return 0;
}

void fillTriangle(BMP* bmp, int x1, int y1, int x2, int y2, int x3, int y3, RGB fill){
    y1 = bmp->BMPH_inf.height - y1;
    y2 = bmp->BMPH_inf.height - y2;
    y3 = bmp->BMPH_inf.height - y3;
    int x_max = maxi(x1, maxi(x2,x3));
    int x_min = mini(x1, mini(x2,x3));
    int y_max = maxi(y1, maxi(y2,y3));
    int y_min = mini(y1, mini(y2,y3));
    for (int i = x_min; i <= x_max; i++){
        for (int j = y_min; j <= y_max; j++){
            if (checkPoint(i,j,x1,x2,x3,y1,y2,y3) == 1){
                paintPixel(bmp, i, j, fill);
            }
        }
    }
}

void drawTriangleFill(BMP* bmp, arguments* args){
    int* x1 = &(args->x1);
    int* y1 = &(args->y1); 
    int* x2 = &(args->x2);
    int* y2 = &(args->y2); 
    int* x3 = &(args->x3); 
    int* y3 = &(args->y3); 
    int* wid = &(args->width); 
    int *c_r = &(args->color_r);
    int *c_b = &(args->color_b);
    int *c_g = &(args->color_g);
    RGB color = {*c_b, *c_g, *c_r};
    int* pouring = &(args->pour);
    int *f_r = &(args->fill_r);
    int *f_b = &(args->fill_b);
    int *f_g = &(args->fill_g);
    RGB fill = {*f_b, *f_g, *f_r};

    if (*x1 < 0 || *x2 < 0 || *x3 < 0 || *y1 < 0 || *y2 < 0 || *y3 < 0 || (*pouring != 0 && *pouring != 1) || *wid <= 0){
        print_not_valid_args();
        exit(-1);
    }

    if (*x1 > bmp->BMPH_inf.width || *x2 > bmp->BMPH_inf.width || *x3 > bmp->BMPH_inf.width || *y1 > bmp->BMPH_inf.height || *y2 > bmp->BMPH_inf.height || *y3 > bmp->BMPH_inf.height){
        print_not_valid_args();
        exit(-1);
    }

    if (check_color(*c_b) || check_color(*c_g) || check_color(*c_r)){
        print_not_enough_or_not_valid_args();
        exit(-1);
    }

    if (*pouring == 1 && (check_color(*f_b) || check_color(*f_g) || check_color(*f_r))){
        print_not_enough_or_not_valid_args();
        exit(-1);
    }
    
    if (*pouring){
        fillTriangle(bmp, *x1, *y1, *x2, *y2, *x3, *y3, fill);
        drawTriangle(bmp, *x1, *y1, *x2, *y2, *x3, *y3, *wid, color);
    }
    else drawTriangle(bmp, *x1, *y1, *x2, *y2, *x3, *y3, *wid, color);
}
//crop
void cropimg(BMP* bmp, arguments* args){
    int* x1 = &(args->x1); 
    int *x2 = &(args->x2);
    int* y1 = &(args->y1);
    int* y2 = &(args->y2);
    
    if (*x1 < 0 || *x2 < 0 || *y1 < 0 || *y2 < 0 || *x1 > bmp->BMPH_inf.width || *x2 > bmp->BMPH_inf.width || *y1 > bmp->BMPH_inf.height || *y2 > bmp->BMPH_inf.height){
        print_not_valid_args();
        exit(-1);
    }

    int new_width = abs(*x1-*x2);
    int new_heigth = abs(*y1-*y2);
    int x_min = mini(*x1,*x2);
    *y1 = bmp->BMPH_inf.height - *y1  - 1;
    *y2 = bmp->BMPH_inf.height - *y2  - 1;
    int y_min = mini(*y1,*y2);

    BMP new_bmp = createBMP(new_width, new_heigth);

    int padding = (new_width * sizeof(RGB)) % 4;
    if (padding != 0) padding = 4 - padding;

    for (int i = 0; i < new_heigth; i++){
        memcpy(new_bmp.img[i], bmp->img[y_min + i] + x_min, sizeof(RGB)*new_width + padding);
    }
    freeBMP(bmp);
    (*bmp) = new_bmp;
    *x1 = -1; *x2 = -1; *y1 = -1; *y2 = -1;
}

//invert
int checkIntoCircle(int x, int y, int x0, int y0, int r){
    if ((x - x0)*(x - x0) + (y - y0)*(y - y0) < r*r) return 1;
    else return -1;
}

void invertPixel(BMP* bmp, int x, int y){
    RGB color;
    color.b = 255 - bmp->img[y][x].b;
    color.g = 255 - bmp->img[y][x].g;
    color.r = 255 - bmp->img[y][x].r;
    paintPixel(bmp, x, y, color);
}

void invertColorinCircle(BMP* bmp, arguments* args){
    int *k = &(args->k);
    if (*k){
        int* x1 = &(args->x1);
        int* x2 = &(args->x2);
        int* y1 = &(args->y1);
        int* y2 = &(args->y2);
        int x = abs(*x1-*x2)/2 + *x1;
        int y = abs(*y1-*y2)/2 + *y1;
        int r = abs(*x1-*x2)/2;
        if (x < 0 || y < 0 || x >= bmp->BMPH_inf.width || y >= bmp->BMPH_inf.height || r <= 0 || r > bmp->BMPH_inf.height || r > bmp->BMPH_inf.width){
            print_not_valid_args();
            exit(-1);
        }
        int x_n = x - r;
        int x_k = x + r;
        y = bmp->BMPH_inf.height - y;
        int y_n = y - r;
        int y_k = y + r;
        for (int i = x_n; i <= x_k; i++){
            for (int j = y_n; j <= y_k; j++){
                if (checkIntoCircle(i,j,x,y,r) ==  1){
                    invertPixel(bmp, i, j);
                }
            }   
        }

        *x1 = -1; *y1 = -1; *x2 = -1; *y2 = -1;
    }
    else{
        int* x = &(args->x);
        int* y = &(args->y);
        int* r = &(args->radius);
        if (*x < 0 || *y < 0 || *x >= bmp->BMPH_inf.width || *y >= bmp->BMPH_inf.height || *r <= 0 || *r > bmp->BMPH_inf.height || *r > bmp->BMPH_inf.width){
            print_not_valid_args();
            exit(-1);
        }
        int x1 = *x - *r;
        int x2 = *x + *r;
        *y = bmp->BMPH_inf.height - *y;
        int y1 = *y - *r;
        int y2 = *y + *r;
        
        for (int i = x1; i <= x2; i++){
            for (int j = y1; j <= y2; j++){
                if (checkIntoCircle(i,j,*x,*y,*r) ==  1){
                    invertPixel(bmp, i, j);
                }
            }
        }

        *x = -1; *y = -1; *r = -1;
    }
}
