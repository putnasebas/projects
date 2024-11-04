#include "person.h"

using namespace std;

Person::Person(int health, const Coordinates &pos)
        : m_health(health), m_pos(pos) {}

Person &Person::update(const Coordinates &move, Coordinates &pos) {
    m_pos = move;
    pos = m_pos;
    return *this;
}

Coordinates Person::getPosition() const {
    return m_pos;
}

bool Person::hasTurn() const {
    return played;
}

Person &Person::changeMovePref() {
    if (played == true) played = false;
    else played = true;
    return *this;
}

Person &Person::setArmor(int set) {
    m_armor = set;
    return *this;
}

void Person::displayStats() const {
    cout << "\033[31m" << "Health:" << m_health << "\033[0m" << endl;
    cout << "\033[34m" << "Armor: " << m_armor << "\033[0m" << endl;
    cout << "Ammo:" << this->displayAmmo();
}

Person &Person::equipWeapon(const shared_ptr <Weapon> &weapon) {
    m_currWeapon.reset(weapon->clone());
    return *this;
}

int Person::equipWeaponRange() const {
    return m_currWeapon->getRange();
}

int Person::displayAmmo() const {
    return m_currWeapon->displayAmmo();
}

Person &Person::changeAmmo(int change) {
    m_currWeapon->changeAmmo(change);
    return *this;
}

bool Person::PassThrough() const {
    return m_currWeapon->passThroughWall();
}

bool Person::blast() const {
    return m_currWeapon->blastOnHit();
}

void Person::displayFire() const {
    return m_currWeapon->display();
}

int Person::showHealth() const {
    return m_health;
}

int Person::showArmor() const {
    return m_armor;
}
