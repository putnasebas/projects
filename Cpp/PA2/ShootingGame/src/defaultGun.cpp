#include "defaultGun.h"

using namespace std;

DefaultGun::DefaultGun(int range, int ammo)
        : Weapon(range, ammo) {}

Weapon *DefaultGun::clone() const {
    return new DefaultGun(*this);
}

bool DefaultGun::blastOnHit() const {
    return m_blast;
}

void DefaultGun::display() const {
    cout << ".";
}

bool DefaultGun::passThroughWall() const {
    return m_passThroughWall;
}