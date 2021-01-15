//
// Created by Rainy Memory on 2021/1/15.
//

#include "Administrator.h"

void Administrator::runProgramme() {
    initialize();
    string cmd;
    while (getline(cin, cmd)) {
        try {
            runCommand(cmd);
        } catch (invalidCommand &ex) {
            cout << "Invalid\n";
            string logContent;
            if (ex.cmdType == SU) logContent += "[log] login failed.\n";
            else if (ex.cmdType == LOGOUT) logContent += "[log] logout failed.\n";
            else if (ex.cmdType == USERADD) logContent += "[log] useradd failed.\n";
            else if (ex.cmdType == REGISTER)logContent += "[log] register failed.\n";
            else if (ex.cmdType == DELETE) logContent += "[log] delete failed.\n";
            else if (ex.cmdType == PASSWD) logContent += "[log] change password failed.\n";
            
            else if (ex.cmdType == UNKNOWN) logContent += "[log] unknown operation failed.\n";
            
            logContent += "Possible reason: ";
            if (ex.errType == REMAINS) logContent += REMAINS_ERROR_MESSAGE;
            else if (ex.errType == MISSING) logContent += "missing [" + ex.detail + "]";
            else if (ex.errType == WRONGFORMAT) logContent += "wrong [" + ex.detail + "] format";
            else if (ex.errType == INEXISTACCOUNT) logContent += "account [" + ex.detail + "] doesn't exist";
            else if (ex.errType == INADEQUATEAUTHORITY) logContent += INADEQUATE_AUTHORITY_MESSAGE;
            else if (ex.errType == WRONGPASSWORD) logContent += WRONG_PASSWORD_MESSAGE;
            else if (ex.errType == NOUSERLOGIN) logContent += NO_USER_LOGIN_NOW_MESSAGE;
            else if (ex.errType == USERALREADYEXIST) logContent += "user-id: [" + ex.detail + "] already exist.";
            else if (ex.errType == DELETEROOTACCOUNT) logContent += DELETE_ROOT_ACCOUNT_MESSAGE;
            else if (ex.errType == WRONGOLDPASSWORD) logContent += WRONG_OLD_PASSWORD_MESSAGE;
            else if (ex.errType == NOBOOKSELECTED)logContent += NO_BOOK_SELECTED_MESSAGE;
            
            else if (ex.errType == UNKNOWNERROR) logContent += UNKNOWN_ERROR_MESSAGE;
            logContent += "\n";
            logRecord(logContent, cmd);
        }
    }
}