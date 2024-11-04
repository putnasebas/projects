#include "blastgun.h"

using namespace std;

BlastGun::BlastGun(int range, int ammo)
        : Weapon(range, ammo) {}

Weapon *BlastGun::clone() const {
    return new BlastGun(*this);
}

void BlastGun::display() const {
    cout << ",";
}

bool BlastGun::blastOnHit() const {
    return m_blast;
}

bool BlastGun::passThroughWall() const {
    return m_passThroughWall;
}