#include <stdio.h>     /* для printf */
#include <stdlib.h>    /* для exit */
#include <getopt.h>
#include <string.h>
#include <regex.h>

#define RGBREG "^([0-9]+)\\.([0-9]{1,3})\\.([0-9]{1,3})$"

#pragma pack (push, 1)

typedef struct {
    unsigned short signature : 16;
    unsigned int filesize : 32;
    unsigned short reserved1 : 16;
    unsigned short reserved2 : 16;
    unsigned int pixelArrOffset : 32;
} BitmapFileHeader;

typedef struct {
    unsigned int headerSize : 32;
    unsigned int width : 32;
    unsigned int height : 32;
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
    unsigned char b : 8;
    unsigned char g : 8;
    unsigned char r : 8;
} Rgb;

#pragma pack(pop) 

void print_error(char* message) {
    fprintf(stderr, "====ERROR====\n%s\nCommand for help: --help -h\n", message);
    exit(1);
}

void print_help() {
    printf("Some kind of HELP\n"); // инфо о командах
}

void freeMemory(BitmapFileHeader* bmfh, BitmapInfoHeader* bmif, Rgb **arr){
    for (int i = 0; i < bmif->height; i++) {
        free(arr[i]);
    }
    free(arr);
    free(bmfh);
    free(bmif);
}

void print_file_header(BitmapFileHeader header) {
    printf("signature:\t%x (%hu)\n", header.signature, header.signature);
    printf("filesize:\t%x (%u)\n", header.filesize, header.filesize);
    printf("reserved1:\t%x (%hu)\n", header.reserved1, header.reserved1);
    printf("reserved2:\t%x (%hu)\n", header.reserved2, header.reserved2);
    printf("pixelArrOffset:\t%x (%u)\n", header.pixelArrOffset, header.pixelArrOffset);
} 

// https://www.file-recovery.com/bmp-signature-format.htm

void print_info_header(BitmapInfoHeader header){
    printf("headerSize:\t%x (%u)\n", header.headerSize, header.headerSize);
    printf("width: \t%x (%u)\n", header.width, header.width); 
    printf("height: \t%x (%u)\n", header.height, header.height);
    printf("planes: \t%x (%hu)\n", header.planes, header.planes);
    printf("bitsPerPixel:\t%x (%hu)\n", header.bitsPerPixel, header.bitsPerPixel);
    printf("compression:\t%x (%u)\n", header.compression, header.compression);
    printf("imageSize:\t%x (%u)\n", header.imageSize, header.imageSize);
    printf("xPixelsPerMeter:\t%x (%u)\n", header.xPixelsPerMeter, header.xPixelsPerMeter);
    printf("yPixelsPerMeter:\t%x (%u)\n", header.yPixelsPerMeter, header.yPixelsPerMeter);
    printf("colorsInColorTable:\t%x (%u)\n", header.colorsInColorTable, header.colorsInColorTable);
    printf("importantColorCount:\t%x (%u)\n", header.importantColorCount, header.importantColorCount);
} 

void get_info(BitmapInfoHeader* bmif, BitmapFileHeader* bmfh)  {
    print_file_header(*bmfh);
    print_info_header(*bmif);
}

