#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#define MAX_PATH 512
#define MAX_FILENAME 256

int file_validator(char* filenmae) {
    int res = 1;
    //...
    // regex 
    // str* func
    return res;
}

int dir_validator(char* dirname) {
    int res = 1;
    if (!(strcmp(dirname, ".") && strcmp(dirname, "..")))
    /* скипаем "ссылку" на корень и текущую директорию */
        res = 0;
    //...
    // regex 
    // str* func
    return res;
}

void read_file(char* filepath) {
    FILE* f = fopen(filepath, "r");
    printf("FILE: [%s]\n", filepath);
    if (f == NULL)
        return;
    // fgets fscanf sscanf
    fclose(f);
}

void dir_lookup(char* root){
    char tmp_dir[MAX_PATH+2]; /* размер имён двух файлов (256*2) + один слеш + один \0. нужно для будущих преобразований */
    strncpy(tmp_dir, root, MAX_FILENAME); 

    DIR* root_dir = opendir(root); /* открывает (заходит в) поток директории и возвращает указатель на него */
    if (root_dir == NULL)
        return; /* выходим, если поток не удалось открыть */

    struct dirent* dir = readdir(root_dir); /* возвращает указатель на структуру с инфо о первом элементе текущей дирректории */

    while(dir) { /* пока в текущей директории возможно считать элементы */
        if (dir->d_type == DT_REG && file_validator(dir->d_name)) { /* если элемент - файл с валидным именем */

            // build_path(d_name, max, *tmp_dir, *) 

            if (strlen(dir->d_name) > MAX_FILENAME) {
                perror("Слишком длинное имя файла");
                exit(0);
            }

            strncat(tmp_dir, "/", strlen(tmp_dir)+1); 
            strncat(tmp_dir, dir->d_name, strlen(dir->d_name)); /* формируем путь вида <директория>/<текущий файл> */
            read_file(tmp_dir); /* открываем файлик и выполняем с ним какие-то действия */

            tmp_dir[strlen(tmp_dir) -1 - strlen(dir->d_name)] = '\0'; /* обнуляем строку, в которой хранится имя текущей директории */
            // таким образом считаем сначала все файлы в рамках одной директории
        } else if (dir->d_type == DT_DIR && dir_validator(dir->d_name)) {
            if (strlen(dir->d_name) > MAX_FILENAME) {
                perror("Слишком длинное имя файла");
                exit(0);
            }
            
            strncat(tmp_dir, "/", strlen(tmp_dir)+1);
            strncat(tmp_dir, dir->d_name, strlen(dir->d_name));
            printf("DIR: [%s]\n", tmp_dir);

            dir_lookup(tmp_dir); /* "углубляемся в дерево", вызываем нашу функцию для вложенной директории */

            tmp_dir[strlen(tmp_dir) -1 - strlen(dir->d_name)] = '\0';
        }
        dir = readdir(root_dir); /* переходим к следующему элемменту */
    }
    closedir(root_dir); /* закрываем поток */
}

int main(){
    dir_lookup("."); /* поиск по текущей директории (там, где main.c) */
    return 0;
}
