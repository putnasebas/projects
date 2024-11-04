#include "player.h"

using namespace std;

Player::Player(int health, const Coordinates &pos)
        : Person(health, pos) {}

Person *Player::clone() const {
    return new Player(*this);
}

Person &Player::changeHealth(int damage) {
    if (m_armor > (damage + 3)) m_armor -= (damage + 3);
    else if (m_armor > 0) m_armor = 0;
    else m_health -= damage;
    return *this;
}

Coordinates Player::nextMove(const Coordinates &tryPos, const vector <vector<int>> &gameMap) const {
    if (gameMap[tryPos.y][tryPos.x] == 0) return m_pos;
    return tryPos;
}

char Player::shootingDirection(const Coordinates &dest, const std::vector <std::vector<int>> &gameMap) const {
    if (gameMap[dest.y][dest.x] == 0) return 'x';
    if (dest.x == m_pos.x && dest.y + 1 == m_pos.y) return 'w';
    if (dest.x == m_pos.x && dest.y - 1 == m_pos.y) return 's';
    if (dest.x + 1 == m_pos.x && dest.y == m_pos.y) return 'a';
    if (dest.x - 1 == m_pos.x && dest.y == m_pos.y) return 'd';
    return 'x';
}


void Player::display() const {
    cout << "\033[5;93m" << "@" << "\033[0m";
}