Rgb** read_bmp (char file_name[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmif){
    FILE* f = fopen(file_name, "rb");
    if (f == NULL) {
        char* mes = (char*)malloc((strlen("File doesn't exist!")+strlen(file_name))*sizeof(char));
        sprintf(mes, "File %s doesn't exist!", file_name);
        freeMemory(bmfh, bmif, NULL);
        print_error(mes);
    }
    fread(bmfh, 1, sizeof(BitmapFileHeader), f); 
    if (bmfh->signature != 19778) {
        print_error("Incorrect signature of BMP file!");
    }
    fread(bmif, 1, sizeof(BitmapInfoHeader), f);  
    /* if (bmif->bitsPerPixel != 8) {
        print_error("Pixel format must be equal to 8 (rgb-format)!");
    } */
    get_info(bmif, bmfh);
    unsigned int H = bmif->height;
    unsigned int W = bmif->width;
    Rgb** arr = (Rgb**)malloc(H*sizeof(Rgb*));
    for (int i = 0; i < H; i++) {
        arr[i] = (Rgb*)malloc(W*sizeof(Rgb) + (W*3)%4); 
        if (arr[i] == NULL) {
            print_error("Not enought memory!");
        }
        fread(arr[i], 1, W*sizeof(Rgb) + (W*3)%4, f);
    fclose(f);
    return arr;
    }
} 

void write_bmp(char file_name[], Rgb** arr, int H, int W, BitmapFileHeader bmfh, BitmapInfoHeader bmif) {
    FILE* ff = fopen(file_name, "wb");
    fwrite(&bmfh, 1, sizeof(BitmapFileHeader), ff);
    fwrite(&bmif, 1, sizeof(BitmapInfoHeader), ff);
    for (int i = 0; i < H; i++) {
        fwrite(arr[i], 1, W*sizeof(Rgb) + (W*3)%4, ff);
    }
    fclose(ff);
} 

void replace_color(int argc, char** argv, Rgb** arr, int H, int W) {
    char* regexString = RGBREG;
    size_t maxGroup = 4;
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroup];

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
        fprintf(stderr, "Can't compile regular expression");
        exit(0);
    }

    int c = 0;
    int option_index = 0;
    static struct option long_options[] = {
        {"old_color", 1, NULL, 'o'},
        {"new_color", 1, NULL, 'n'},
        {0, 0, 0, 0}
    };
    c = getopt_long(argc, argv, "o:n:", long_options, &option_index);

    Rgb old_color;
    Rgb new_color;

    int flag = 0;
    while (c != -1) {
        switch(c) {
            case 'o':
                if(regexec(&regexCompiled, optarg, maxGroup, groupArray, 0)){
                    regfree(&regexCompiled);
                    print_error("Your old_color must be a string in the format 'rrr.ggg.bbb'!");
                }

                unsigned short rgb[3]; 
                for (int i = 1; i < maxGroup; i++) {
                    char* tmp = (char*)malloc((groupArray[i].rm_eo - groupArray[i].rm_so + 1)*sizeof(char));
                    for (int j = groupArray[i].rm_so; j < groupArray[i].rm_eo; j++) 
                        tmp[j-groupArray[i].rm_so] = optarg[j];
                    tmp[groupArray[i].rm_eo - groupArray[i].rm_so] = '\0';
                    if (atoi(tmp) < 0 || atoi(tmp) > 255) {
                        print_error("Old color must be a number from 0 to 255!");
                    }
                    rgb[i-1] = atoi(tmp);
                    free(tmp);
                }
                old_color.r = rgb[0];
                old_color.g = rgb[1];
                old_color.b = rgb[2];
                flag += 1;
                break;
            case 'n':
                if(regexec(&regexCompiled, optarg, maxGroup, groupArray, 0)){
                    regfree(&regexCompiled);
                    print_error("Your old_color must be a string in the format 'rrr.ggg.bbb'!");
                }

                unsigned short rgb_n[3]; 
                for (int i = 1; i < maxGroup; i++) {
                    char* tmp = (char*)malloc((groupArray[i].rm_eo - groupArray[i].rm_so + 1)*sizeof(char));
                    for (int j = groupArray[i].rm_so; j < groupArray[i].rm_eo; j++) {
                        tmp[j-groupArray[i].rm_so] = optarg[j];
                    }
                    tmp[groupArray[i].rm_eo - groupArray[i].rm_so] = '\0';
                    if (atoi(tmp) < 0 || atoi(tmp) > 255) {
                        print_error("Old color must be a number from 0 to 255!");
                    }
                    rgb_n[i-1] = atoi(tmp);
                    free(tmp);
                }
                flag += 1;
                new_color.r = rgb_n[0];
                new_color.g = rgb_n[1];
                new_color.b = rgb_n[2];
                break;
            default:
                regfree(&regexCompiled);
                print_error("Unknown argument for function replace_color.");
        }
        if (flag == 2) {
            regfree(&regexCompiled);
            break;
        }
        c = getopt_long(argc, argv, "o:n:", long_options, &option_index);
    }
    if (flag < 2) 
        print_error("Not enought arguments for function replace_color!");

    int cr = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if ((arr[i][j].r == old_color.r) && (arr[i][j].g == old_color.g) && (arr[i][j].b == old_color.b)) {
                cr++;
                arr[i][j].r = new_color.r&255;
                arr[i][j].g = new_color.g&255;
                arr[i][j].b = new_color.b&255; // 48*48 
            }
        }
    }
}

int main (int argc, char **argv) {
    if (strstr(argv[argc-1], ".") == NULL)
        print_error("Please, input your filename as last argument.");
    if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".bmp"))
        print_error("Your file must be in .bmp extention");
    char* file_name = (char*)malloc((strlen(argv[argc-1]) + 1)*sizeof(char));
    strcpy(file_name, argv[argc-1]);
    file_name[strlen(argv[argc-1])] = '\0';
    BitmapFileHeader* bmfh = malloc(sizeof(BitmapFileHeader));
    BitmapInfoHeader* bmif = malloc(sizeof(BitmapInfoHeader));
    Rgb** arr = read_bmp(file_name, bmfh, bmif); // считывание файла 
    char* output_file;

    int c = 0;
    int option_index = 0;
    static struct option long_options[] = {
        {"color_replace", 1, NULL, 'c'},
        {"rgbfilter", 1, NULL, 'f'},
        {"split", 1, NULL, 's'},
        {"info", 0, NULL, 'i'},
        {"output", 1, NULL, 'o'}, // аргумент - output file
        {"help", 0, NULL, 'h'},
        {0, 0, 0, 0}
    };
    c = getopt_long(argc, argv, "cfsio:h", long_options, &option_index);
    /* optind - индекс текущего аргумента argv https://ru.manpages.org/getopt/3 */

    if (c == -1) {
        print_help();
        return 0;
    }

    while (c != -1) {
        switch (c) {
            case 'c':
                replace_color(argc, argv, arr, bmif->height, bmif->width);
                break;
            case 'i':
                get_info(bmif, bmfh);
                break;
            case 'h':
                print_help();
                break;
            case 'o':
                if (optarg == NULL || optarg[0] == '-' || optind == argc) {
                    print_error("Error"); // прописать текст error
                    break;
                }
                output_file = optarg;
                break;
            case '?':
                freeMemory(bmfh, bmif, arr);
                print_error("Found unknown command!");
        }
        c = getopt_long(argc, argv, "cfsio:h", long_options, &option_index);
    }

    if (optind < argc-1)
        print_error("Found unknown argument(-s)!");
    
    write_bmp("out.bmp", arr, bmif->height, bmif->width, *bmfh, *bmif);
    free(file_name);
    freeMemory(bmfh, bmif, arr);
    return 0;
}
