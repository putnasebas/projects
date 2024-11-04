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
#include <thread>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <map>
#include "person.h"
#include "computer.h"
#include "weapon.h"
#include "defaultGun.h"
#include "overpoweredGun.h"
#include "blastgun.h"
#include "player.h"
#include "coordinates.h"
#include "object.h"
#include "unbreakableObject.h"
#include "emptySpace.h"
#include "movingObject.h"
#include "loot.h"
#include "configurate.h"
#include "configurateStats.h"
#include "breakableObject.h"

/**
 * Loading and updating game map and displays it to player on screen
*/
class GameMap {
    public:
        /**
         * Default constructor
        */
        GameMap() = default;
        /**
         * Creating gamemap and taking loaded map and position from another class
         * @param[out] position - player starting position
        */
        GameMap &createMap(Coordinates &position);
        /**
         * Creates gameplaymap using only 0 and 1 for players and AI to orientate in game
         * @returns vector of vectors that symbolize map using only 0 and 1
        */
        std::vector<std::vector<int>> CreateMapPositions() const;
        /**
         * Checks if place to spawn player is not already occupied by something/someone else
         * @param[in] pos - player coordinates
         * @throw exception when position is already occupied
         * @throw exception if position is outside of the map
        */
        GameMap &spawnPlayer(const Coordinates &pos);
        /**
         * Changes occupied status on an old and a new position
         * @param[in] move - new position where person is trying to move
         * @param[in] oldpos - current position from where is person trying to move out from
        */
        GameMap &executeMove(const Coordinates &move,const Coordinates &oldPos);
        /**
         * Spawning enemies on map and taking loaded data from another class
         * @param[out] person - sending data to load to another class
         * @param[in] Gun - weapon which all AI will use in game
         * @throw exception when coordinates are outside map
         * @throw exception when place is already occupied
        */
        GameMap &spawnEnemies(std::map<Coordinates,std::shared_ptr<Person>> &persons,const std::shared_ptr<Weapon> &Gun);
        /**
         * Frees the place from killed person
         * @param[in] pos - person's positon
        */
        void RemovePerson(const Coordinates &pos);
        /**
         * Searching for every object which has moving attribute set on true
         * @param[in] person - change person's health upon contact 
         * @param[in] visited - set of visited objects so recursion will not apply on them again
        */
        GameMap &searchForMovingObj(const std::map <Coordinates,std::shared_ptr<Person>> &person,std::set <Coordinates> &visited);
        /**
         * Decides in which direction bullets will be displayed
         * @param[in] dest - which way is person shooting
         * @param[in] startingPos - starting position of person who is shooting
         * @param[out] loot - return's which loot to give player when breakable object is destroyed
         * @param[out] person - map of all persons on map, decreasing ammo when fired
         * @param[in] pos - player's position
         * @param[in] damage - amount of health to remove when damage taken
         * @param[in] range - range of person's weapon
        */
        GameMap &displayBullets(const char &dest, const Coordinates &startingPos, int &loot,
                                std::map <Coordinates,std::shared_ptr<Person>> &person,const Coordinates &pos,int damage,int range);
        /**
         * Displaying map after round end
         * @param[out] person - map of all persons on map
        */
        void display(const std::map <Coordinates,std::shared_ptr<Person>> &person) const;
        /**
         * Clearing map after game ends
        */
        GameMap &ClearMap();

    private:
        /**Loading drop chances for each buff*/
        GameMap &getDropChances();
        /**
         * Decides if bullet can continue or if it is absorbed by obstacle
         * @param[in] startingPos - starting position of person who is shooting
         * @param[in] change - new position where bullet is trying to get
         * @param[in] damage - amount of health to remove when damage taken
         * @param[out] breakCycle - breaking cycle and ending bullet's path fly when set on true, else continues flying
         * @param[out] loot - return's which loot to give player when breakable object is destroyed
         * @param[out] person - decreasing person's health by damage upon contact with bullet
         * @param[in] pos - player's position
        */
        GameMap &moveBulletPos( const Coordinates &startingPos,const Coordinates &change, int damage, bool &breakCycle,
                                int &loot, std::map <Coordinates,std::shared_ptr<Person>> &person,const Coordinates &pos);
        /**
         * Removing breaking object when destroyed by bullet
         * @param[in] pos - breaking object position
        */
        GameMap &breakObject(const Coordinates &pos);
        /**
         * Moving object if found by searchForMovingObj
         * @param[in] change - new position where to move object
         * @param[in] currpos - current position of object
         * @param[in] person - change person's health upon contact 
        */
        GameMap &moveObject(const Coordinates &change,const Coordinates &currPos,
                            const std::map <Coordinates,std::shared_ptr<Person>> &person);
        /**
         * Displaying bullet and effects on map
         * @param[in] person - map of all persons on map
         * @param[in] currpos - current bullet's position
        */
        void displayFire(const std::map <Coordinates,std::shared_ptr<Person>> &person,
                         const Coordinates &currpos,const Coordinates &pos) const;
        /**
         * Map that contains all object with their coordinates on map
        */
        std::map <Coordinates,std::shared_ptr<Object>> m_gameMap;
        /**
         * Spawn rate of each buff
        */
        int m_armorSpawnRate; 
        int m_ammoSpawnRate;
        int m_weaponSpawnRate;
        /**
         * Decides which path moving object is taking
         * true - moving obj. is moving from left to right
         * false - moving obj is moving from right to left
        */
        bool m_path = true;
        /**
         * Map size
        */
        size_t m_mapWidth;
        size_t m_mapLenght;
};