#pragma once

#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "weapon.h"
#include "defaultGun.h"
#include "blastgun.h"
#include "overpoweredGun.h"
#include "person.h"
#include "computer.h"
#include "coordinates.h"
#include "object.h"
#include "unbreakableObject.h"
#include "emptySpace.h"
#include "movingObject.h"
#include "breakableObject.h"

/**
 * Loading data about map and players from files
*/
class loadData {
    public:
        /**
         * Load's map size and saves its size
         * @param[out] width - map width
         * @param[out] lenght - map lenght
        */
        loadData &getMapSize(size_t &width, size_t &lenght);
        /**
         * Load's player starting pos
         * @param[out] position - player's starting position
         * @throw exception when position outside map
        */
        loadData getPlayerPos(Coordinates &position);
        /**
         * Building map and storing it into vector of shares_ptr
         * @throw exception when file not found
         * @throw exception when invalid character found
         * @throw exception when map size reached
         * @throw exception when map does not have walls around edges
        */
        std::map<Coordinates,std::shared_ptr<Object>> buildMap();
        /**
         * Loading AI starting positions and storing them into vector of shared_ptr
         * @param[out] persons - list of AI's ment to be loaded into map
         * @param[in] Gun - weapon which is meant to be equipped by AI
         * @throws exception when file not found
         * @throws exceptions when invalid enemy found
         * @throws exception when parameter missing
        */
        loadData getEnemyPositions(std::map<Coordinates,std::shared_ptr<Person>> &persons,const std::shared_ptr<Weapon> &Gun);

    private:
        /**
         * Get's numbers from file
         * @param[in] filename - file name to read from
         * @param[out] x - x coordinate
         * @param[out] y - y coordinate
         * @throw exception when file not found
         * @throw exception when x or y missing
        */
        loadData getNumber(size_t &x, size_t &y,const std::string &filename);
        /**
         * Files to read from
        */
        std::string m_playerpos = "examples/startingPos";
        std::string m_wavefile = "examples/enemyWaves";
        std::string m_mapname = "examples/ExampleMap";
        /**
         * Map size parameters
        */
        size_t m_width;
        size_t m_lenght;
};
