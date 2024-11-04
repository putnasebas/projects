#include "gameloop.h"

using namespace std;

gameLoop &gameLoop::loadMap() {
    ConfigurateStats getWeaponStats;
    bulletDamage = getWeaponStats.getBulletDamage();
    int ammo = 0, range = 0;
    getWeaponStats.getWeaponStat(ammo, range, 0);
    DefaultGun defGun(range, ammo);
    defGunptr = make_shared<DefaultGun>(defGun);
    getWeaponStats.getWeaponStat(ammo, range, 1);
    BlastGun upgdrGun(range, ammo);
    defUpgrdptr = make_shared<BlastGun>(upgdrGun);
    getWeaponStats.getWeaponStat(ammo, range, 2);
    PassingThroughWallGun OPGun(range, ammo);
    defOPptr = make_shared<PassingThroughWallGun>(OPGun);
    m_customMap.createMap(m_position);
    m_customMap.spawnPlayer(m_position);
    m_customMap.spawnEnemies(m_persons, defGunptr);
    Player person(1, m_position);
    person.equipWeapon(defGunptr);
    m_persons.emplace(m_position, person.clone());
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    return *this;
}

gameLoop &gameLoop::play(size_t &score, bool &repeat) {
    this->loadMap();
    //GameLoop
    while (true) {
        //Move on board
        //reset player turns
        for (auto it = m_persons.begin(); it != m_persons.end(); ++it) {
            if (!it->second->hasTurn())
                it->second->changeMovePref();
        }
        cout << "\033c";
        m_customMap.display(m_persons);
        m_persons.at(m_position)->displayStats();
        cout << endl << "\033[33m" << "move!" << "\033[0m" << endl;
        //Player and AI making moves
        char nextmove;
        cin >> nextmove;
        if (m_persons.find(m_position) == m_persons.end() || nextmove == 'q') {
            this->DefeatScreen(score);
            repeat = false;
            break;
        }
        if (nextmove == 'r') {
            score = 0;
            repeat = true;
            break;
        }
        Coordinates TryMove = this->calculatePos(nextmove);
        this->PersonMove(TryMove);
        set <Coordinates> Visitedposition;
        m_customMap.searchForMovingObj(m_persons, Visitedposition);
        this->update(score);
        if (m_persons.find(m_position) == m_persons.end() || nextmove == 'q') {
            this->DefeatScreen(score);
            repeat = false;
            break;
        } else if (m_persons.size() < 2) {
            this->VictoryScreen(score);
            repeat = false;
            break;
        }
        Visitedposition.clear();
        //Second move on board when player can shoot
        //reset player turns
        for (auto it = m_persons.begin(); it != m_persons.end(); ++it) {
            if (!it->second->hasTurn())
                it->second->changeMovePref();
        }
        cout << "\033c";
        m_customMap.display(m_persons);
        m_persons.at(m_position)->displayStats();
        cout << endl << "\033[31m" << "shoot!" << "\033[0m" << endl;
        cin >> nextmove;
        if (m_persons.find(m_position) == m_persons.end() || nextmove == 'q') {
            this->DefeatScreen(score);
            repeat = false;
            break;
        }
        if (nextmove == 'r') {
            score = 0;
            repeat = true;
            break;
        }
        this->shootingRound(bulletDamage, nextmove, score);
        this->update(score);
        if (m_persons.find(m_position) == m_persons.end() || nextmove == 'q') {
            this->DefeatScreen(score);
            repeat = false;
            break;
        } else if (m_persons.size() < 2) {
            this->VictoryScreen(score);
            repeat = false;
            break;
        }
        m_customMap.searchForMovingObj(m_persons, Visitedposition);
        this->update(score);
        if (m_persons.find(m_position) == m_persons.end() || nextmove == 'q') {
            this->DefeatScreen(score);
            repeat = false;
            break;
        } else if (m_persons.size() < 2) {
            this->VictoryScreen(score);
            repeat = false;
            break;
        }
        Visitedposition.clear();
    }
    m_persons.clear();
    return *this;
}

