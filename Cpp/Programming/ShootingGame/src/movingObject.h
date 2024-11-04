#pragma once

#include "object.h"
#include <iostream>

/**
 * Object moving from right to left and changes path when defined by outside class(gameMap)
*/
class movingObject : public Object {
    public:
        /**
         * Class constructor
         * @param[in] x - x coordinate
         * @param[in] y - y coordinate
        */        
        movingObject(size_t x, size_t y);
        Object *clone() const override;
        /**
         * Decides if Object is occupied
         * @returns true if so, else false
         */  
        bool isOccupied(bool changeStatus = false) override;
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
         * Displays object on map
        */
        void display() const override;
        /** 
         * Displays object when got damaged by bullet 
        */
        void displayOnHit() const override;

};
