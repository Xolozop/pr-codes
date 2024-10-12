#include <iostream>
#include <iomanip>

#include "ship-manager.h"

#define TERM_UNDERLINE "\033[4m"
#define TERM_DEF "\033[0m"

shipManager::shipManager(int am, std::vector<std::vector<Ship*>> shipArray) : amount{am} {
    ships.resize(shipArray.size());
    for (int x = 0; x < shipArray.size(); x++) {
        for (int y = 0; y < shipArray[x].size(); y++) {
            ships[x].push_back(shipArray[x][y]);
        }
    }
}

void shipManager::refresh(Ship* ship) {
    int len = ship->getLength();
    if (ships.size() < len)
        ships.resize(len);
    if (ships[len-1].size() >= (5 - len))
        throw "All ships of this length are already in the list! ";
    ships[len-1].push_back(ship);
    amount++;
}

int shipManager::getShipIndex(int len) const {
    return ships[len-1].size();
}

Ship* shipManager::getShip(int x, int y) {
    return ships[x-1][y-1];
}

void shipManager::printShipList() const {
    std::cout << TERM_UNDERLINE << "\t  №  |";
    for (int i = 1; i < amount+1; i++)
            std::cout << "  " << i << "  |";

    std::cout << TERM_DEF << std::endl;
    std::cout << "\tShip |";
    for (int x = 0; x < ships.size(); x++) {
        for (int y = 0; y < ships[x].size(); y++) {
            std::cout << std::setw(2-x/2) << ' ';
            for (int i = 1; i <= x+1; i++)
                ships[x][y]->printSeg(i);
            std::cout << std::setw(2-(x+1)/2) << ' ';
            std::cout << "|";
        }
    }
    std::cout << std::endl;
}

void shipManager::clear() {
    for (int x = 0; x < ships.size(); x++) {
        for (int y = 0; y < ships[x].size(); y++) {
            delete ships[x][y];
        }
    }
}
