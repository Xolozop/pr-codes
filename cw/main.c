#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Вопросы, непонятки: 
1) разница sizeof(Rgd) и 3 в arr[i] = (Rgb*)malloc(W*sizeof(Rgb) + (W*3)%4);

*/

#pragma pack (push, 1)

typedef struct {
    unsigned short signature;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixelArrOffset;
} BitmapFileHeader;

typedef struct {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BitmapInfoHeader;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Rgb;

#pragma pack(pop) // для корректного считывания, регулирует выравнивание структур
// ресурс: https://habr.com/ru/articles/142662/

Rgb** read_bmp (char file_name[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmif){
    FILE* f = fopen(file_name, "rb"); // режим побайтового чтения
    fread(bmfh, 1, sizeof(BitmapFileHeader), f); 
    fread(bmif, 1, sizeof(BitmapInfoHeader), f); // считывание заголовков
    unsigned int H = bmif->height;
    unsigned int W = bmif->width;
    Rgb** arr = (Rgb**)malloc(H * sizeof(Rgb*)); // создание двумерного массива пикселей (кол-во строк = высоте)
    for (int i = 0; i < H; i++){
        arr[i] = (Rgb*)malloc(W*sizeof(Rgb) + (W*3)%4); // создание W столбцов(?) пикселей для каждой строки
        // добавление (W*3)%4 из-за того, что в bmp выравнивание по 4 байтам
        fread(arr[i], 1, W*sizeof(Rgb) + (W*3)%4, f);
    }
    fclose(f);
    return arr;
} 

void drawPixelBorder(Rgb **arr, int H, int W, int x, int y, int border_width){
    // поиск координат левого верхнего угла
    int left = y - border_width;
    int high = x - border_width;
    // рисование рамки вокруг пикселя
    for (int i = high; i < x + border_width; i++){
        for (int j = left; j < y + border_width; j++){
            if (i >= 0 && j >= 0 && i < H && j < W){
                if (!(arr[i][j].r == 0 && arr[i][j].g == 0 && arr[i][j].b == 255)) {
                arr[i][j].r = 255;
                arr[i][j].g = 0;
                arr[i][j].b = 0;
                }
            }
        }
    }
} 

void drawBorder(Rgb **arr, int H, int W, int border_width){
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
        // проверяем, что пиксель синий
            if (arr[i][j].r == 0 && arr[i][j].g == 0 && arr[i][j].b == 255) {
                drawPixelBorder(arr, H, W, i, j, border_width); 
            }
        }
    }
}

void write_bmp(char file_name[], Rgb **arr, int H, int W, BitmapFileHeader bmfh, BitmapInfoHeader bmif) {
    FILE* ff = fopen(file_name, "wb");
    fwrite(&bmfh, 1, sizeof(BitmapFileHeader), ff);
    fwrite(&bmif, 1, sizeof(BitmapInfoHeader), ff);
    for (int i = 0; i < H; i++) {
        fwrite(arr[i], 1, W * sizeof(Rgb) + (W * 3) % 4, ff);
    }
    fclose(ff);
} 

void freeMemory(BitmapFileHeader* bmfh, BitmapInfoHeader* bmif, Rgb **arr){
    for (int i = 0; i < bmif->height; i++) {
        free(arr[i]);
    }
    free(arr);
    free(bmfh);
    free(bmif);
}

int main(int argc, char* argv[]) {
    char* opts = "bw?";
    // аргумент -b -w и какой-то в конце
    int opt;
    //opt = getopt(argc, argv, opts); - надо ли?
    // ресурс: https://www.opennet.ru/man.shtml?topic=getopt_long&category=3&russian=0

    int width = 0; // ширина границы для функции drawBorder
    int makeDraw = 0; // флаг для использования функции drawBorder
    BitmapFileHeader* bmfh = malloc(sizeof(BitmapFileHeader));
    BitmapInfoHeader* bmif = malloc(sizeof(BitmapInfoHeader));
    Rgb** arr = read_bmp("out.bmp", bmfh, bmif);
    unsigned int H = bmif->height;
    unsigned int W = bmif->width; // разбираем аргументы командной строки
    while(opt != -1) {
        switch(opt) {
            case 'b':
                makeDraw = 1;
                break;
            case 'w':
                if (optind < argc) {
                    // считываем ширину границы из строки опций
                    sscanf(argv[optind], "%d", &width);
                }
                break;
            case '?':
                freeMemory(bmfh, bmif, arr);
                return 0;
        }
        opt = getopt(argc, argv, opts);
    }
    if (makeDraw && width > 0) {
        drawBorder(arr, bmif->height, bmif->width, width);
    }
    write_bmp("blue_out.bmp", arr, H, W, *bmfh, *bmif);
    freeMemory(bmfh, bmif, arr);
    return 0;
}