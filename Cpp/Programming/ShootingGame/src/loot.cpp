#include "loot.h"

using namespace std;

Loot::Loot(int armorChance, int ammoChance, int weaponChance)
        : m_armorChance(armorChance), m_ammoChance(ammoChance), m_weaponChance(weaponChance) {}

//RET 0 = ammo, 1 = armor, 2 = blastWep, 3 wallWep, -1 = nothing
int Loot::getRandomLoot() const {
    map<int, string, greater<int>> data;
    data.emplace(m_ammoChance, "ammo");
    data.emplace(m_armorChance, "armor");
    data.emplace(m_weaponChance, "weapon");
    for (const auto &it: data) {
        int chance = rand() % 100;
        if (it.second == "ammo") {
            if (chance <= it.first) return 0;
        } else if (it.second == "armor") {
            if (chance <= it.first) return 1;
        } else {
            if (chance <= it.first) {
                int specWep = rand() % 100;
                if (specWep < 60) return 2;
                else return 3;
            }
        }
    }
    return -1;
}