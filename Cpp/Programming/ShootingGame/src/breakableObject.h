#pragma once
#include <iostream>
#include "object.h"

/**
 * Breakable object which is destroyed when damaged by bullet
*/
class breakableObject : public Object {
    public:
        /**
         * Class constructor
         * @param[in] x - x coordinate
         * @param[in] y - y coordinate
        */
        breakableObject(size_t x, size_t y);
        /**
        * Creates new pointer to breakable object
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
         * Displays object on map
        */
        void display() const override;
        /** 
         * Displays object when got damaged by bullet 
        */
        void displayOnHit() const override;

};