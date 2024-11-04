#pragma once

#include <iostream>

/** 
 * Coordinates on map, used on efficient orientation
 */
struct Coordinates {
    /**
     * Class constuctor
     * @param[in] x - position x on the map
     * @param[in] y - position y on the map
    */
    Coordinates(size_t X = 0, size_t Y = 0);
    /** 
     * Opertor checking equalitity of two coordinates
     * @returns true if same otherwise false
    */
    bool operator == (const Coordinates &compare) const;
    /** 
     * Similar usage as operator == but with exactly opposite output
     * @returns false if same otherwise true
    */
    bool operator != (const Coordinates &compare) const;
    /**
     * Operator for comparrisson which coordinates is "bigger"
     * @returns true if coordinates on the left side are smaller otherwise false
    */
    bool operator < (const Coordinates &compare) const;
    /** Ostream operator for printing output, usage mainly for debugging
     * @param[in] os - on which ostream should output go
     * @param[in] data - specific coordinates whuch are ment to be printed out
    */
    friend std::ostream &operator << (std::ostream &os, const Coordinates &data);

    /** Coordinate 'x' and 'y' */
    size_t x;
    size_t y;
};