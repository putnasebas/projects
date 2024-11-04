#include "movingObject.h"

using namespace std;

movingObject::movingObject(size_t x, size_t y)
        : Object(x, y) {}

Object *movingObject::clone() const {
    return new movingObject(*this);
}

void movingObject::displayOnHit() const {
    cout << "\033[47m" << "*" << "\033[0m";
}

bool movingObject::isMovable() const {
    return true;
}

bool movingObject::isBreakable() const {
    return false;
}

bool movingObject::isOccupied(bool changeStatus) {
    (void) changeStatus;
    m_occupied = true;
    return true;
}

void movingObject::display() const {
    cout << "*";
}