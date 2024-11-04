#pragma once

#include <sstream>
#include <iostream>

/** Obstract class for each object on the map*/
class Object {
    public:
        /** Constructor for object Coordinates*/
        Object(size_t x, size_t y) : m_x(x),m_y(y) {}
        /**
         * Virtual destructor for object class
        */
        virtual ~Object(void) noexcept = default;
        /**
         * Creates new pointer to object
        */
        virtual Object *clone() const = 0;
        /**
         * Determines that person can step on object or not
         *  @param[in] changeStatus - determines wheater to change occupied status or not
         *  @returns true if so otherwise false
        */
        virtual bool isOccupied(bool changeStatus = false) = 0;
        /**
         * Determines if object can move or it is static
         * @returns true if so, otherwise false
        */
        virtual bool isMovable() const = 0;
        /**
         * Determines if object can be destroyed
        */
        virtual bool isBreakable() const = 0;
        /**
         *  Displays object on game map
        */
        virtual void display() const = 0;
        /**
         * Display effect when bullet hits object
        */
        virtual void displayOnHit() const = 0;

    protected:
        bool m_occupied = false;
        size_t m_x;
        size_t m_y;
};