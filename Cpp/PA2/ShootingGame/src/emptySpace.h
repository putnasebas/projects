#pragma once

#include "object.h"
#include <iostream>
#include <sstream>

/**
 * Empty space where players can step on and bullets fly freely
*/
class emptySpace : public Object {
    public:
        /**
         * Class constructor
         * @param[in] x - x coordinate
         * @param[in] y - y coordinate
        */
        emptySpace(size_t x, size_t y);
        /**
        * Creates new pointer to EmptySpace object
        */
        Object *clone() const override;
        /**
         * Decides if Object is breakable
         * @returns true if so, else false
        */
        bool isBreakable() const override;
        /**
         * Decides if Object is movable
         * @returns true if so, else false
        */
        bool isMovable() const override;
        /**
         * Decides if Object is occupied
         * @returns true if so, else false
         */  
        bool isOccupied(bool changeStatus = false) override;
        /** 
         * Displays object when got damaged by bullet 
        */
        void displayOnHit() const override;
        /**
         * Displays object on map
        */
        void display() const override; 
};