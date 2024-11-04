#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <fstream>
#include "coordinates.h"

/**
 * Absract class for every type of weapon int the game
*/
class Weapon {
    public:
        /**
         * Weapon constructor
         * @param[in] range = weapon range
         * @param[in] ammo = weapon ammo with default value
        */
        Weapon(int range,int ammo = 10000);
        /**
         * virtual destructor
        */
        virtual ~Weapon(void) noexcept = default;
        /**
         * Creates new pointer to weapon
        */
        virtual Weapon *clone() const = 0;
        /**
         * Displays Weapon projectile on game map
        */
        virtual void display() const = 0;
        /**
         * determines if weapon can pass through wall
        */
        virtual bool passThroughWall() const = 0;
        /**
         * determines if weapon will leave blast effect upon collision
        */
        virtual bool blastOnHit() const = 0;
        /**
         * Displays remaining ammo in the gun
        */
        int displayAmmo() const;
        /**
         * Changes current amount of ammo
        */
        Weapon &changeAmmo(int change);
        /**
         * Displays range of equipped weapon
        */
        int getRange() const;
        
    protected:
        int m_range;
        int m_ammo;
};