Coordinates gameLoop::calculatePos(char move) {
    Coordinates TryMove = m_position;
    switch (move) {
        case ('w'):
            TryMove.y--;
            break;
        case ('a'):
            TryMove.x--;
            break;
        case ('s'):
            TryMove.y++;
            break;
        case ('d'):
            TryMove.x++;
            break;
        default:
            return m_position;
    }
    return TryMove;
}

gameLoop &gameLoop::applyBuff(int loot, const Coordinates &pos, const shared_ptr <Weapon> &applyWeapon) {
    int currArmor = m_persons.at(pos)->showArmor();
    if (loot == -1) return *this;
    else if (loot == 0) {
        m_persons.at(pos)->changeAmmo(5);
    } else if (loot == 1) {
        currArmor++;
        m_persons.at(pos)->setArmor(currArmor);
    } else {
        m_persons.at(pos)->equipWeapon(applyWeapon);
    }
    return *this;
}

gameLoop &gameLoop::update(size_t &score) {
    for (auto it = m_persons.begin(); it != m_persons.end(); ++it) {
        if (it->second->showHealth() < 1) {
            m_customMap.RemovePerson(it->first);
            score += 5;
            m_persons.erase(it->first);
            break;
        }
    }
    return *this;
}

void gameLoop::VictoryScreen(size_t &score) const {
    char endgame;
    cout << "\033c";
    m_customMap.display(m_persons);
    cout << "\nCongratulations!! You have cleared the level.\n";
    score += 10;
    cout << "Press key to continue\n";
    cin >> endgame;
}

void gameLoop::DefeatScreen(size_t &score) const {
    char endgame;
    cout << "\033c";
    m_customMap.display(m_persons);
    cout << "\nYou have died.\n";
    if (score < 5) score = 0;
    else score -= 5;
    cout << "Press key to continue\n";
    cin >> endgame;
}

gameLoop &gameLoop::PersonMove(const Coordinates &nextMove) {
    bool found = false;
    for (auto it = m_persons.begin(); it != m_persons.end(); ++it) {
        if (it->second->hasTurn()) {
            vector <vector<int>> mapVisualization = m_customMap.CreateMapPositions();
            found = true;
            Coordinates newPos = it->second->nextMove(nextMove, mapVisualization);
            if (it->first == newPos) {
                m_persons.at(it->first)->changeMovePref();
                break;
            }
            m_customMap.executeMove(newPos, it->first);
            int armor = m_persons.at(it->first)->showArmor();
            Coordinates oldPos = it->first;
            Coordinates update = it->first;
            it->second->update(newPos, update);
            if (m_position == oldPos) m_position = update;
            m_persons.emplace(update, it->second->clone());
            m_persons.at(newPos)->changeMovePref();
            m_persons.at(newPos)->setArmor(armor);
            if (oldPos != newPos) m_persons.erase(it->first);
            break;
        }
    }
    if (found) this->PersonMove(nextMove);
    return *this;
}

gameLoop &gameLoop::shootingRound(int bulletDamage, char direction, size_t &score) {
    for (auto it = m_persons.begin(); it != m_persons.end();) {
        Coordinates dest = this->calculatePos(direction);
        vector <vector<int>> gameMap = m_customMap.CreateMapPositions();
        if (it->second->hasTurn()) {
            int getLoot = -1;
            char shootingPath = it->second->shootingDirection(dest, gameMap);
            int range = it->second->equipWeaponRange();
            m_customMap.displayBullets(shootingPath, it->first, getLoot, m_persons, m_position, bulletDamage, range);
            it->second->changeMovePref();
            if (getLoot == 2) this->applyBuff(getLoot, it->first, defUpgrdptr);
            else if (getLoot == 3) this->applyBuff(getLoot, it->first, defOPptr);
            else this->applyBuff(getLoot, it->first, defGunptr);
            this->update(score);
        } else ++it;
    }
    return *this;
}
