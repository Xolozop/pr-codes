#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>

#define MAX_LEN 1024
#define TEXT_BLOCK 100

void readText(const char* root, char** *files_data, int* p_cnt) {
    DIR* root_dir = opendir(root);
    if (!root_dir) {
        perror("Cannot open directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* dir;
    while (dir = readdir(root_dir)) {
        char path[MAX_LEN];
        sprintf(path, "%s/%s", root, dir->d_name);

        if (dir->d_type == DT_DIR) {
            if (strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0) {
                continue;
            }
            readText(path, files_data, p_cnt);
        }

        if (strstr(dir->d_name, "result.txt")) {
            continue;
        }

        if (dir->d_type == DT_REG && strstr(dir->d_name, ".txt")) {  
            FILE* file;
            file = fopen(path, "r");

            if (file) {
                int sizet = TEXT_BLOCK;
                char* str = malloc(sizet);
                char c = fgetc(file);
                int str_len = 0;
                int flag = 1;
                while (c != EOF) {
                    if (c == '\n') {
                        flag = 0;
                    }
                    str[str_len++] = c;
                    if (str_len == sizet-2) {
                        sizet += TEXT_BLOCK;
                        char* old_ptr = str;
                        str = (char*)realloc(str, sizet);
                        if (str == NULL) {
                            perror("Allocation memory error");
                            free(old_ptr);
                        }
                    }
                    c = fgetc(file);
                }
                str[str_len] = '\0';

                if (flag) 
                    (*files_data)[(*p_cnt)++] = str;
                *files_data = (char**)realloc(*files_data, (*p_cnt + 1)*sizeof(char*));
                fclose(file);
            }
        }
    }
    closedir(root_dir);
}

int cmp (const void* s1, const void* s2) {
    char** str_a = (char**)s1;
    char** str_b = (char**)s2;
    long a = atol(*str_a);
    long b = atol(*str_b);
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int main() {
    char root[MAX_LEN];
    if (!getcwd(root, sizeof(root))) {
        perror("Cannot copy path");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char** files_data = malloc(sizeof(char*));
    readText(root, &files_data, &count);

    qsort(files_data, count, sizeof(char*), cmp);
    
    char filepath[] = "/result.txt";
    strcat(root, filepath);
    FILE *file = fopen(root, "w+");
    for (int i = 0; i < count; i++) {
        if (i != 0) 
            fputs("\n", file);
        fprintf(file, "%s", files_data[i]);
        free(files_data[i]);
    }
    free(files_data);
    fclose(file);
    return 0;
}
