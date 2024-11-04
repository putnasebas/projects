#pragma once

#include "person.h"
#include "weapon.h"
#include "defaultGun.h"
#include "blastgun.h"
#include "overpoweredGun.h"

/**
 * Player who is being controlled by user
*/
class Player : public Person {
    public:
        /**
         * Class constructor
         * @param[in] health - person's health
         * @param[in] pos - person's starting position
        */    
        Player(int health, const Coordinates &pos);
        Person *clone() const override;
        /**
         * Takes player input and changes it into coordinates which will be returned for trying to execute
         * @param[in] currPos - player current position
         * @param[in] action - player input
         * @returns result coordinates made by player move 
        */
        Coordinates nextMove(const Coordinates &currPos,const std::vector<std::vector<int>> &gameMap) const override;
        /**
         * Prevents player from killing itself
         * @param[in] dest - neighbouring position, where player is shooting
         * @param[in] gameMap - visualization of map made out of 0 and 1
         * @returns character representing shooting destination, if move is invalid returns char 'x'
        */
        char shootingDirection(const Coordinates &dest,const std::vector<std::vector<int>> &gameMap) const override;
        /**
         * Killing player or decreasing armor when damage taken
         * @param[in] damage - amount of damage taken by bullet/object
        */
        Person &changeHealth(int damage = 1) override;
        /**
         * Displays player on game map
        */
        void display() const override;

};