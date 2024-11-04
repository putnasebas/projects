#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <thread>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <map>
#include "person.h"
#include "weapon.h"
#include "defaultGun.h"
#include "blastgun.h"
#include "overpoweredGun.h"

/**
 * Computer orders how to react on player's move
*/
class Computer : public Person {
    public:
        /**
         * Class constructor
         * @param[in] health - enemy health
         * @param[in] pos - starting position of AI
        */
        Computer(int health,const Coordinates &pos);
        /**
         * Creates new pointer to computer controlled person
        */
        Person *clone() const override;
        /**
         * Changes AI's health when damage taken
         * @param[in] damage - amount of health to remove from AI
        */
        Person &changeHealth(int damage = 1) override;
        /**
         * Finding Shortest path to player using BFS algorithm
         * @param[in] playerPos - current player position
        */
        Coordinates nextMove( const Coordinates &playerPos, const std::vector<std::vector<int>> &gameMap) const override;
        /**
         * Calculates, if shooting on player is possible or not
         * @param[in] dest - player position
         * @param[in] gameMap - map visualization using 0 and 1
         * @returns char where to shoot, if bullet cannot hit player returns 'x'
        */
        char shootingDirection(const Coordinates &dest,const std::vector<std::vector<int>> &gameMap) const override;
        /**
         * Displays AI on gamemap
        */
        void display() const override;

    protected:
        /**
         * Compare method for sorting m_options vector
         * @param[in] cmp1 - first object to compare
         * @param[in] cmp2 - second object to compare
        */
        static bool compare(const std::pair<size_t,Coordinates> &cmp1, const std::pair<size_t,Coordinates> &cmp2);

        
};