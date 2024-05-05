#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 20

typedef struct Pair {
    char* key;
    int count;
} Pair;

int cmp(const void* a, const void* b) {
    char* f = (*(Pair**)a)->key;
    char* e = (*(Pair**)b)->key;
    return strcmp(f, e);
}

int main() {
    int size = SIZE;
    int pairs = 0;
    Pair** arr = (Pair**)malloc(size*sizeof(Pair*));
    char c;
    char* wrd;
    while(1) {
        c = getchar();
        int i = 0;
        wrd = (char*)malloc(sizeof(char)*50);
        while(c != '\n' && c != ' '){
            wrd[i++] = c;
            c = getchar();
            if (strstr(wrd, "[Final]") != NULL)
                break;
        }
        wrd[i] = '\0';
        if (strstr(wrd, "[Final]") != NULL)
            break;

        if (!strcmp(wrd, " "))
            continue;
        int flag = 0;
        for (int x = 0; x < pairs; x++) {
            if(!strcmp(arr[x]->key, wrd)) {
                arr[x]->count++; 
                flag = 1;
                break;
            } 
        }
        if (!flag) {
            arr[pairs] = (Pair*)malloc(sizeof(Pair));
            arr[pairs]->key = wrd;
            arr[pairs]->count = 1;
            pairs++;
        }
    }

    qsort(arr, pairs, sizeof(Pair*), cmp);

    for (int i = 0; i < pairs; i++) {
        printf("%s %d\n", arr[i]->key, arr[i]->count);
    }

    return 0;
}
