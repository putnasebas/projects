#include "emptySpace.h"
using namespace std;

emptySpace::emptySpace(size_t x, size_t y)
    : Object(x,y) {}

Object *emptySpace::clone() const {
    return new emptySpace(*this);
}

bool emptySpace::isBreakable() const {
    return false;
}
        
bool emptySpace::isMovable() const {
    return false;
} 

bool emptySpace::isOccupied(bool changeStatus) {
    if(changeStatus) {
        if(m_occupied) m_occupied = false;
        else m_occupied = true;
    }
    return m_occupied;
}

void emptySpace::display() const {
    cout << " ";
}

void emptySpace::displayOnHit() const {
    this->display();
}
