#include "menu.h"

using namespace std;

void GameMenu::run(termios &term) {
    bool play = true;
    while (play) {
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        displayMenu();
        char choice;
        cin >> choice;
        if (choice == '1') {
            cout << "\033c";
            term.c_lflag |= ECHO | ICANON;
            tcsetattr(STDIN_FILENO, TCSANOW, &term);
            string name;
            size_t score = 0;
            cout << "Enter a name:" << endl;
            getline(cin >> ws, name);
            if (!cin.eof()) {
                gameLoop game;
                bool repeat = true;
                while (repeat) game.play(score, repeat);
                scoreboard.emplace_back(score, name);
            } else throw runtime_error("Invalid name\n");
        } else if (choice == '2') this->displayStatistics();
        else if (choice == '3') play = false;
        else if (choice == '4') this->manual();
        cout << "\033c";
    }
    term.c_lflag |= ECHO | ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    cout << "\033c";
    cout << "Thanks for playing!" << endl;
}

void GameMenu::displayStatistics(void) {
    cout << "\033c";
    cout << "\033[41m" << "===============" << "\033[0m" << endl;
    sort(scoreboard.begin(), scoreboard.end(),
         [](const pair <size_t, string> &first, const pair <size_t, string> &second) {
             return first.first < second.first;
         });
    for (int i = scoreboard.size() - 1; i >= 0; --i) {
        cout << "\033[46m" << scoreboard[i].first << "\033[0m" << "  " << "\033[32m" << scoreboard[i].second
             << "\033[0m" << endl;
    }
    for (size_t i = scoreboard.size(); i < 10; ++i) {
        cout << "-------------" << endl;
    }
    cout << "\033[41m" << "===============" << "\033[0m" << endl;
    cout << "Press key to continue\n";
    char dummyVal;
    cin >> dummyVal;
}

void GameMenu::displayMenu(void) {
    cout << "\033c";
    cout << "-----------------------------------------------" << endl;
    cout << "\033[31m" << "[1] New game" << "\033[0m" << endl << "\033[33m" << "[2] Scoreboard" << "\033[0m" << endl
         << "\033[37m" << "[3] Quit" << "\033[0m" << endl << "\033[96m" << "[4] Manual" << "\033[0m" << endl;
    cout << "-----------------------------------------------" << endl;
}

void GameMenu::manual(void) {
    cout << "\033c";
    cout << "\033[41m" << "===============" << "\033[0m" << endl;
    cout << "\033[93m" << "Controling player:" << endl;
    cout << "Shooting and moving works with same keys" << endl;
    cout << "\033[96m";
    cout << "[W]: move up" << endl;
    cout << "[A]: move left" << endl;
    cout << "[S]: move down" << endl;
    cout << "[D]: move right" << endl;
    cout << "[E]: skip shooting round" << endl;
    cout << "[Q]: quit level" << endl;
    cout << "[R]: restart current level" << "\033[0m" << endl;
    cout << "\033[41m" << "===============" << "\033[0m" << endl;
    cout << "Press key to continue" << endl;
    char dummyVal;
    cin >> dummyVal;
}