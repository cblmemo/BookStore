//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

extern vector<UserAccount> accountStack;
extern vector<int> chosenBookOffsetStack;

extern UnrolledLinkedList indexUserID;
extern UnrolledLinkedList indexISBN;
extern UnrolledLinkedList indexAuthor;
extern UnrolledLinkedList indexName;
extern UnrolledLinkedList indexKeyWord;

//commandFunction:-----\/

string splitKeyWord(string keyWord, vector<string> &result) {
    
}

int nowAuthority() {
    if (accountStack.empty())return 0;
    return accountStack[accountStack.size() - 1].authority;
}

bool authorityCheck(int requirements) {
    int auth = accountStack[accountStack.size() - 1].authority;
    return auth >= requirements;
}

void runCommand(string cmd) {
    string logContent;
    if (cmd == "exit") {
        logContent += "[log] exit programme.\n";
        logRecord(logContent, cmd);
        exit(0);
    }
    if (cmd == "quit") {
        logContent += "[log] quit programme.\n";
        logRecord(logContent, cmd);
        exit(0);
    }
    
    stringstream ss(cmd);
    string cmdType;
    string remains;
    ss >> cmdType;
    if (cmdType == "su") {
        string user_id, passwd;
        ss >> user_id >> passwd;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(SU, REMAINS);
        if (user_id == "")throw invalidCommand(SU, MISSING, "user-id");
        if (passwd == "") {
            if (user_id.size() > 30)throw invalidCommand(SU, WRONGFORMAT, "user-id");
            login(user_id);
            logContent += "[log] high authority login successful.\n";
            logContent += "user-id: " + user_id + "\n";
            logRecord(logContent, cmd);
        }
        else {
            ss >> remains;
            if (!remains.empty())throw invalidCommand(SU, REMAINS);
            if (user_id.size() > 30)throw invalidCommand(SU, WRONGFORMAT, "user-id");
            if (passwd.size() > 30)throw invalidCommand(SU, WRONGFORMAT, "passwd");
            login(user_id, passwd);
            logContent += "[log] login successful.\n";
            logContent += "user-id: " + user_id + "\n";
            logContent += "passwd: " + passwd + "\n";
            logRecord(logContent, cmd);
        }
    }
    else if (cmdType == "logout") {
        ss >> remains;
        if (!remains.empty())throw invalidCommand(LOGOUT, REMAINS);
        UserAccount logoutAccount = logout();
        logContent += "[log] logout successful.\n";
        logContent += "logout account: \n";
        logContent += "user-id: " + (string) logoutAccount.userID + "\n";
        //logContent += "passwd: " + (string) logoutAccount.password + "\n";
        logContent += "name: " + (string) logoutAccount.name + "\n";
        logContent += "authority: ";
        logContent += (logoutAccount.authority + '0');
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
        if (user_id.size() > 30)throw invalidCommand(USERADD, WRONGFORMAT, "user-id");
        if (passwd.size() > 30)throw invalidCommand(USERADD, WRONGFORMAT, "passwd");
        if (_auth.size() > 1 || (_auth[0] != '7' && _auth[0] != '3' && _auth[0] != '1'))throw invalidCommand(USERADD, WRONGFORMAT, "authority");
        if (name.size() > 30)throw invalidCommand(USERADD, WRONGFORMAT, "name");
        if (!authorityCheck(3))throw invalidCommand(USERADD, INADEQUATEAUTHORITY);
        vector<int> possibleOffset;
        indexUserID.findElement(user_id, possibleOffset);
        if (!possibleOffset.empty())throw invalidCommand(USERADD, USERALREADYEXIST, user_id);
        auth = _auth[0] - '0';
        UserAccount _addAccount(auth, user_id, name, passwd);
        addAccount(_addAccount, user_id);
        logContent += "[log] useradd successful.\n";
        logContent += "add account: \n";
        logContent += "user-id: " + (string) _addAccount.userID + "\n";
        logContent += "passwd: " + (string) _addAccount.password + "\n";
        logContent += "name: " + (string) _addAccount.name + "\n";
        logContent += "authority: ";
        logContent += (_addAccount.authority + '0');
        logContent += "\n";
        logRecord(logContent, cmd);
    }
    else if (cmdType == "register") {
        string user_id, passwd, name;
        ss >> user_id >> passwd >> name;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(REGISTER, REMAINS);
        if (user_id.empty())throw invalidCommand(REGISTER, MISSING, "user-id");
        if (passwd.empty())throw invalidCommand(REGISTER, MISSING, "passwd");
        if (name.empty())throw invalidCommand(REGISTER, MISSING, "name");
        if (user_id.size() > 30)throw invalidCommand(REGISTER, WRONGFORMAT, "user-id");
        if (passwd.size() > 30)throw invalidCommand(REGISTER, WRONGFORMAT, "passwd");
        if (name.size() > 30)throw invalidCommand(REGISTER, WRONGFORMAT, "name");
        vector<int> possibleOffset;
        indexUserID.findElement(user_id, possibleOffset);
        if (!possibleOffset.empty())throw invalidCommand(USERADD, USERALREADYEXIST, user_id);
        UserAccount _registerAccount(1, user_id, name, passwd);
        registerAccount(_registerAccount, user_id);
        logContent += "[log] register successful.\n";
        logContent += "register account: \n";
        logContent += "user-id: " + (string) _registerAccount.userID + "\n";
        logContent += "passwd: " + (string) _registerAccount.password + "\n";
        logContent += "name: " + (string) _registerAccount.name + "\n";
        logContent += "authority: ";
        logContent += (_registerAccount.authority + '0');
        logContent += "\n";
        logRecord(logContent, cmd);
    }
    else if (cmdType == "delete") {
        string user_id;
        ss >> user_id;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(DELETE, REMAINS);
        if (user_id.empty())throw invalidCommand(DELETE, MISSING, "user-id");
        if (user_id.size() > 30)throw invalidCommand(DELETE, WRONGFORMAT, "user-id");
        if (!authorityCheck(7))throw invalidCommand(DELETE, INADEQUATEAUTHORITY);
        deleteAccount(user_id);
        logContent += "[log] delete successful.\n";
        logContent += "delete account: \n";
        logContent += "user-id: " + user_id + "\n";
        logRecord(logContent, cmd);
    }
    else if (cmdType == "passwd") {
        string nowUser = accountStack[accountStack.size() - 1].userID;
        if (nowUser == "root") {
            string user_id, newPasswd;
            ss >> user_id >> newPasswd;
            ss >> remains;
            if (!remains.empty())throw invalidCommand(PASSWD, REMAINS);
            if (user_id.empty())throw invalidCommand(PASSWD, MISSING, "user-id");
            if (user_id.size() > 30)throw invalidCommand(PASSWD, WRONGFORMAT, "user-id");
            if (newPasswd.empty())throw invalidCommand(PASSWD, MISSING, "new-passwd");
            if (newPasswd.size() > 30)throw invalidCommand(PASSWD, WRONGFORMAT, "new-passwd");
            if (!authorityCheck(1))throw invalidCommand(PASSWD, INADEQUATEAUTHORITY);
            changePassword(user_id, newPasswd);
            logContent += "[log] [root] change password successful.\n";
            logContent += "user-id: " + user_id + "\n";
            logContent += "new-passwd: " + newPasswd + "\n";
            logRecord(logContent, cmd);
        }
        else {
            string user_id, oldPasswd, newPasswd;
            ss >> user_id >> oldPasswd >> newPasswd;
            ss >> remains;
            if (!remains.empty())throw invalidCommand(PASSWD, REMAINS);
            if (user_id.empty())throw invalidCommand(PASSWD, MISSING, "user-id");
            if (user_id.size() > 30)throw invalidCommand(PASSWD, WRONGFORMAT, "user-id");
            if (oldPasswd.empty())throw invalidCommand(PASSWD, MISSING, "old-passwd");
            if (oldPasswd.size() > 30)throw invalidCommand(PASSWD, WRONGFORMAT, "old-passwd");
            if (newPasswd.empty())throw invalidCommand(PASSWD, MISSING, "new-passwd");
            if (newPasswd.size() > 30)throw invalidCommand(PASSWD, WRONGFORMAT, "new-passwd");
            if (!authorityCheck(1))throw invalidCommand(PASSWD, INADEQUATEAUTHORITY);
            changePassword(user_id, newPasswd, oldPasswd);
            logContent += "[log] change password successful.\n";
            logContent += "user-id: " + user_id + "\n";
            logContent += "old-passwd: " + oldPasswd + "\n";
            logContent += "new-passwd: " + newPasswd + "\n";
            logRecord(logContent, cmd);
        }
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
    //logContent += "\n";
    cout << "[debug] logContent:" << endl;
    cout << logContent << endl;
    //todo write in file
    //todo operate time
    //todo user name
    //todo arguments
}

//commandFunction:-----/\



//basicCommand:--------\/

void reportFinance() {
    
}

void reportEmployee() {
    
}

void reportMyself() {
    
}

void showLog() {
    
}

//basicCommand:--------/\



//bookCommand:---------\/

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

//bookCommand:---------/\



//userCommand:---------\/

void login(string userID, string password) {
    if (password == "") {
        vector<int> possibleOffset;
        indexUserID.findElement(userID, possibleOffset);
        if (possibleOffset.empty())throw invalidCommand(SU, INEXISTACCOUNT, userID);
        UserAccount loginAccount(readData<UserAccount>(USER, possibleOffset[0]));
        int auth = loginAccount.authority;
        if (nowAuthority() > auth) {
            accountStack.push_back(loginAccount);
            chosenBookOffsetStack.push_back(-1);
        }
        else throw invalidCommand(SU, INADEQUATEAUTHORITY);
    }
    else {
        vector<int> possibleOffset;
        indexUserID.findElement(userID, possibleOffset);
        if (possibleOffset.empty())throw invalidCommand(SU, INEXISTACCOUNT, userID);
        UserAccount loginAccount(readData<UserAccount>(USER, possibleOffset[0]));
        string userPassword = loginAccount.password;
        if (userPassword == password) {
            accountStack.push_back(loginAccount);
            chosenBookOffsetStack.push_back(-1);
        }
        else throw invalidCommand(SU, WRONGPASSWORD);
    }
}

UserAccount logout() {
    if (accountStack.empty())throw invalidCommand(LOGOUT, NOUSERLOGIN);
    UserAccount logoutAccount = accountStack[accountStack.size() - 1];
    accountStack.erase(accountStack.end() - 1);
    chosenBookOffsetStack.erase(chosenBookOffsetStack.end() - 1);
    return logoutAccount;
}

void addAccount(const UserAccount &o, string userID) {
    int offset = writeData<UserAccount>(USER, o);
    Element temp(offset, userID);
    indexUserID.addElement(temp);
}

void registerAccount(const UserAccount &o, string userID) {
    int offset = writeData<UserAccount>(USER, o);
    Element temp(offset, userID);
    indexUserID.addElement(temp);
}

void deleteAccount(string userID) {
    if (userID == "root")throw invalidCommand(DELETE, DELETEROOTACCOUNT);
    vector<int> possibleOffset;
    indexUserID.findElement(userID, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(DELETE, INEXISTACCOUNT, userID);
    Element temp(possibleOffset[0], userID);
    indexUserID.deleteElement(temp);
}

void changePassword(string userID, string newPassword, string oldPassword) {
    vector<int> possibleOffset;
    indexUserID.findElement(userID, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(PASSWD, INEXISTACCOUNT, userID);
    UserAccount changeAccount = readData<UserAccount>(USER, possibleOffset[0]);
    if (oldPassword != "") {
        string nowPassword = changeAccount.password;
        if (nowPassword != oldPassword)throw invalidCommand(PASSWD, WRONGOLDPASSWORD);
    }
    strcpy(changeAccount.password, newPassword.c_str());
    writeData<UserAccount>(USER, changeAccount, possibleOffset[0]);
}

//userCommand:---------/\