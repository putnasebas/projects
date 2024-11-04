#pragma once

#include <iostream>
#include "object.h"

/**
 * Unbreakable object creates barriers on map edges or obstacles inside game map
*/
class UnBreakableObject : public Object {
    public:
        /**
         * Class constructor
         * @param[in] x - x coordinate
         * @param[in] y - y coordinate
        */
        UnBreakableObject(size_t x, size_t y);
        Object *clone() const override;
        /**
         * Decides if Object is occupied
         * @returns true if so, else false
         */ 
        bool isOccupied(bool changeStatus = false) override;
        /**
         * Decides if Object is movable
         * @returns true if so, else false
        */
        bool isMovable()const override;
        /**
         * Decides if Object is breakable
         * @returns true if so, else false
        */
        bool isBreakable() const override;
        /**
         * Displays object on map
        */
        void display() const override;
        /** 
         * Displays object when got damaged by bullet 
        */
        void displayOnHit() const override;
                
};