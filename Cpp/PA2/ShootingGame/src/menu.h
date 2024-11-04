/**
 * !\mainpage Shooting game
 *  @author Sebastian Putna <putnaseb@fit.cvut.cz>
 *  Control player:
 *  Shooting and moving works with same keys
 *  [W]: move up
 *  [A]: move left
 *  [S]: move down
 *  [D]: move right
 *  [E]: skip shooting round
*/
#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include "gameloop.h"

/**
 * Basic menu for player to see stats or start a game
*/
class GameMenu {
    public:
        /**
         * Title screen, when game is started or played game has ended
         * @param[in] term - parameter for turning on/off need from player to press enter after input 
         *      and visibitility of input on the screen
        */
        void run(termios &term);
        /**
         * Leaderboard from the best player to worst
        */
        void displayStatistics();
        /**
         * Interface which player sees, nothing more than grafic desing
        */
        void displayMenu();
        /**
         * Basic manual for new players to tell them how to play the game
        */
        void manual();
        
    private:
        /**Vector which stores score of each player*/
        std::vector <std::pair<std::size_t,std::string>> scoreboard;
};