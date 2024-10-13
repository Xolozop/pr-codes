#include <iostream>
#include <climits>
#include <ctime>

#include "ship.h"
#include "ship-manager.h"
#include "field.h"

#define TERM_GREEN "\033[32m"
#define TERM_RED "\033[31m"
#define TERM_DEF "\033[0m"
#define FIELD_SZ 10
// works only on linux

typedef struct {
    int x;
    char y;
} coords_t;

shipManager createEnemyField(Field* enemyField) {
    std::vector<std::vector<Ship>> enemyShips(4);
    shipManager manager = shipManager(0, enemyShips);
    int x, y;
    int amount = 0;
    for (int len = 0; len < enemyShips.size(); len++) {
        for (int count = 0; count < (4 - len); count++) {
            char ori = (len%2) ? 'h' : 'v'; 
            Ship tmp(len+1, ori);
            enemyShips[len].push_back(tmp);
            //srand(time(0));
            while (1) {
                y = std::rand()/(RAND_MAX/FIELD_SZ);
                x = std::rand()/(RAND_MAX/FIELD_SZ);
                try {
                    enemyField->setShip(tmp, (char)(y + 97), x, count+1);
                } catch (const char* err) {
                    continue;
                }
                break;
            }
            amount++;
        }
    }
    manager = shipManager(amount, enemyShips);
    return manager;
}

int main() {
    Field enemyField(FIELD_SZ, false);
    // change to true to see field and skipi confirmation
    shipManager enemyManager = createEnemyField(&enemyField);
    
    Field playersField(FIELD_SZ, true);
    std::vector<std::vector<Ship>> playersShips;
    shipManager playersManager(0, playersShips);
    int shipAmount = 0;

    int len = 4;
    Ship ship1(len, 'v');
    playersManager.refresh(ship1);
    shipAmount++;

    try {
        Ship ship2(len, 'h');
        playersManager.refresh(ship2);
        shipAmount++;
    } catch (const char* err) {
        std::cerr << TERM_RED << err << TERM_DEF << std::endl;
    }

    len = 2;
    Ship ship2(len, 'h');
    playersManager.refresh(ship2);
    shipAmount++;

    try { 
        playersField.setShip(ship1, 'c', 8, playersManager.getShipIndex(len)); 
    } catch (const char* err) { 
        std::cerr << TERM_RED << err << TERM_DEF << std::endl; 
    }
    playersField.setShip(ship1, 'c', 3, playersManager.getShipIndex(len));
    
    try {
        playersField.setShip(ship2, 'd', 6, playersManager.getShipIndex(len));
    } catch (const char* err) {
        std::cerr << TERM_RED << err << TERM_DEF << std::endl;
    }
    try { 
        playersField.setShip(ship2, 'j', 10, playersManager.getShipIndex(len)); 
    } catch (const char* err) { 
        std::cerr << TERM_RED << err << TERM_DEF << std::endl; 
    }
    try {
        playersField.setShip(ship2, 'c', 6, playersManager.getShipIndex(len));
    } catch (const char* err) {
        std::cerr << TERM_RED << err << TERM_DEF << std::endl;
    }
    playersField.setShip(ship2, 'i', 10, playersManager.getShipIndex(len));

    enemyField.shoot('g', 5, enemyManager);
    enemyField.shoot('e', 5, enemyManager);
    enemyField.shoot('i', 3, enemyManager);
    try {
        enemyField.shoot('k', 5, enemyManager);
    } catch(const char* err) {
        std::cerr << TERM_RED << err << TERM_DEF << std::endl;
    }
    try {
        enemyField.shoot('a', 11, enemyManager);
    } catch(const char* err) {
        std::cerr << TERM_RED << err << TERM_DEF << std::endl;
    }
    try {
        enemyField.shoot('i', 3, enemyManager);
    } catch(const char* err) {
        std::cerr << TERM_RED << err << TERM_DEF << std::endl;
    }
    try {
        enemyField.shoot('i', 3, enemyManager);
    } catch(const char* err) {
        std::cerr << TERM_RED << err << TERM_DEF << std::endl;
    }

    std::cout << "Your ships: " << std::endl;
    playersManager.printShipList();
    std::cout << "Your field: " << std::endl;
    playersField.printField(true, playersManager);
    std::cout << std::endl;
    std::cout << "Enemy ships: " << std::endl;
    enemyManager.printShipList();
    std::cout << "Enemy field: " << std::endl;
    enemyField.printField(false, enemyManager);

    return 0;
}