//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

extern vector<UserAccount> accountStack;
extern UnrolledLinkedList indexUserID;
extern UnrolledLinkedList indexISBN;
extern UnrolledLinkedList indexAuthor;
extern UnrolledLinkedList indexName;
extern UnrolledLinkedList indexKeyWord;

//CommandFunction:-----\/

string splitKeyWord(string keyWord, vector<string> &result) {

}

int nowAuthority() {
    return accountStack[accountStack.size() - 1].authority;
}

bool authorityCheck(int requirements) {
    int auth = accountStack[accountStack.size() - 1].authority;
    return auth > requirements;
}

void runCommand(string cmd) {
    stringstream ss(cmd);
    string cmdType;
    string remains;
    string logContent;
    ss >> cmdType;
    if (cmdType == "su") {
        string user_id, passwd;
        ss >> user_id >> passwd;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(SU, REMAINS);
        if (user_id == "")throw invalidCommand(SU, MISSING, "user-id");
        if (passwd == "") {
            login(user_id);
            logContent += "[log] High authority login successful.\n";
            logContent += "user-id: " + user_id + "\n";
            logRecord(logContent, cmd);
        }
        else {
            ss >> remains;
            if (!remains.empty())throw invalidCommand(SU, REMAINS);
            login(user_id, passwd);
            logContent += "[log] Login successful.\n";
            logContent += "user-id: " + user_id + "\n";
            logContent += "passwd: " + passwd + "\n";
            logRecord(logContent, cmd);
        }
    }
    else if (cmdType == "logout") {
        ss >> remains;
        if (!remains.empty())throw invalidCommand(LOGOUT, REMAINS);
        UserAccount logoutAccount = logout();
        logContent += "[log] Logout successful.\n";
        logContent += "logout account: \n";
        logContent += "user-id: " + (string) logoutAccount.userID + "\n";
        logContent += "passwd: " + (string) logoutAccount.password + "\n";
        logContent += "name: " + (string) logoutAccount.name + "\n";
        logContent += "authority: ";
        logContent += logoutAccount.authority + '0';
        logContent += "\n";
        logRecord(logContent, cmd);
    }
    else if (cmdType == "useradd") {
        string user_id, passwd, _auth, name;
        int auth;
        ss >> user_id >> passwd >> _auth >> name;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(USERADD, REMAINS);
        if (user_id.empty())throw invalidCommand(USERADD, MISSING, "user-id");
        if (passwd.empty())throw invalidCommand(USERADD, MISSING, "passwd");
        if (_auth.empty())throw invalidCommand(USERADD, MISSING, "authority");
        if (name.empty())throw invalidCommand(USERADD, MISSING, "name");
        if (_auth.size() > 1 || (_auth[0] != '7' && _auth[0] != '3' && _auth[0] != '1'))throw invalidCommand(USERADD, WRONGFORMAT, "authority");
        
    }
    else if (cmdType == "register") {
    
    }
    else if (cmdType == "delete") {
    
    }
    else if (cmdType == "passwd") {
    
    }
    else if (cmdType == "select") {
    
    }
    else if (cmdType == "modify") {
    
    }
    else if (cmdType == "import") {
    
    }
    else if (cmdType == "show") {
        string keyWord;
        ss >> keyWord;
        if (keyWord == "finance") {
        
        }
        else {
        
        }
    }
    else if (cmdType == "buy") {
    
    }
    else if (cmdType == "report") {
        string reportType;
        ss >> reportType;
        if (reportType == "finance") {
        
        }
        else if (reportType == "employee") {
        
        }
        else if (reportType == "myself") {
        
        }
        else throw invalidCommand(UNKNOWN, UNKNOWNERROR);
    }
    else if (cmdType == "log") {
        ss >> remains;
        if (!remains.empty())throw invalidCommand(LOG, REMAINS);
        showLog();
    }
    else throw invalidCommand(UNKNOWN, UNKNOWNERROR);
}

void logRecord(string logContent, string cmd) {
    logContent += "Original command: ";
    logContent += cmd;
    logContent += "\n";
    //todo write in file
    //todo operate time
    //todo user name
    //todo arguments
}

//CommandFunction:-----/\



//BasicCommand:--------\/

void reportFinance() {

}

void reportEmployee() {

}

void reportMyself() {

}

void showLog() {

}

//BasicCommand:--------/\



//BookCommand:---------\/

void selectBook(string ISBN) {

}

void modify(string arguments) {

}

void import(int quantity, double cost) {

}

void show(string argument) {

}

void showFinance(int times) {

}

void buy(string ISBN, int quantity) {

}

//BookCommand:---------/\



//UserCommand:---------\/

void login(string userID, string password) {
    if (password == "") {
        vector<int> possibleOffset;
        indexUserID.findElement(userID, possibleOffset);
        if (possibleOffset.empty())throw invalidCommand(SU, INEXISTACCOUNT);
        UserAccount loginAccount(readData<UserAccount>(USER, possibleOffset[0]));
        int auth = loginAccount.authority;
        if (nowAuthority() > auth) accountStack.push_back(loginAccount);
        else throw invalidCommand(SU, INADEQUATEAUTHORITY);
    }
    else {
        vector<int> possibleOffset;
        indexUserID.findElement(userID, possibleOffset);
        if (possibleOffset.empty())throw invalidCommand(SU, INEXISTACCOUNT);
        UserAccount loginAccount(readData<UserAccount>(USER, possibleOffset[0]));
        string userPassword = loginAccount.password;
        if (userPassword == password)accountStack.push_back(loginAccount);
        else throw invalidCommand(SU, WRONGPASSWORD);
    }
}

UserAccount logout() {
    if (accountStack.empty())throw invalidCommand(LOGOUT, NOUSERLOGIN);
    UserAccount logoutAccount = accountStack[accountStack.size() - 1];
    accountStack.erase(accountStack.end() - 1);
    return logoutAccount;
}

void addAccount(const UserAccount &o) {

}

void registerAccount(const UserAccount &o) {

}

void deleteAccount(string userID) {

}

void changePassword(string userID, string newPassword, string oldPassword) {

}

//UserCommand:---------/\