#include "BookStoreHeader.h"

int main() {
    initialize();
    string cmd;
    while (true) {
        getline(cin, cmd);
        try {
            runCommand(cmd);
        } catch (invalidCommand &ex) {
            string logContent;
            if (ex.cmdType == SU) {
                logContent += "[log] Login failed.\n";
                logContent += "Possible reason: ";
                if (ex.errType == REMAINS) logContent += REMAINS_ERROR_MESSAGE;
                if (ex.errType == MISSING) logContent += MISSING_ERROR_MESSAGE + ex.detail;
                if (ex.errType == INEXISTACCOUNT) logContent += INEXISTENT_ACCOUNT_MESSAGE;
                if (ex.errType == INADEQUATEAUTHORITY) logContent += INADEQUATE_AUTHORITY_MESSAGE;
                if (ex.errType == WRONGPASSWORD) logContent += WRONG_PASSWORD_MESSAGE;
                logContent += "\n";
            }
            if (ex.cmdType == LOGOUT) {
                logContent += "[log] Logout failed.\n";
            }
            //logContent += "Possible reason: " + ex.detail + "\n";
            
            logRecord(logContent, cmd);
        }
    }
}
