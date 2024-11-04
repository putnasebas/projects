#include "overpoweredGun.h"

using namespace std;

PassingThroughWallGun::PassingThroughWallGun(int range, int ammo)
        : Weapon(range, ammo) {}

Weapon *PassingThroughWallGun::clone() const {
    return new PassingThroughWallGun(*this);
}

void PassingThroughWallGun::display() const {
    cout << ":";
}

bool PassingThroughWallGun::blastOnHit() const {
    return m_blast;
}

bool PassingThroughWallGun::passThroughWall() const {
    return m_passThroughWall;
}