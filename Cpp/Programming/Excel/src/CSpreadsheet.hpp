#pragma once
#include "CBuildExpr.hpp"

#ifndef  __PROGTEST__
constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif


/**
 * Class that serves only as user interface and all logic is implemented elsewhere
 * */
class CSpreadsheet {
    public:
        CSpreadsheet() = default;

        static unsigned capabilities();

        /**
         * loads data from istream into spreadsheet
         * @param[in] is - istream from which method loads from
         * @returns true if load successful, otherwise false
         * */
        bool load(std::istream &is);

        /**
         * saves data from spreadsheet into ostream
         * @param[in] os - ostream to which data should be saved
         * @returns true if load successful, otherwise false
         * */
        bool save(std::ostream &os) const;

        /**
         * Initializes cell on given position and tries to save expression on that position
         * @param[in] pos - specific position where expression should be stored
         * @param[in] contents - expression which will method try to save into spreadsheet
         * @returns true if successful, otherwise false (for example invalid expression)
         * */
        bool setCell(const CPos &pos, const std::string &contents);

        /**
         * gets value from given position
         * @param[in] pos - position which is to be evaluated
         * @returns evaluated value if position on spreadsheet exists, otherwise std::monostate
         *
         * */
        CValue getValue(const CPos &pos);

        /**
         * Copies data inside given rectangle to another one
         * Serves as wrapper for copyValues method in CBuildExpr class
         * */
        void copyRect(CPos dst, CPos src, int w = 1, int h = 1);

        friend std::ostream &operator<<(std::ostream &os, const CSpreadsheet &data);

    private:
        CBuildExpr m_expr;
};