#pragma once

#include "weapon.h"

class PassingThroughWallGun : public Weapon {
    public:
        /**
         * Class constructor
         * @param[in] range - range of each bullet
         * @param[in] ammo - ammo for weapon 
        */
        PassingThroughWallGun(int range, int ammo = 10000);
        Weapon *clone() const override;
        /**
         * Displays bullet on map
        */
        void display() const override;
        /**
         * Decides if bullet can go though object
         * @returns true if so, else false
        */
        bool passThroughWall() const override;
        /**
         * Decides if bullets blast on contact with object
         * @returns true if so, else false
        */
        bool blastOnHit() const override;

    protected:
        bool m_passThroughWall = true;
        bool m_blast = true;
};