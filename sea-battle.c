#include <iostream>
#include <climits>

#include "field.h"
#include "ship-manager.h"
#include "ship.h"

#define CON_RED "\033[31m"
#define CON_DEF "\033[0m"

void error_handler() {
    std::cerr << CON_RED << "Некорректное значение, попробуйте снова: " << CON_DEF; 
    // works only on linux
    std::cin.clear();
    std::cin.ignore(LONG_MAX, '\n');
}

int main() {
    int size = 10;
    // std::cout << "Введите размер игрового поля n*n: ";
    // while (!(std::cin >> size) || (size < 1) || (size > 9)) {
    //     error_handler();
    // }
    Field f(size);

    std::vector<Ship*> ships;

    int len = 1;
    char pos;
    while (len != 0) {
        std::cout << "Введите размер корабля (для завершения введите 0): ";
        while (!(std::cin >> len) || (len < 0) || (len > 4)) {
            error_handler();
        }
        if (len == 0) break;
        std::cout << "Введите расположение корабля (h - горизонтальное,  v - вертикальное): ";
        std::cin >> pos;
        while ((pos != 'h') && (pos != 'v')) {
            error_handler();
            std::cin >> pos;
        }
        ships.push_back(new Ship(len, pos));
        if (ships.size() == 10)
            std::cout << "Все корабли введены!" << std::endl;
    }

    shipManager manager(ships.size(), ships);
    manager.printShipList();
    //f.printField();
    return 0;
}
