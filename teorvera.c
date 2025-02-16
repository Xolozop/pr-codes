#include <time.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>
#include <stdio.h>

typedef unsigned short ushort; 

#define false 0
#define true 1

#define SUITS_NUM 4
#define RANKS_NUM 13
#define EXPERIMENTS_NUM pow(10, 8)

enum combination {
    high_card,
    pair,
    two_pair,
    three_of_kind,
    straight,
    flush, 
    full_house,
    four_of_kind,
    straight_flush,
    royal_flush
};

void take_cards(ushort (*result)[RANKS_NUM+1]) {
    for (int x = 0; x < SUITS_NUM+1; x++) {
        for (int y = 0; y < RANKS_NUM+1; y++) {
            result[x][y] = 0;
        }
    }
    
    for (int i = 0; i < 5; i++) {
        int rank = rand()%RANKS_NUM;
        int suit = rand()%SUITS_NUM;

        if (result[suit][rank] != 0) {
            i--;
            continue;
        } else {
            result[suit][rank] = 1;

            result[SUITS_NUM][rank]++;
            result[suit][RANKS_NUM]++;
        }
    }
}

void print_cards(ushort (*result)[RANKS_NUM+1]) {
    for (int x = 0; x < SUITS_NUM+1; x++) {
        for (int y = 0; y < RANKS_NUM+1; y++) {
            printf("%d ", result[x][y]);
        }
        puts("");
    }
    puts("");
}

enum combination check_rows(ushort (*result)[RANKS_NUM+1]) {
    ushort is_pair = false, is_three = false;

    for (ushort i = 0; i < RANKS_NUM; i++) {
        switch(result[SUITS_NUM][i]) {
            case 2:
                is_pair++;
                break;
            case 3:
                is_three = true;
                break;
            case 4:
                return four_of_kind;
        }
    }

    if (is_pair > 0) {
        if (is_pair == 2) 
            return two_pair;
        if (is_three == true)
            return full_house;
        return pair;
    }

    if (is_three == true)
        return three_of_kind;
    return high_card;
}

int check_straight(ushort (*result)[RANKS_NUM+1]) {
    unsigned short i = 0;
    while (result[SUITS_NUM][i] == 0)
        i++;

    for (unsigned short ind = 0; ind < 5; ind++) {
        if ((i+ind >= RANKS_NUM) || (result[SUITS_NUM][i+ind] == 0)) {
            if ((i == 0) && (ind == 4) && (result[SUITS_NUM][RANKS_NUM-1] == 1)) // стрит туз-2-3-4-5
                return i;
            return -1;
        }
    }
    return i;
}


ushort check_flush(ushort (*result)[RANKS_NUM+1]) {
    for (ushort i = 0; i < RANKS_NUM; i++) {
        if (result[i][RANKS_NUM] == 5)
            return true;
        if (result[i][RANKS_NUM] > 0)
            return false;
    }
    return false;
}


int main() {
    srand(time(NULL));
    char* combs[10] = {"старшая карта", "пара", "2 пары", "тройка", "стрит", "флэш", "фулл хаус", "каре", "стрит флэш", "флэш рояль"};
    size_t results[10] = {0};
    double probs[10] = {0};

    time_t start = clock();

    ushort result[SUITS_NUM+1][RANKS_NUM+1];
    for (size_t exp = 0; exp < EXPERIMENTS_NUM; exp++) {
        take_cards(result);
        //print_cards(result);

        unsigned short row_res = check_rows(result); 
        int straight_ind = -1;
        if (row_res == 0) {
            straight_ind = check_straight(result);
        } else {
            results[row_res]++;
            continue;
        }

        ushort is_flush = check_flush(result);
        if (is_flush) {
            if (straight_ind > -1) {
                if (straight_ind == 8) {
                    results[royal_flush]++;
                    continue;
                } else {
                    results[straight_flush]++;
                    continue;
                }
            } else {
                results[flush]++;
                continue;
            }
        }

        if (straight_ind > -1) {
            results[straight]++;
            continue;
        }

        results[high_card]++;
    }
    time_t end = clock();

    size_t sum = 0;
    double sum_prob = 0;
    for (int i = 0; i < 10; i++) {
        probs[i] = (double)results[i]/EXPERIMENTS_NUM;
        sum += results[i];
        sum_prob += probs[i];
    }

    for (int i = 0; i < 10; i++)
        printf("%s: %d %f\n", combs[i], results[i], probs[i]);

    printf("\nTotal prob = %f (%d experiments)", sum, sum_prob);
    printf("\nTime passed: %f\n",  (double)(end - start)/CLOCKS_PER_SEC);
    return 0;
}
