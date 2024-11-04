#include "configurate.h"

using namespace std;

//width = X |, lenght = Y ->
//          V
loadData &loadData::getMapSize(size_t &width, size_t &lenght) {
    this->getNumber(width, lenght, m_mapname);
    m_width = width;
    m_lenght = lenght;
    return *this;
}

loadData loadData::getNumber(size_t &x, size_t &y, const string &filename) {
    ifstream file(filename);
    if (!file.is_open() || !file) {
        throw runtime_error("Unable to load the file.\n");
    }
    string line;
    string test;
    bool found = false;
    getline(file, line);
    stringstream str;
    size_t tempNum;
    str << line;
    int count = 0;
    while (!str.eof()) {
        str >> test;
        if (stringstream(test) >> tempNum) {
            switch (count) {
                case (0):
                    x = tempNum;
                    break;
                case (1):
                    y = tempNum;
                    break;
            }
            count++;
        }
        if (count == 2) found = true;
    }
    if (!found) {
        throw runtime_error("Unable to load data.\n");
    }
    return *this;
}

map <Coordinates, shared_ptr<Object>> loadData::buildMap() {
    map <Coordinates, shared_ptr<Object>> customMap;
    ifstream file(m_mapname);
    string name;
    if (!file.is_open() || !file) {
        throw runtime_error("Unable to load the file.\n");
    }
    // skip first line
    getline(file, name);
    size_t Y = 0;
    while (getline(file, name)) {
        if (name.size() - 1 != m_width) throw runtime_error("Invalid map\n");
        for (size_t X = 0; X < name.size(); ++X) {
            if (Y == 0 || Y == m_lenght) {
                if (name[X] != '#') throw runtime_error("Invalid map edge\n");
            }
            if ((X == 0 || X == m_width) && name[X] != '#') throw runtime_error("Invalid map edge\n");
            if (name[X] == '#') {
                UnBreakableObject obj(X, Y);
                customMap.emplace(pair<Coordinates, Object *>(Coordinates(X, Y), obj.clone()));
            } else if (name[X] == ' ') {
                emptySpace obj(X, Y);
                customMap.emplace(pair<Coordinates, Object *>(Coordinates(X, Y), obj.clone()));
            } else if (name[X] == '<') {
                breakableObject obj(X, Y);
                customMap.emplace(pair<Coordinates, Object *>(Coordinates(X, Y), obj.clone()));
            } else if (name[X] == '*') {
                movingObject obj(X, Y);
                customMap.emplace(pair<Coordinates, Object *>(Coordinates(X, Y), obj.clone()));
            } else {
                throw runtime_error("Invalid character found\n");
            }
        }
        Y++;
    }
    if (Y - 1 != m_lenght) throw runtime_error("Invalid map size\n");
    return customMap;
}

loadData loadData::getEnemyPositions(map <Coordinates, shared_ptr<Person>> &persons, const shared_ptr <Weapon> &Gun) {
    ifstream file(m_wavefile);
    Coordinates data;
    if (!file.is_open() || !file) {
        throw runtime_error("Unable to load the file.\n");
    }
    string line;
    string test;
    bool found = false;
    while (getline(file, line)) {
        char enemyType = line[0];
        if (line[0] != 69 && line[0] != 70)
            throw runtime_error("Invalid Enemy type\n");
        stringstream str;
        size_t tempNum;
        str << line;
        int count = 0;
        while (!str.eof()) {
            str >> test;
            if (stringstream(test) >> tempNum) {
                switch (count) {
                    case (0):
                        data.x = tempNum;
                        break;
                    case (1):
                        data.y = tempNum;
                        break;
                }
                count++;
            }
            if (count == 2) found = true;
        }
        if (!found) {
            throw runtime_error("Unable to load data.\n");
        }
        if (enemyType == 69) {
            Computer enemy(1, data);
            enemy.equipWeapon(Gun);
            persons.emplace(pair<Coordinates, Person *>(data, enemy.clone()));
        } else {
            Computer enemy(2, data);
            enemy.equipWeapon(Gun);
            persons.emplace(pair<Coordinates, Person *>(data, enemy.clone()));
        }
    }
    return *this;
}

loadData loadData::getPlayerPos(Coordinates &position) {
    size_t X = 0;
    size_t Y = 0;
    this->getNumber(X, Y, m_playerpos);
    if (X > m_width || Y > m_lenght) {
        throw runtime_error("Position out of map.\n");
    }
    position.x = X;
    position.y = Y;
    return *this;
}
