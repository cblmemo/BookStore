//
// Created by Rainy Memory on 2021/1/15.
//

#include "Administrator.h"

//#define log

void Administrator::runProgramme() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//    cout.tie(nullptr);
    initialize();
    string cmd;
    while (getline(cin, cmd)) {
        if (cmd == "exit" || cmd == "quit")break;
        try {
            runCommand(cmd);
        } catch (invalidCommand &ex) {
            cout << "Invalid\n";
        }
    }
}