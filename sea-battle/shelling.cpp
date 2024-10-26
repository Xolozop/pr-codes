#include <iostream>

#include "shelling.h"

Shelling::Shelling(Field& field, shipManager& manager) : Skill(field, manager) { }

void Shelling::useSkill() {
    printSkillName();
    int* indexes = enemyManager.getRandomUndamagedShip();
    Ship& ship = enemyManager.getShip(indexes[0]+1, indexes[1]+1);
    ship.shellAtack();
    delete indexes;
}

void Shelling::printSkillName() const {
    std::cout << "Using Shelling..." << std::endl;
}