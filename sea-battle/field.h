#ifndef FIELD_H
#define FIELD_H

#include "ship.h"
#include "ship-manager.h"

enum blockStates {unknown=-4, shoted=-3, atGunpoint=-2, padding=-1, empty=0};
class Field 
{
private:
    int size;
    bool isMine;
    std::vector<std::vector<int>> fieldBlocks;
    bool confirmData() const;
public:
    Field(int size, bool isMine);
    ~Field();
    void printField(bool showPaddings, shipManager& manager) const;
    void setShip(Ship& ship, char coord_y, int coord_x, int ship_ind);
    void shoot(char coord_y, int coord_x, shipManager& manager);
};

#endif 