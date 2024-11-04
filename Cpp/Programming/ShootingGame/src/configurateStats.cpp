#include "configurateStats.h"
using namespace std;

void ConfigurateStats::getWeaponStat(int &weaponAmmo, int &weaponRange,size_t index) const {
    switch (index) {
        case(0): getCurrentStat(weaponAmmo,weaponRange,m_defaultGunStats);
                 break;
        case(1): getCurrentStat(weaponAmmo,weaponRange,m_blastGunStats);
                 break;
        case(2): getCurrentStat(weaponAmmo,weaponRange,m_passThroughWallStats);
                 break;
    }
}

int ConfigurateStats::getBulletDamage() const {
    ifstream file(m_bulletConfig);
    if(!file.is_open() || !file) {
        throw runtime_error("Unable to load the file.\n");
    } string line;
    stringstream str; string output;
    int value = -1;
    getline(file,line);
    str << line; str >> output;
    if(stringstream(output) >> value) {
        if(value < 1) throw runtime_error("invalid number\n");
        return value;
    } else throw runtime_error("invalid data.\n");
}

void ConfigurateStats::getCurrentStat(int &weaponAmmo, int &weaponRange,const string &filename) const {
    ifstream file(filename);
    if(!file.is_open() || !file) {
        throw runtime_error("Unable to load the file.\n");
    } string line;
    bool foundAmmo = false, foundRange = false;
    int ammo = -1; int range = -1; int count = 0;
    while (getline(file,line)) {
        stringstream str; string output;
        str << line;
        while(!str.eof()) {
            str >> output;
            switch (count) {
                case(0): if(stringstream(output) >> ammo) {
                            weaponAmmo = ammo;
                            if(ammo < 0) throw runtime_error("invalid number\n");
                            foundAmmo = true;
                         } break;
                case(1): if(stringstream(output) >> range) {
                            weaponRange = range;
                            if(range < 0) throw runtime_error("invalid number\n");
                            foundRange = true;
                         } break;
            }
        } ++count;
    } if(!foundAmmo || !foundRange) throw runtime_error("invalid data.\n");
}

void ConfigurateStats::getSpawnRate(int &m_ammoSpawnRate, int &m_weaponSpawnRate, int &m_armorSpawnRate) const {
    ifstream file(m_chanceFilename);
    if(!file.is_open() || !file) {
        throw runtime_error("Unable to load the file.\n");
    } string line;
    bool foundArmor = false, foundWeapon = false, foundAmmo = false;
    while (getline(file,line)) {
        stringstream str; string out;
        str << line; int pair = 0;
        string name; int data = -1;
        while (!str.eof()) {
            str >> out;
            if(stringstream(out) >> data) {
                if(data < 0 || data > 100) throw runtime_error("invalid number\n");
            } else stringstream(out) >> name;
            if(pair % 2 == 1) {
                if(name == "armor") {
                    m_armorSpawnRate = data;
                    foundArmor = true;
                } else if(name == "ammo"){
                    m_ammoSpawnRate = data;
                    foundAmmo = true;
                } else if(name == "weapon") {
                    m_weaponSpawnRate = data;
                    foundWeapon = true;
                } else throw runtime_error("invalid loot type name\n");
            } ++pair;
        }
    } if(!foundArmor || !foundWeapon || !foundAmmo)
        throw runtime_error("invalid data.\n");
}