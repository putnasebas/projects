#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <map>
#include "configurateStats.h"
#include "weapon.h"
#include "defaultGun.h"
#include "overpoweredGun.h"
#include "blastgun.h"
#include "gameplayMap.h"
#include "person.h"
#include "player.h"
#include "computer.h"

/**
 * Main game loop where every player input is executed a with a respond on that move by AI
*/
class gameLoop {
    public:
        /**
         * Loading player and map data from other classes , setting up the map and connecting all game mechanics together
         * @param[in] repeat - decides whether to reset game or not
         * @param[out] score - result score of currently playing player.
         *      It is being calculated by the amount of kills and damage taken from AI
        */
        gameLoop &play(size_t &score,bool &repeat);

    private:
        /**
         * Loading necessary data for game to start
        */
        gameLoop &loadMap();
        /** 
         * Removing killed persons after each round
         *  @param[out] score - adding score when AI is killed/player took damage
        */
        gameLoop &update(size_t &score);
        /**
         * Graphic interface when player successfully cleared the level
        */
        void VictoryScreen(size_t &score) const;
        /**
         * Graphic interface when player failed to clear the level
        */
        void DefeatScreen(size_t &score) const;
        /**
         * Calculates new position from player input
         * @param[in] move - input from keyboard
         * @returns new position
        */
        Coordinates calculatePos(char move);
        /**
         * Adding buff to a player when breakable object was destroyed and player was lucky enough to get loot
         * @param[in]  loot - says which specific loot will be applied
         * @param[in]  pos - coordinates of person who will receive buff
         * @param[in]  applyWeapon - adding specific weapon to a player
        */
        gameLoop &applyBuff(int loot,const Coordinates &pos,const std::shared_ptr<Weapon> &applyWeapon);
        /**
         * Checking if person's move is valid and if so executing it
         * @param[in] nextmove - player input
        */
        gameLoop &PersonMove(const Coordinates &nextMove);
        /**
         * Executing shooting rounds
         * @param[in]  bulletDamage - damage of bullets configured from file
         * @param[in]  direction - player input direction
         * @param[out] score - updated player score
        */
        gameLoop &shootingRound(int bulletDamage,char direction,size_t &score);
        /** 
         * Map containing player and AI controlled enemies
        */
        std::map <Coordinates,std::shared_ptr<Person>> m_persons;
        /** 
         * Parameter holding data of gameplayMap
        */
        GameMap m_customMap;
        /**
         * bullet damage(customizable from files)
        */
        int bulletDamage;
        /**
         * InGame weapons
        */
        std::shared_ptr<Weapon> defGunptr;
        std::shared_ptr<Weapon> defUpgrdptr;
        std::shared_ptr<Weapon> defOPptr;
        /**
         * Player current position
        */
        Coordinates m_position;
};