#include <iostream>

#include "ship.h"

#define TERM_DEF "\033[0m"
#define TERM_CROSSED "\033[9m"
#define TERM_GREEN "\033[32m"
#define TERM_RED "\033[31m"

Ship::Ship(int len, char ori) {
    if ((len < 1) || (len > 4))
        throw "Improper length of ship!";
    length = len;
    segments = new segStates[length];
    for (int i = 0; i < length; i++)
        segments[i] = segStates::intact;
    if ((ori != 'h') && (ori != 'v'))
        throw "Improper orientation!";
    orient = (ori == 'h') ? orientation::horizontal : orientation::vertical;
}

Ship::~Ship() {
    if (segments != nullptr)
        delete[] segments;
}

Ship::Ship(const Ship& other) : length(other.length), orient(other.orient) {
    delete[] segments;
    if (other.segments != nullptr) {
        segments = new segStates[length];
        for(int i = 0; i < length; i++)
            segments[i] = other.segments[i];
    }
}


int Ship::getLength() const {
    return length;
}

bool Ship::isVertical() const {
    return orient == orientation::vertical;
}

bool Ship::isDestroyed() const {
    for (int i = 0; i < length; i++) {
        if (segments[i] != segStates::destroyed) 
            return false; 
    }
    return true;
}

int Ship::getState(int i) const {
    return segments[i-1];
}

void Ship::printSeg(int i) const {
    if (segments[i-1] != segStates::intact) {
        std::cout << TERM_RED;
        if (segments[i-1] == segStates::destroyed)
            std::cout << TERM_CROSSED;
    }   
    std::cout << 'X' << TERM_DEF;
}

void Ship::atack(int i) {
    i--;
    switch(segments[i]) {
        case segStates::intact:
            segments[i] = segStates::damaged;
            std::cout << TERM_GREEN << "Congratulations! You're damaged the ship!" << TERM_DEF << std::endl;
            break;
        case segStates::damaged:
            segments[i] = segStates::destroyed;
            std::cout << TERM_GREEN << "Congratulations! You destroyed the ship segment!" << TERM_DEF << std::endl;
            break;
        default:
            throw "An error occured...";
    }
}