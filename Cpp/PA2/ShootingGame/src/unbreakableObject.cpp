#include "unbreakableObject.h"

using namespace std;

UnBreakableObject::UnBreakableObject(size_t x, size_t y)
        : Object(x, y) {}

void UnBreakableObject::display() const {
    cout << "\033[42m" << "#" << "\033[0m";
}

void UnBreakableObject::displayOnHit() const {
    cout << "\033[47m" << "#" << "\033[0m";
}

Object *UnBreakableObject::clone() const {
    return new UnBreakableObject(*this);
}

bool UnBreakableObject::isMovable() const {
    return false;
}

bool UnBreakableObject::isBreakable() const {
    return false;
}

bool UnBreakableObject::isOccupied(bool changeStatus) {
    (void) changeStatus;
    m_occupied = true;
    return true;
}

