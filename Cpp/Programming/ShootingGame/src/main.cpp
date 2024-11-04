#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <exception>
#include "menu.h"

using namespace std;

int main() {
    GameMenu play;
    termios term;
    try {
        play.run(term);
    } catch (const runtime_error &failed) {
        term.c_lflag |= ECHO | ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        cout << "Error: " << failed.what() << endl;
    }
    return 0;
}