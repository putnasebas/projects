#include "breakableObject.h"

using namespace std;

breakableObject::breakableObject(size_t x, size_t y)
        : Object(x, y) {}

void breakableObject::display() const {
    cout << "\033[103m" << "<" << "\033[0m";
}

void breakableObject::displayOnHit() const {
    this->display();
}

bool breakableObject::isBreakable() const {
    return true;
}

bool breakableObject::isMovable() const {
    return false;
}

bool breakableObject::isOccupied(bool changeStatus) {
    (void) changeStatus;
    m_occupied = true;
    return true;
}

Object *breakableObject::clone() const {
    return new breakableObject(*this);
}