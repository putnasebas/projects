#pragma once

#include "weapon.h"
/**
 * Default gun without any upgrades
*/
class DefaultGun : public Weapon {
    public:
        /**
         * Class constructor
         * @param[in] range - range of each bullet
         * @param[in] ammo - ammo for weapon 
        */
        DefaultGun(int range,int ammo = 10000);
        Weapon *clone() const override;
        /**
         * Display bullet on map
        */
        void display() const override;
        /**
         * Decides if bullets blast on contact with object
         * @returns true if so, else false
        */
        bool blastOnHit() const override;
        /**
         * Decides if bullet can go though object
         * @returns true if so, else false
        */
        bool passThroughWall() const override;

     protected:
        bool m_passThroughWall = false;
        bool m_blast = false;    
};