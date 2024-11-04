#include "weapon.h"

using namespace std;

Weapon::Weapon(int range, int ammo)
        : m_range(range), m_ammo(ammo) {}

Weapon &Weapon::changeAmmo(int change) {
    m_ammo += change;
    return *this;
}

int Weapon::displayAmmo() const {
    return m_ammo;
}

int Weapon::getRange() const {
    return m_range;
}