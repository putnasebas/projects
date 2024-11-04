#include "gameplayMap.h"

using namespace std;

GameMap &GameMap::createMap(Coordinates &position) {
    loadData getSize;
    Coordinates playerPos;
    getSize.getMapSize(m_mapWidth, m_mapLenght);
    m_gameMap = getSize.buildMap();
    getSize.getPlayerPos(position);
    this->getDropChances();
    return *this;
}

vector <vector<int>> GameMap::CreateMapPositions() const {
    vector <vector<int>> result;
    for (size_t y = 0; y <= m_mapLenght; ++y) {
        result.push_back({});
        for (size_t x = 0; x <= m_mapWidth; ++x) {
            if (m_gameMap.at({x, y})->isOccupied()) {
                result[y].emplace_back(0);
            } else result[y].emplace_back(1);
        }
    }
    return result;
}

GameMap &GameMap::getDropChances() {
    ConfigurateStats getData;
    getData.getSpawnRate(m_ammoSpawnRate, m_weaponSpawnRate, m_armorSpawnRate);
    return *this;
}

void GameMap::RemovePerson(const Coordinates &pos) {
    m_gameMap.at(pos)->isOccupied(true);
}

GameMap &GameMap::searchForMovingObj(const map <Coordinates, shared_ptr<Person>> &person, set <Coordinates> &visited) {
    bool end = false;
    for (auto it = m_gameMap.begin(); it != m_gameMap.end(); ++it) {
        if (it->second->isMovable() && (visited.find(it->first)) == visited.end()) {
            Coordinates pos = it->first;
            Coordinates moveLeft(pos.x - 1, pos.y);
            Coordinates moveRight(pos.x + 1, pos.y);
            if (m_gameMap.at(moveLeft)->isOccupied() && m_gameMap.at(moveRight)->isOccupied()) {
                if (person.find(moveLeft) != person.end()) {
                    person.at(moveLeft)->changeHealth();
                } else if (person.find(moveRight) != person.end()) {
                    person.at(moveRight)->changeHealth();
                }
                return *this;
            }
            if (m_path) {
                this->moveObject(moveLeft, pos, person);
                visited.insert(moveLeft);
            } else {
                this->moveObject(moveRight, pos, person);
                visited.insert(moveRight);
            }
            end = true;
            break;
        }
    }
    if (end) this->searchForMovingObj(person, visited);
    return *this;
}

GameMap &GameMap::moveObject(const Coordinates &change, const Coordinates &currPos,
                             const map <Coordinates, shared_ptr<Person>> &person) {
    if (!m_gameMap[change]->isOccupied()) {
        movingObject movingObj(change.x, change.y);
        emptySpace emptyObj(currPos.x, currPos.y);
        m_gameMap.erase(currPos);
        m_gameMap.erase(change);
        m_gameMap.emplace(pair<Coordinates, Object *>(currPos, emptyObj.clone()));
        m_gameMap.emplace(pair<Coordinates, Object *>(change, movingObj.clone()));
        return *this;
    }
    if (person.find(change) != person.end()) {
        person.at(change)->changeHealth();
        //m_path = true - obj is moving from left to right
        //m_path = false - obj is moving from right to left
        //m_path changes when moving obj hits another obj or person
        if (m_path) m_path = false;
        else m_path = true;
        return *this;
    } else {
        if (m_path) m_path = false;
        else m_path = true;
    }
    return *this;
}

GameMap &GameMap::breakObject(const Coordinates &pos) {
    if (m_gameMap.erase(pos)) {
        emptySpace obj(pos.x, pos.y);
        m_gameMap.emplace(pair<Coordinates, Object *>(pos, obj.clone()));
    }
    return *this;
}

GameMap &GameMap::displayBullets(const char &dest, const Coordinates &startingPos, int &loot,
                                 map <Coordinates, shared_ptr<Person>> &person, const Coordinates &pos, int damage,
                                 int range) {
    Coordinates change = startingPos;
    bool breakCycle = false;
    if (person.at(startingPos)->displayAmmo() < 1) return *this;
    if (dest == 'w') {
        person.at(startingPos)->changeAmmo(-1);
        while (change.y > 0 && range > 1) {
            change.y--;
            range--;
            this->moveBulletPos(startingPos, change, damage, breakCycle, loot, person, pos);
            if (breakCycle) break;
        }
    } else if (dest == 'a') {
        person.at(startingPos)->changeAmmo(-1);
        while (change.x > 0 && range > 1) {
            change.x--;
            range--;
            this->moveBulletPos(startingPos, change, damage, breakCycle, loot, person, pos);
            if (breakCycle) break;
        }
    } else if (dest == 's') {
        person.at(startingPos)->changeAmmo(-1);
        while (change.y < m_mapLenght && range > 1) {
            change.y++;
            range--;
            this->moveBulletPos(startingPos, change, damage, breakCycle, loot, person, pos);
            if (breakCycle) break;
        }
    } else if (dest == 'd') {
        person.at(startingPos)->changeAmmo(-1);
        while (change.x < m_mapWidth && range > 1) {
            change.x++;
            range--;
            this->moveBulletPos(startingPos, change, damage, breakCycle, loot, person, pos);
            if (breakCycle) break;
        }
    } else return *this;
    //explosion animation
    if (person.at(startingPos)->blast()) {
        Coordinates changeW(change.x, change.y - 1);
        Coordinates changeS(change.x, change.y + 1);
        Coordinates changeA(change.x - 1, change.y);
        Coordinates changeD(change.x + 1, change.y);

        this->moveBulletPos(startingPos, changeW, damage, breakCycle, loot, person, pos);
        this->moveBulletPos(startingPos, changeS, damage, breakCycle, loot, person, pos);
        this->moveBulletPos(startingPos, changeA, damage, breakCycle, loot, person, pos);
        this->moveBulletPos(startingPos, changeD, damage, breakCycle, loot, person, pos);
    }
    return *this;
}

