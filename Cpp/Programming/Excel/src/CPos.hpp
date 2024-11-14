#pragma once
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>

/**
 * Structure that represents key for position in map
 * */
struct CPos {
    public:
        /**
         * Constructor to convert pair column and row, into structure
         * Absolute links can be modified, by default false
         * @param[in] column - column represented as size_t
         * @param[in] row - row represented as size_t
         * @param[in] absoluteCol - bool value if column is absolute(false by default)
         * @param[in] absoluteRow - bool value if row is absolute(false by default)
         * */
        CPos(size_t column, size_t row, bool absoluteCol = false, bool absoluteRow = false)
                : m_column(column), m_row(row), m_absoluteColumn(absoluteCol), m_absoluteRow(absoluteRow) {}

        /**
         * Constructor to convert string value of position to size_t value column and row
         * @param[in] str - position represented as std::string_view
         * */
        CPos(const std::string_view &str);

        /**
         * Operator to compare two position, main usage in std::map where position is key
         * @param[in] other - position to compare with this
         * @returns true if other is greater than this, otherwise false
         * */
        bool operator<(const CPos &other) const;

        friend std::ostream &operator<<(std::ostream &os, const CPos &other);

        /**
         * Determines if string from class constructor is correct and can be converted to integer representation
         * @param[in] str - string, which is to be validated
         * @param[out] separator - index in string where the input should be cut in half to split column and row value
         * @returns true if input string can be converted, otherwise false
         * */
        bool isValid(const std::string_view &str, size_t &separator) const;

        /**
         * Converts column number representation to equivalent string value
         * @returns converted value
         * */
        std::string convertToString() const;

        size_t m_column;
        size_t m_row;
        bool m_absoluteColumn;
        bool m_absoluteRow;
};