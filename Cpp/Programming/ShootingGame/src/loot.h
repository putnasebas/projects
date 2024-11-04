#pragma once

#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

/** Calculating wheater player will get loot or not and if so which one specifically*/
class Loot {
    public:
        /** 
         * Constructor taking chance of drop for each individual thing
         * @param[in] armorChance - chance for armor drop
         * @param[in] ammoChance - chance for ammo drop
         * @param[in] weaponChance - chance for weapon Drop
        */
        Loot(int armorChance, int ammoChance,int weaponChance);
        /**
         * Calculating the drop chance from data provided by class constuctor
        */
        int getRandomLoot() const; 

    private:
        /** Drop chance of each buff*/
        int m_armorChance; 
        int m_ammoChance;
        int m_weaponChance;
};