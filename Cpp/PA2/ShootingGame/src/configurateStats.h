#pragma once

#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <map>
#include <string>
#include <sstream>
#include <vector>

class ConfigurateStats {
    public:
        /**
         * Loads data for each specific weapon
         * @param[out] weaponAmmo - weapon ammunition
         * @param[out] weaponRange - weapon range
         * @param[in] index - which weapon stats to load
        */
        void getWeaponStat(int &weaponAmmo, int &weaponRange, size_t index) const;
        /**
         * Load;s chances for each buff to drop
         * @param[out] m_ammoSpawnRate - spawn rate for ammunition
         * @param[out] m_weaponSpawnRate - spawn rate of better weapon
         * @param[out] m_armorSpawnRate- spawn rate of armor
         * @throws exception when unable to load file
         * @throws exceptions when invalid  chance is below 0 or higher than 100
         * @throws exceptions when not all necessary data found or invalid loot type
        */
        void getSpawnRate(int &m_ammoSpawnRate, int &m_weaponSpawnRate, int &m_armorSpawnRate) const;
        /**
         * Loads bullet damage from file
         * @throw exception when unable to load file
         * @throw exception when invalid character found
         * @throw exceptions when damage value is on 0
        */
        int getBulletDamage() const;

    private:
        /**
         * Loads weapon data
         * @param[out] weaponAmmo - weapon ammunition
         * @param[out] weaponRange - weapon range
         * @param[in] filename - file name to load from
         * @throw exception when unable to load file
         * @throw exception when numbers are below 0
         * @throw exception when not all necessary data found
        */
        void getCurrentStat(int &weaponAmmo, int &weaponRange, const std::string &filename) const;
        /**
         * File names to load from
        */
        std::string m_chanceFilename = "examples/chances";
        std::string m_defaultGunStats = "examples/WeaponDefault";
        std::string m_blastGunStats = "examples/WeaponBlast";
        std::string m_passThroughWallStats = "examples/WeaponThroughWall";
        std::string m_bulletConfig = "examples/BulletConfig";
};