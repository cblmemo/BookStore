//
// Created by Rainy Memory on 2021/1/15.
//

#include "Administrator.h"

void Administrator::runProgramme() {
    ios::sync_with_stdio(false);
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
            else if (ex.cmdType == SELECT) logContent += "[log] select failed.\n";
            else if (ex.cmdType == MODIFY) logContent += "[log] modify failed.\n";
            else if (ex.cmdType == IMPORT) logContent += "[log] import failed.\n";
            else if (ex.cmdType == SHOW) logContent += "[log] show books failed.\n";
            else if (ex.cmdType == SHOWFINANCE) logContent += "[log] show finance failed.\n";
            else if (ex.cmdType == BUY) logContent += "[log] buy books failed.\n";
            else if (ex.cmdType == REPORTFINANCE) logContent += "[log] report finance failed.\n";
            else if (ex.cmdType == REPORTEMPLOYEE) logContent += "[log] report employee failed.\n";
            else if (ex.cmdType == REPORTMYSELF) logContent += "[log] report yourself failed.\n";
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
            else if (ex.errType == DELETEALREADYLOGINACCOUNT) logContent += DELETE_ALREADY_LOGIN_ACCOUNT_MESSAGE;
            else if (ex.errType == WRONGOLDPASSWORD) logContent += WRONG_OLD_PASSWORD_MESSAGE;
            else if (ex.errType == NOBOOKSELECTED)logContent += NO_BOOK_SELECTED_MESSAGE;
            else if (ex.errType == REPEATEDKEYWORDS)logContent += REPEATED_KEYWORDS_MESSAGE;
            else if (ex.errType == INEXISTBOOK)logContent += INEXIST_BOOK_MESSAGE;
            else if (ex.errType == NOENOUGHINVENTORY)logContent += NO_ENOUGH_INVENTORY_MESSAGE;
            else if (ex.errType == BOSSREPORTITSELF)logContent += BOSS_REPORT_ITSELF_MESSAGE;
            else if (ex.errType == UNKNOWNERROR) logContent += UNKNOWN_ERROR_MESSAGE;
            logContent += "\n";
            logRecord(logContent, cmd);
        }
    }
}