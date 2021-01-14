#include "BookStoreHeader.h"

int main() {
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
            else if (ex.cmdType == DELETE) logContent += "[log] change password failed.\n";
            
            
            logContent += "Possible reason: ";
            if (ex.errType == REMAINS) logContent += REMAINS_ERROR_MESSAGE;
            else if (ex.errType == MISSING) logContent += MISSING_ERROR_MESSAGE + ex.detail;
            else if (ex.errType == WRONGFORMAT) logContent += "wrong [" + ex.detail + "] format";
            else if (ex.errType == INEXISTACCOUNT) logContent += INEXISTENT_ACCOUNT_MESSAGE;
            else if (ex.errType == INADEQUATEAUTHORITY) logContent += INADEQUATE_AUTHORITY_MESSAGE;
            else if (ex.errType == WRONGPASSWORD) logContent += WRONG_PASSWORD_MESSAGE;
            else if (ex.errType == NOUSERLOGIN) logContent += NO_USER_LOGIN_NOW_MESSAGE;
            else if (ex.errType == USERALREADYEXIST) logContent += "user-id: [" + ex.detail + "] already exist.\n";
            else if (ex.errType == WRONGOLDPASSWORD) logContent += WRONG_OLD_PASSWORD_MESSAGE;
            
            
            else if (ex.errType == UNKNOWNERROR) logContent += UNKNOWN_ERROR_MESSAGE;
            logContent += "\n";
            logRecord(logContent, cmd);
        }
    }
}
