#include <iostream>
#include <climits>

#include "field.h"
#include "ship-manager.h"
#include "ship.h"

#define TERM_GREEN "\033[32m"
#define TERM_RED "\033[31m"
#define TERM_DEF "\033[0m"
// works only on linux

typedef struct {
    int x;
    char y;
} coords_t;

void error_handler(const char* mes = "Incorrect value! ") {
    std::cerr << TERM_RED << mes << "Try again: " << TERM_DEF;
    std::cin.clear();
    std::cin.ignore(LONG_MAX, '\n');
}

void createEnemyField() {
    
}

int main() {
    int fieldSize = 10;
    // std::cout << "Введите размер игрового поля n*n: ";
    // while (!(std::cin >> size) || (size < 1) || (size > 9)) {
    //     error_handler();
    // }
    Field enemyField(fieldSize);
    std::vector<Ship*> enemyShips[4];
    int y = 1;
    int x = 1;
    int amount = 0;
    for (int len = 1; len < 5; len++) {
        for (int count = 0; count < (5 - len); count++) {
            amount++;
            char ori = (len%2) ? 'h' : 'v'; 
            Ship* tmp = new Ship(len, ori);
            enemyShips[len-1].push_back(tmp);
            while (1) {
                y = std::rand()/(RAND_MAX/fieldSize);
                x = std::rand()/(RAND_MAX/fieldSize);
                try {
                    enemyField.setShip(tmp, (char)(y + 97), x, amount);
                } catch (...) {
                    continue;
                }
                break;
            }
        }
    }
    shipManager enemyManager(amount, enemyShips);


    Field myField(fieldSize);
    std::vector<Ship*> ships[4];
    int ships_count = 0;
    int len;
    char ori = 'h';
    coords_t shipCoords;
    std::cout << "Your field: " << std::endl;

    shipManager myManager(ships_count, ships);
    while (ships_count != 10) {
        std::cout << "Please, enter ship length: ";
        while ((std::cin >> len) || 1) {
            if (ships[len-1].size() == (5-len)) {
                error_handler("All ships of this length are entered! ");
                continue;
            }
            if (std::cin.fail()) {
                error_handler();
                continue;
            }
            if ((len < 1) || (len > 4)) {
                error_handler("Improper value of ship length! Must be a number from 1 to 4! ");
                continue;
            }
            break;
        }

        Ship* tmp;
        if (len > 1) {
            std::cout << "Enter ship orientation (H for horizontal, V for vertical): ";
            while (std::cin >> ori || 1) {
                ori = tolower(ori);
                try {
                    tmp = new Ship(len, ori);
                } catch (const char* err) {
                    error_handler(err);
                    continue;
                }
                break;
            }
        } else {
            tmp = new Ship(len, 'h');
        }

        std::cout << "Please, input coordinats of the start of the ship in format L N (there L is a letter for vertical position and N is a number for a horizontal position): ";
        while ((std::cin >> shipCoords.y >> shipCoords.x) || 1) {
            shipCoords.y = tolower(shipCoords.y);
            if (std::cin.fail()) {
                error_handler();
                continue;
            }
            try {
                myField.setShip(tmp, shipCoords.y, shipCoords.x, ships_count);
            } catch (const char* err) {
                error_handler(err);
                continue;
            }

            std::cin.ignore(LONG_MAX, '\n');
            std::cout << "Your field: " << std::endl;
            myField.printField(true, true);
            std::cout << "Do you agree? (enter N to replace ship, Y or ENTER to continue): ";
            char ans = getchar();
            while ((tolower(ans) != 'n') && (ans != '\n') && (tolower(ans) != 'y')) {
                error_handler();
                ans = getchar();
            }
            if (tolower(ans) == 'n') {
                std::cout << "Please, input again coordinats of the start of the ship in format L N (there L is a letter for vertical position and N is a number for a horizontal position): ";
                myField.deleteShip(tmp, shipCoords.y, shipCoords.x);
                continue;
            }
            break;
        }

        ships[len-1].push_back(tmp);
        myManager.addShip(tmp);
        ships_count++;
        if (ships_count == 10) {
            std::cout << TERM_GREEN << "All ships are installed!\n" << TERM_DEF << std::endl;
            break;
        }
    }

    std::cout << "Your ships: " << std::endl;
    myManager.printShipList();
    std::cout << "Your field: " << std::endl;
    myField.printField(false, true, myManager);
    std::cout << std::endl;
    std::cout << "Enemy's ships: " << std::endl;
    enemyManager.printShipList();
    std::cout << "Enemy's field: " << std::endl;
    enemyField.printField(false, false, enemyManager);

    /*coords_t shotCoords = {'d', 1};
    while ((std::cin >> shotCoords.y >> shotCoords.x) && (shotCoords.y != '0')) {
        std::cout << "Where do you want to shoot? Input coordinates: ";
        shotCoords.y = tolower(shotCoords.y);
        if (std::cin.fail()) {
            error_handler();
            continue;
        }
        try {
            //enemyField.shoot(shotCoords.y, shotCoords.x);
        } catch (const char* err) {
            error_handler(err);
            continue;
        }

        std::cin.ignore(LONG_MAX, '\n');
        enemyField.printField(false, false);
        std::cout << "Do you agree? (enter N to replace ship, Y or ENTER to continue): ";
        char ans = getchar();
        while ((tolower(ans) != 'n') && (ans != '\n') && (tolower(ans) != 'y')) {
            error_handler();
            ans = getchar();
        }
        if (tolower(ans) == 'n') {
            std::cout << "Please, shoot again: ";
            continue;
        }
        break;
    }*/
// get shoot coordinates from terminal

    try {
        enemyField.shoot('a', 2);
        enemyField.shoot('c', 4);
        enemyField.shoot('a', 2);
        enemyField.shoot('i', 3);
    } catch (const char* err) {
        error_handler(err);
    }

    enemyField.printField(false, true, enemyManager);
    //enemyField.printField(false, false);

    enemyManager.clear();
    myManager.clear();
    return 0;
}

/*
link ship and field
which block is under atack
change print field, add switch
atack
colourfull field
*/