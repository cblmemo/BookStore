//
// Created by Rainy Memory on 2021/1/15.
//

#include "Administrator.h"

//#define log

void Administrator::runProgramme() {
    initialize();
    string cmd;
    while (getline(cin, cmd)) {
        if (cmd == "exit" || cmd == "quit")break;
        try {
            runCommand(cmd);
        } catch (...) {
            cout << "Invalid\n";
        }
    }
}