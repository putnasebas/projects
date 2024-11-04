#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdlib>
#include "coordinates.h"
#include "weapon.h"
#include "defaultGun.h"
#include "blastgun.h"
#include "overpoweredGun.h"

/**
 * Abstract class of Person including AI and player
*/
class Person {
    public:
        /**
         * Class constructor
         * @param[in] health - person's health
         * @param[in] pos - person's starting position
        */
        Person(int health, const Coordinates &pos);
        /**
         * Virtual destructor
        */
        virtual ~Person() noexcept = default;
        /**
         * @returns parameter played
        */
        bool hasTurn() const;
        /**
         * Changing played parameter depending if turn is over or not
        */
        Person &changeMovePref();
        /**
         * Creates pointer to person for cloning purposes
        */
        virtual Person *clone() const = 0;
        /**
         * Updating person's position
         * @param[in] move - position after successful move
         * @param[out] pos - new position to return 
        */
        Person &update(const Coordinates &move,Coordinates &pos);
        /**
         * Calculates best possible solution for shooting
        */
        virtual char shootingDirection(const Coordinates &dest,const std::vector<std::vector<int>> &gameMap) const = 0;
        /**
         * Makes next person's move on game map
        */
        virtual Coordinates nextMove(const Coordinates &move,const std::vector<std::vector<int>> &gameMap) const = 0;
        /**
         * Changes person's current health
        */
        virtual Person &changeHealth(int damage = 1) = 0;
        /**
         * Get person's current position
        */
        Coordinates getPosition() const;
        /**
         * Displaying person's stats
        */
        void displayStats() const;
        /**
         * Displays person on game map
        */
        virtual void display() const = 0;
        /**
         * @returns person's health
        */
        int showHealth() const;
        /**
         * @returns person's armor
        */
        int showArmor() const;
        /**
         * @returns person's ammo
        */
        int displayAmmo() const;
        /**
         * Equips new weapon
         * @param[in] weapon - new weapon to equip
        */
        Person &equipWeapon(const std::shared_ptr<Weapon> &weapon);
        /**
         * Decreasing/increasing player's ammo
         * @param[in] change - changing ammount of remaining ammo
        */
        Person &changeAmmo(int change);
        /**
         * @returns weapon's range
        */
        int equipWeaponRange() const;
        /**
         * Gives info if bullet can get through wall or not
         * @returns true if so, else false
        */
        bool PassThrough() const;
        /**
         * Gives info if bullet blasts or not
         * @returns true if so, else false
        */
        bool blast() const;
        /**
         * Displaying bullets of currently equipped weapon
        */
        void displayFire() const;
        /**
         * Sets armor to person
         * @param[in] set - setting amount of armor to person
        */
        Person &setArmor(int set);
        
    protected:
        /**
         * Currently equipped weapon
        */
        std::shared_ptr<Weapon> m_currWeapon;
        /**
         * Param if person played in turn or not
        */
        bool played = true;
        int m_health;
        int m_armor = 0;
        /**
         * Current position
        */
        Coordinates m_pos;
};