//
// Created by Rainy Memory on 2021/1/12.
//

#include "Administrator.h"

void Administrator::runProgramme() {
    initialize();
    string cmd;
    while (true) {
        getline(cin, cmd);
        try {
            runCommand(cmd);
        } catch (invalidCommand &ex) {
            string logContent;
            if (ex.type == SU) logContent += "[log] Login failed.\n";
            if (ex.type == LOGOUT)logContent += "[log] Logout failed.\n";
            if(ex.type==)
            
            logContent += "Possible reason: " + ex.detail + "\n";
            logRecord(logContent, cmd);
        }
    }
}