#include "headers.h"
#include "structs.h"
#include "BMPwork.h"

BMP readBmp(const char* file_name){
    FILE* fp = fopen(file_name, "rb");

    if(!fp){
        printf("Не удалось открыть файл для выполения.\n");
        exit(-1);
    }

    BMP bmp_data;
    fread(&bmp_data.BMPH, 1, sizeof(bmp_data.BMPH), fp);
    fread(&bmp_data.BMPH_inf, 1, sizeof(bmp_data.BMPH_inf), fp);

    if(bmp_data.BMPH_inf.bitperpixel != 24){
        printf("Программа не обрабатывает файлы, у которых не выделено 24 бита на цвет.\n");
        exit(-1);
    }

    size_t height = bmp_data.BMPH_inf.height;
    size_t width = bmp_data.BMPH_inf.width;

    int padding = (width * sizeof(RGB)) % 4;
    if (padding != 0) padding = 4 - padding;

    bmp_data.img = (RGB**)malloc(sizeof(RGB*)*height);
    for (size_t i = 0; i < height; i++){
        bmp_data.img[i] = (RGB*)malloc(sizeof(RGB)*width + padding);
        fread(bmp_data.img[i], 1, sizeof(RGB)*width +padding, fp);
    }

    fclose(fp);
    return bmp_data;
}
//save
void saveBmp(const char* file_name, BMP bmp_data){
    FILE* fp = fopen(file_name, "wb");
    fwrite(&bmp_data.BMPH, 1, sizeof(bmp_data.BMPH), fp);
    fwrite(&bmp_data.BMPH_inf, 1, sizeof(bmp_data.BMPH_inf), fp);
    
    size_t width = bmp_data.BMPH_inf.width;
    size_t height = bmp_data.BMPH_inf.height;

    int padding = (width * sizeof(RGB)) % 4;
    if (padding != 0) padding = 4 - padding;

    for (size_t i = 0; i < height; i++){
        fwrite(bmp_data.img[i], 1, sizeof(RGB)*width + padding, fp);
    }
    fclose(fp);
}
void print_bmp_info(BMP* bmp_data){
    printf("Информация о bmp-файле:\n");
    printf("file type = \t%hi\n", bmp_data->BMPH.bfType);
    printf("file size = \t%hi\n", bmp_data->BMPH.bfSize);
    printf("reserved1 = \t%hi\n", bmp_data->BMPH.bfReserved1);
    printf("reserved2 = \t%hi\n", bmp_data->BMPH.bfReserved2);
    printf("size of bits = \t%hi\n", bmp_data->BMPH.bfBits);
    printf("bit per pixel = \t%hi\n", bmp_data->BMPH_inf.bitperpixel);
    printf("size of header = \t%hi\n", bmp_data->BMPH_inf.headerSize);
    printf("height = \t%d\n", bmp_data->BMPH_inf.height);
    printf("width = \t%d\n", bmp_data->BMPH_inf.width);
    printf("size of img = \t%hi\n", bmp_data->BMPH_inf.imgSize);
    printf("important color count = \t%hi\n", bmp_data->BMPH_inf.clr_important);
    printf("colors in color table = \t%hi\n", bmp_data->BMPH_inf.clr_table);
    printf("compression = \t%hi\n", bmp_data->BMPH_inf.compr);
    printf("planes = \t%hi\n", bmp_data->BMPH_inf.planes);
    printf("x pixels per meter = \t%hi\n", bmp_data->BMPH_inf.x_pelspermetr);
    printf("y pixels per meter = \t%hi\n", bmp_data->BMPH_inf.y_pelspermetr);
    
}

BMP createBMP(size_t width, size_t height){
    BMP bmp_data;
    //file
    int padding = (width * sizeof(RGB)) % 4;
    if (padding != 0) padding = 4 - padding;
    bmp_data.BMPH.bfType = 0x4D42;
    bmp_data.BMPH.bfSize = height*(width*sizeof(RGB) + padding) + sizeof(BMPHeaderFile)+sizeof(BMPHeaderInfo);
    bmp_data.BMPH.bfReserved1 = 0;
    bmp_data.BMPH.bfReserved2 = 0;
    bmp_data.BMPH.bfBits = sizeof(BMPHeaderFile)+sizeof(BMPHeaderInfo);

    //info
    bmp_data.BMPH_inf.headerSize = 40;
    bmp_data.BMPH_inf.width = width;
    bmp_data.BMPH_inf.height = height;
    bmp_data.BMPH_inf.planes = 1;
    bmp_data.BMPH_inf.bitperpixel = sizeof(RGB)*8;
    bmp_data.BMPH_inf.compr = 0;
    bmp_data.BMPH_inf.imgSize = height*(width*sizeof(RGB) + padding);
    bmp_data.BMPH_inf.x_pelspermetr = 0;
    bmp_data.BMPH_inf.y_pelspermetr = 0;
    bmp_data.BMPH_inf.clr_important = 0;
    bmp_data.BMPH_inf.clr_table = 0;

    bmp_data.img = (RGB**)malloc(sizeof(RGB*)*height);
    for (size_t i = 0; i < height; i++){
        bmp_data.img[i] = (RGB*)malloc(sizeof(RGB)*width + padding);
        for (size_t j = 0; j < width; j++){
            bmp_data.img[i][j].b = 255;
            bmp_data.img[i][j].g = 255;
            bmp_data.img[i][j].r = 255;
        }
    }
    return bmp_data;
}

void freeBMP(BMP* bmp_file) {
    if (bmp_file) {
        for(int i = 0; i < bmp_file->BMPH_inf.height; i++) {
            free(bmp_file->img[i]);
        }
        free(bmp_file->img);
    }
}