GameMap &GameMap::moveBulletPos(const Coordinates &startingPos, const Coordinates &change, int damage, bool &breakCycle,
                                int &loot, map <Coordinates, shared_ptr<Person>> &person, const Coordinates &pos) {
    if (m_gameMap.find(change) == m_gameMap.end()) return *this;
    if (m_gameMap[change]->isOccupied()) {
        if ((person.find(change) != person.end()) && m_gameMap[change]->isOccupied()) {
            person.at(change)->changeHealth(damage);
        } else displayFire(person, change, startingPos);
        if (m_gameMap[change]->isBreakable()) {
            Loot getLoot(m_armorSpawnRate, m_ammoSpawnRate, m_weaponSpawnRate);
            loot = getLoot.getRandomLoot();
            this->breakObject(change);
        }
        if (person.contains(startingPos) and !person.at(startingPos)->PassThrough()) {
            breakCycle = true;
            return *this;
        }
    }
    displayFire(person, change, startingPos);
    return *this;
}

void GameMap::displayFire(const map <Coordinates, shared_ptr<Person>> &person, const Coordinates &currpos,
                          const Coordinates &pos) const {
    cout << "\033c";
    size_t foundAll = 0;
    size_t checkY = 0;
    auto itPerson = person.begin();
    for (const auto &it: m_gameMap) {
        if (it.first.y != checkY) {
            cout << endl;
            checkY++;
        }
        bool printed = false;
        if (itPerson->first == it.first) {
            itPerson->second->display();
            foundAll++;
            if (itPerson != person.end() && foundAll < person.size()) itPerson++;
            printed = true;
        }
        if (!printed) {
            if (currpos == it.first) {
                if (it.second->isOccupied()) {
                    it.second->displayOnHit();
                } else person.at(pos)->displayFire();
            } else it.second->display();
        }
    }
    cout << endl;
    this_thread::sleep_for(100ms);
}

GameMap &GameMap::spawnPlayer(const Coordinates &pos) {
    if (pos.x > m_mapWidth || pos.y > m_mapLenght)
        throw runtime_error("Player position is outside of map\n");
    if (m_gameMap.at(pos)->isOccupied())
        throw runtime_error("Place already occupied\n");
    m_gameMap.at(pos)->isOccupied(true);
    return *this;
}


GameMap &GameMap::executeMove(const Coordinates &move, const Coordinates &oldPos) {
    m_gameMap.at(oldPos)->isOccupied(true);
    m_gameMap.at(move)->isOccupied(true);
    return *this;
}

GameMap &GameMap::spawnEnemies(map <Coordinates, shared_ptr<Person>> &persons, const shared_ptr <Weapon> &Gun) {
    loadData getEnemies;
    getEnemies.getEnemyPositions(persons, Gun);
    for (auto it = persons.begin(); it != persons.end(); ++it) {
        if (it->first.x > m_mapWidth || it->first.y > m_mapLenght) {
            throw runtime_error("Position out of map.\n");
        }
        if (m_gameMap.at(it->first)->isOccupied()) {
            throw runtime_error("Place already occupied\n");
        }
        m_gameMap.at(it->first)->isOccupied(true);
    }
    return *this;
}

void GameMap::display(const map <Coordinates, shared_ptr<Person>> &person) const {
    size_t checkY = 0;
    auto itPersons = person.begin();
    size_t foundAll = 0;
    for (const auto &it: m_gameMap) {
        if (it.first.y != checkY) {
            cout << endl;
            checkY++;
        }
        bool printed = false;
        if (itPersons->first == it.first) {
            itPersons->second->display();
            foundAll++;
            if (itPersons != person.end() && foundAll < person.size()) itPersons++;
            printed = true;
        }
        if (!printed) it.second->display();
    }
    cout << endl;
    this_thread::sleep_for(100ms);
}

GameMap &GameMap::ClearMap() {
    for (const auto &it: m_gameMap) {
        if (it.second->isOccupied()) {
            it.second->isOccupied(true);
        }
    }
    return *this;
}