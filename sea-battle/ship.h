#ifndef SHIP_H
#define SHIP_H

enum segStates {intact, damaged, destroyed};
enum orientation {horizontal, vertical};

class Ship 
{
private:
    int length;
    orientation orient;
    segStates* segments = nullptr;
public:
    Ship(int len, char ori);
    Ship(const Ship& other);
    ~Ship();
    int getLength() const;
    bool isVertical() const;
    bool isDestroyed() const;
    int getState(int i) const;
    void printSeg(int i) const;
    void atack(int i);
};

#endif