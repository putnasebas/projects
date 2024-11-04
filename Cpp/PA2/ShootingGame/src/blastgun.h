#pragma once

#include "weapon.h"

/**
 * BlastGun where bullets explode uppon contact
*/
class BlastGun : public Weapon {
    public:
        /**
         * Class constructor
         * @param[in] range - range of each bullet
         * @param[in] ammo - ammo for weapon 
        */
        BlastGun(int range, int ammo = 10000);
        /**
         * Creates new pointer to BlastGun Weapon
        */
        Weapon *clone() const override;
        /**
         * Displays bullet on map
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
        bool m_blast = true;
};