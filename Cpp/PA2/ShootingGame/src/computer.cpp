#include "computer.h"

using namespace std;

Computer::Computer(int health, const Coordinates &pos)
        : Person(health, pos) {}

bool Computer::compare(const pair <size_t, Coordinates> &cmp1, const pair <size_t, Coordinates> &cmp2) {
    return cmp1.first < cmp2.first;
}

Person &Computer::changeHealth(int damage) {
    m_health -= damage;
    return *this;
}

Person *Computer::clone() const {
    return new Computer(*this);
}

Coordinates Computer::nextMove(const Coordinates &playerPos, const vector <vector<int>> &gameMap) const {
    map <Coordinates, set<Coordinates>> m_Graph;
    set <Coordinates> m_Forbidden;
    //Creating graph
    for (size_t y = 0; y < gameMap.size(); ++y) {
        for (size_t x = 0; x < gameMap[y].size(); ++x) {
            if (gameMap[y][x] == 0) m_Forbidden.emplace(Coordinates(x, y));
            //up
            if (y > 0) m_Graph[Coordinates(x, y)].emplace(Coordinates(x, y - 1));
            //down
            if (y < gameMap.size() - 1) m_Graph[Coordinates(x, y)].emplace(Coordinates(x, y + 1));
            //left
            if (x > 0) m_Graph[Coordinates(x, y)].emplace(Coordinates(x - 1, y));
            //right
            if (x < gameMap[y].size() - 1) m_Graph[Coordinates(x, y)].emplace(Coordinates(x + 1, y));
        }
    }
    queue <Coordinates> q;
    set <Coordinates> visited;
    q.emplace(m_pos);
    visited.emplace(m_pos);
    map <Coordinates, Coordinates> parent;
    while (!q.empty()) {
        auto node = q.front();
        q.pop();
        if(m_Graph.contains(node)) {
            for (const auto &neighbor: m_Graph.at(node)) {
                if ((visited.count(neighbor) == 0 && m_Forbidden.count(neighbor) == 0)
                    || (m_Forbidden.count(neighbor) != 0 && neighbor == playerPos)) {
                    q.emplace(neighbor);
                    visited.emplace(neighbor);
                    parent[neighbor] = node;
                    if (neighbor == playerPos) {
                        list <Coordinates> backtrack;
                        auto curr = playerPos;
                        while (curr != m_pos) {
                            backtrack.insert(backtrack.begin(), curr);
                            curr = parent[curr];
                        }
                        if (m_Forbidden.count(backtrack.front()) == 0)
                            return backtrack.front();
                    }
                }
            }
        }

    }
    return m_pos;
}

char Computer::shootingDirection(const Coordinates &dest, const std::vector <std::vector<int>> &gameMap) const {
    // result = destination where AI will try to shoot
    // x = default value, no bullet will be created
    char result = 'x';
    //up
    if ((dest.y < m_pos.y) && (dest.x == m_pos.x || dest.x + 1 == m_pos.x || dest.x - 1 == m_pos.x))
        result = 'w';
    //down
    if ((dest.y > m_pos.y) && (dest.x == m_pos.x || dest.x + 1 == m_pos.x || dest.x - 1 == m_pos.x))
        result = 's';
    //left
    if ((dest.x < m_pos.x) && (dest.y == m_pos.y || dest.y + 1 == m_pos.y || dest.y - 1 == m_pos.y))
        result = 'a';
    //right
    if ((dest.x > m_pos.x) && (dest.y == m_pos.y || dest.y + 1 == m_pos.y || dest.y - 1 == m_pos.y))
        result = 'd';
    //check if there is no obstacle right besides AI
    switch (result) {
        case ('w'):
            if (gameMap[m_pos.y - 1][m_pos.x] == 0)
                result = 'x';
            break;
        case ('s'):
            if (gameMap[m_pos.y + 1][m_pos.x] == 0)
                result = 'x';
            break;
        case ('a'):
            if (gameMap[m_pos.y][m_pos.x - 1] == 0)
                result = 'x';
            break;
        case ('d'):
            if (gameMap[m_pos.y][m_pos.x + 1] == 0)
                result = 'x';
            break;
        default:
            break;
    }
    return result;
}

void Computer::display() const {
    if (m_health == 1)cout << "\033[5;101m" << "F" << "\033[0m";
    else if (m_health == 2) cout << "\033[5;104m" << "E" << "\033[0m";
    else cout << "\033[5;91m" << "F" << "\033[0m";
}