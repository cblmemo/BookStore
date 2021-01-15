//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

#define debug
#define customCommand

const string argumentName[5] = {"ISBN", "name", "author", "keyword", "price"};

extern int bookNumber;
extern int totalTransaction;
extern double totalExpense;
extern double totalIncome;

extern vector<UserAccount> accountStack;
extern vector<int> selectedBookOffsetStack;

extern UnrolledLinkedList indexUserID;
extern UnrolledLinkedList indexISBN;
extern UnrolledLinkedList indexAuthor;
extern UnrolledLinkedList indexName;
extern UnrolledLinkedList indexKeyWord;

//commandFunction:-----\/

void splitKeyWord(string keyWordStr, vector<string> &keyWord) {
    stringstream ss(keyWordStr);
    string temp;
    while (getline(ss, temp, '|'))keyWord.push_back(temp);
}

argumentType getArgumentType(commandType type, string argument) {
    if (argument.size() < 6)throw invalidCommand(type, WRONGFORMAT, "arguments");
    string temp;
    for (int i = 0; i < 6; i++)temp += argument[i];
    if (temp == "-ISBN=")return ISBN_;
    if (temp == "-name=")return NAME_;
    if (argument.size() < 7)throw invalidCommand(type, WRONGFORMAT, "arguments");
    temp += argument[6];
    if (temp == "-price=")return PRICE_;
    if (argument.size() < 8)throw invalidCommand(type, WRONGFORMAT, "arguments");
    temp += argument[7];
    if (temp == "-author=")return AUTHOR_;
    if (argument.size() < 9)throw invalidCommand(type, WRONGFORMAT, "arguments");
    temp += argument[8];
    if (temp == "-keyword=")return KEYWORD_;
    return INVALID_;
}

void deleteArgumentType(string &argument, argumentType type, commandType _type) {
    string temp;
    int pos;
    for (int i = 0; i < argument.size(); i++) {
        if (argument[i] == '=') {
            pos = i;
            break;
        }
    }
    if (type != PRICE_ && type != ISBN_) {
        if (argument[pos + 1] != '\"' || argument[argument.size() - 1] != '\"')throw invalidCommand(MODIFY, WRONGFORMAT, argumentName[type]);
        for (int i = pos + 2; i < argument.size() - 1; i++)temp += argument[i];
    }
    else {
        for (int i = pos + 1; i < argument.size(); i++)temp += argument[i];
    }
    argument = temp;
}

int nowSelected() {
    if (selectedBookOffsetStack.empty())return -1;
    return selectedBookOffsetStack[selectedBookOffsetStack.size() - 1];
}

int nowAuthority() {
    if (accountStack.empty())return 0;
    return accountStack[accountStack.size() - 1].authority;
}

void authorityCheck(int requirements, commandType type) {
    int auth = nowAuthority();
    if (auth < requirements)throw invalidCommand(type, INADEQUATEAUTHORITY);
}

void argumentCheck(string argument, string argumentName, commandType type, int maxsize) {
    if (argument.empty())throw invalidCommand(type, MISSING);
    if (argument.size() > maxsize)throw invalidCommand(type, WRONGFORMAT);
}

void runCommand(string cmd) {
    if (cmd == "exit" || cmd == "quit")exit(0);
    stringstream ss(cmd);
    string cmdType;
    string logContent;
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
        argumentCheck(user_id, "user-id", USERADD, 30);
        argumentCheck(passwd, "passwd", USERADD, 30);
        argumentCheck(name, "name", USERADD, 30);
        if (_auth.empty())throw invalidCommand(USERADD, MISSING, "authority");
        if (_auth.size() > 1 || (_auth[0] != '7' && _auth[0] != '3' && _auth[0] != '1'))throw invalidCommand(USERADD, WRONGFORMAT, "authority");
        authorityCheck(3, USERADD);
        auth = _auth[0] - '0';
        authorityCheck(auth + 1, USERADD);
        vector<int> possibleOffset;
        indexUserID.findElement(user_id, possibleOffset);
        if (!possibleOffset.empty())throw invalidCommand(USERADD, USERALREADYEXIST, user_id);
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
        if (nowAuthority() == 3)staffLogRecord(cmdType, user_id + " " + passwd + " " + _auth + " " + name);
        if (auth == 3)staffRecord(user_id);
    }
    else if (cmdType == "register") {
        string user_id, passwd, name;
        ss >> user_id >> passwd >> name;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(REGISTER, REMAINS);
        argumentCheck(user_id, "user-id", REGISTER, 30);
        argumentCheck(passwd, "passwd", REGISTER, 30);
        argumentCheck(name, "name", REGISTER, 30);
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
        argumentCheck(user_id, "user-id", DELETE, 30);
        authorityCheck(7, DELETE);
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
            argumentCheck(user_id, "user-id", PASSWD, 30);
            argumentCheck(newPasswd, "new-passwd", PASSWD, 30);
            authorityCheck(1, PASSWD);
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
            argumentCheck(user_id, "user-id", PASSWD, 30);
            argumentCheck(newPasswd, "new-passwd", PASSWD, 30);
            argumentCheck(oldPasswd, "old-passwd", PASSWD, 30);
            authorityCheck(1, PASSWD);
            changePassword(user_id, newPasswd, oldPasswd);
            logContent += "[log] change password successful.\n";
            logContent += "user-id: " + user_id + "\n";
            logContent += "old-passwd: " + oldPasswd + "\n";
            logContent += "new-passwd: " + newPasswd + "\n";
            logRecord(logContent, cmd);
        }
    }
    else if (cmdType == "select") {
        string ISBN;
        ss >> ISBN;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(SELECT, REMAINS);
        argumentCheck(ISBN, "ISBN", SELECT, 20);
        authorityCheck(3, SELECT);
        selectBook(ISBN);
        logContent += "[log] select book successful.\n";
        logContent += "ISBN: " + ISBN + "\n";
        logRecord(logContent, cmd);
        staffLogRecord(cmdType, ISBN);
    }
    else if (cmdType == "modify") {
        if (nowSelected() < 0)throw invalidCommand(MODIFY, NOBOOKSELECTED);
        Book nowSelectedBook = readData<Book>(BOOK, nowSelected());
        string arg[5];
        ss >> arg[0] >> arg[1] >> arg[2] >> arg[3] >> arg[4];
        argumentType argType[5] = {ISBN_, NAME_, AUTHOR_, KEYWORD_, PRICE_};
        string arguments[6];
        int appearTime[6] = {0};
        bool haveThisArgument[6] = {false};
        for (int i = 0; i < 5; i++) {
            if (!arg[i].empty()) {
                argumentType type = getArgumentType(MODIFY, arg[i]);
                arguments[type] = arg[i];
                appearTime[type]++;
            }
        }
        for (int i = 0; i < 5; i++)if (appearTime[i] > 1)throw invalidCommand(MODIFY, WRONGFORMAT, argumentName[i]);
        for (int i = 0; i < 5; i++) {
            if (!arguments[i].empty()) {
                deleteArgumentType(arguments[i], argType[i], MODIFY);
                haveThisArgument[i] = true;
            }
        }
        
        string oldISBN, oldName, oldAuthor, keyWordStr;
        vector<string> oldKeyWord;
        //0:ISBN, 1:name, 2:author, 3:keyword, 4:price, 5:invalid
        if (haveThisArgument[0]) {
            argumentCheck(arguments[0], "ISBN", MODIFY, 20);
            oldISBN = nowSelectedBook.ISBN;
            Element oldIndex(nowSelected(), oldISBN);
            indexISBN.deleteElement(oldIndex);
            Element newIndex(nowSelected(), arguments[0]);
            indexISBN.addElement(newIndex);
            strcpy(nowSelectedBook.ISBN, arguments[0].c_str());
        }
        if (haveThisArgument[1]) {
            argumentCheck(arguments[1], "name", MODIFY, 60);
            oldName = nowSelectedBook.name;
            if (oldName.empty()) {
                Element temp(nowSelected(), arguments[1]);
                indexName.addElement(temp);
            }
            else {
                Element oldIndex(nowSelected(), oldName);
                indexName.deleteElement(oldIndex);
                Element newIndex(nowSelected(), arguments[1]);
                indexName.addElement(newIndex);
            }
            strcpy(nowSelectedBook.name, arguments[1].c_str());
        }
        if (haveThisArgument[2]) {
            argumentCheck(arguments[2], "author", MODIFY, 60);
            oldAuthor = nowSelectedBook.author;
            if (oldAuthor.empty()) {
                Element temp(nowSelected(), arguments[2]);
                indexAuthor.addElement(temp);
            }
            else {
                Element oldIndex(nowSelected(), oldAuthor);
                indexAuthor.deleteElement(oldIndex);
                Element newIndex(nowSelected(), arguments[2]);
                indexAuthor.addElement(newIndex);
            }
            strcpy(nowSelectedBook.author, arguments[2].c_str());
        }
        if (haveThisArgument[3]) {
            argumentCheck(arguments[3], "keyword", MODIFY, 60);
            keyWordStr = nowSelectedBook.keyword;
            splitKeyWord(keyWordStr, oldKeyWord);
            if (oldKeyWord.empty()) {
                vector<string> keyWord;
                splitKeyWord(arguments[3], keyWord);
                for (const string &i:keyWord) {
                    Element temp(nowSelected(), i);
                    indexKeyWord.addElement(temp);
                }
            }
            else {
                vector<string> newKeyWord;
                splitKeyWord(arguments[3], newKeyWord);
                for (const string &i:oldKeyWord) {
                    Element oldIndex(nowSelected(), i);
                    indexKeyWord.deleteElement(oldIndex);
                }
                for (const string &i:newKeyWord) {
                    Element newIndex(nowSelected(), i);
                    indexKeyWord.addElement(newIndex);
                }
            }
            strcpy(nowSelectedBook.keyword, arguments[3].c_str());
        }
        if (haveThisArgument[4]) {
            if (arguments[4].empty())throw invalidCommand(MODIFY, MISSING, "price");
            bool point = false;
            for (auto i:arguments[4]) {
                if (i < '0' || i > '9') {
                    if (i == '.' && !point)point = true;
                    else throw invalidCommand(MODIFY, WRONGFORMAT, "price");
                }
            }
            stringstream ss_(arguments[4]);
            double price;
            ss_ >> price;
            nowSelectedBook.price = price;
        }
        writeData<Book>(BOOK, nowSelectedBook, nowSelected());
        
        logContent += "[log] modify book successful.\n";
        if (haveThisArgument[0]) logContent += "ISBN: from [" + oldISBN + "] to [" + arguments[0] + "]\n";
        if (haveThisArgument[1])logContent += "name: from [" + oldName + "] to [" + arguments[1] + "]\n";
        if (haveThisArgument[2])logContent += "author: from [" + oldAuthor + "] to [" + arguments[2] + "]\n";
        if (haveThisArgument[3])logContent += "keyword: from [" + keyWordStr + "] to [" + arguments[3] + "]\n";
        logRecord(logContent, cmd);
        string argForLog;
        for (int i = 0; i < 5; i++) {
            if (haveThisArgument[i])argForLog += arguments[i];
            else argForLog += "--";
            if (i < 4)argForLog += " ";
        }
        staffLogRecord(cmdType, argForLog);
    }
    else if (cmdType == "import") {
        if (nowSelected() < 0)throw invalidCommand(IMPORT, NOBOOKSELECTED);
        Book nowSelectedBook = readData<Book>(BOOK, nowSelected());
        
        
    }
    else if (cmdType == "show") {
        string argument;
        ss >> argument;
        if (argument == "finance") {
            authorityCheck(7, SHOW);
            string times;
            //todo
        }
        else {
            authorityCheck(1, SHOW);
            if (argument.empty()) {
                if (bookNumber == 0)cout << "\n";
                else {
                    for (int i = 0; i < bookNumber; i++) {
                        Book temp = readData<Book>(BOOK, i * sizeof(Book));
                        temp.show();
                    }
                }
                logContent += "[log] show all books successful.\n";
                logRecord(logContent, cmd);
                staffLogRecord(cmdType, "");
            }
            else {
                argumentType type = getArgumentType(SHOW, argument);
                if (type > 3)throw invalidCommand(SHOW, WRONGFORMAT, "arguments");
                deleteArgumentType(argument, type, SHOW);
                vector<int> possibleOffset;
                if (type == ISBN_) indexISBN.findElement(argument, possibleOffset);
                else if (type == NAME_) indexName.findElement(argument, possibleOffset);
                else if (type == AUTHOR_)indexAuthor.findElement(argument, possibleOffset);
                else {
                    vector<string> keyWord;
                    splitKeyWord(argument, keyWord);
                    for (const string &i:keyWord)indexKeyWord.findElement(i, possibleOffset);
                }
                //todo sort as ISBN's lexicographical order
                if (possibleOffset.empty())cout << "\n";
                else {
                    for (int i:possibleOffset) {
                        Book temp = readData<Book>(BOOK, i);
                        temp.show();
                    }
                }
            }
            
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
#ifdef customCommand
    else if (cmdType == "cmd") {
        fstream fs;
        string _cmd;
        fs.open(COMMAND_FILENAME, ios::in);
        while (getline(fs, _cmd))cout << _cmd << "\n";
        cout.flush();
        fs.close();
    }
    else if (cmdType == "clear") {
        fstream fs;
        //create file
        fs.open(LOG_FILENAME, ios::out);
        fs.close();
        fs.open(COMMAND_FILENAME, ios::out);
        fs.close();
        fs.open(STAFF_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(STAFF_LOG_FILENAME, ios::out);
        fs.close();
        fs.open(BILL_FILENAME, ios::out);
        fs.close();
        fs.open(BASIC_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(USER_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(BOOK_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_USERID_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_ISBN_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_AUTHOR_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_NAME_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_KEYWORD_FILENAME, ios::out);
        fs.close();
        
        //create root account
        UserAccount root(7, "root", "root", "sjtu");
        int offset = writeData<UserAccount>(USER, root);
        Element temp(offset, "root");
        indexUserID.addElement(temp);
        
        //set basic data
        bookNumber = 0;
        totalTransaction = 0;
        totalExpense = 0;
        totalIncome = 0;
        writeBasicData<int>(BOOKNUMBER, bookNumber);
        writeBasicData<int>(TRANSACTION, totalTransaction);
        writeBasicData<double>(EXPENSE, totalExpense);
        writeBasicData<double>(INCOME, totalIncome);
    }
#endif
    else throw invalidCommand(UNKNOWN, UNKNOWNERROR);
}

void staffRecord(const string &userID) {
    char user_id[30] = {0};
    strcpy(user_id, userID.c_str());
    fstream fs;
    fs.open(STAFF_DATA_FILENAME, ios::in | ios::out | ios::binary);
    fs.seekp(0, ios::end);
    fs.write(user_id, sizeof(user_id));
    fs.close();
}

void getStaff(vector<string> &staff) {
    char user_id[30];
    string userID;
    fstream fs;
    fs.open(STAFF_DATA_FILENAME, ios::in | ios::binary);
    fs.read(user_id, sizeof(user_id));
    while (!fs.eof()) {
        userID = user_id;
        staff.push_back(userID);
        fs.read(user_id, sizeof(user_id));
    }
    fs.close();
}

void logRecord(string logContent, const string &cmd) {
    logContent += "[original command] ";
    logContent += cmd;
    logContent += "\n";
    
    string userStr;
    if (accountStack.empty()) userStr += "no user login now";
    else {
        string user_id, name;
        user_id = accountStack[accountStack.size() - 1].userID;
        name = accountStack[accountStack.size() - 1].name;
        userStr += "user-id: [" + user_id + "], name: [" + name + "]";
    }
    logContent += "[operator] " + userStr + "\n";
    
    time_t now = time(0);
    string timeStr = ctime(&now);
    logContent += "[operating time] " + timeStr + "\n";
    
    fstream fs;
    fs.open(LOG_FILENAME, ios::in | ios::out | ios::app);
    fs << logContent;
    fs.close();
    fs.open(COMMAND_FILENAME, ios::in | ios::out | ios::app);
    fs << cmd << endl;
    fs.close();

#ifdef debug
    cout << "[debug] logContent:" << endl;
    cout << logContent;
#endif
}

void staffLogRecord(const string &type, const string &arguments) {
    //"--" represent no this argument
    string staffLogContent;
    string user_id = accountStack[accountStack.size() - 1].userID;
    staffLogContent += user_id + " ";
    staffLogContent += type + " ";
    staffLogContent += arguments;
    
    fstream fs;
    fs.open(STAFF_LOG_FILENAME, ios::in | ios::out | ios::app);
    fs << staffLogContent << endl;
    fs.close();
}

//commandFunction:-----/\



//basicCommand:--------\/

void reportFinance() {
    
}

void reportEmployee() {
    cout << "[function] report employee :\n";
    vector<string> staff;
    getStaff(staff);
    int cnt = 0;
    for (const string &i:staff) {
        cout << "staff No." << ++cnt << " :\n";
        reportMyself(i);
    }
}

void reportMyself(string userID) {
    //useradd(4 arguments) select(1) modify(5) import(2)
}

void showLog() {
    fstream fs;
    string log;
    fs.open(LOG_FILENAME, ios::in);
    while (getline(fs, log))cout << log << "\n";
    cout.flush();
    fs.close();
}

//basicCommand:--------/\



//bookCommand:---------\/

void selectBook(const string &ISBN) {
    vector<int> possibleOffset;
    indexISBN.findElement(ISBN, possibleOffset);
    if (possibleOffset.empty()) {
        Book addBook(0, 0, ISBN, "", "", "");
        int offset = writeData<Book>(BOOK, addBook);
        Element temp(offset, ISBN);
        indexISBN.addElement(temp);
        selectedBookOffsetStack[selectedBookOffsetStack.size() - 1] = offset;
        bookNumber++;
        writeBasicData<int>(BOOKNUMBER, bookNumber);
    }
    else selectedBookOffsetStack[selectedBookOffsetStack.size() - 1] = possibleOffset[0];
}

void import(int quantity, double cost) {
    
}

void showFinance(int times) {
    
}

void buy(string ISBN, int quantity) {
    
}

//bookCommand:---------/\



//userCommand:---------\/

void login(const string &userID, const string &password) {
    if (password.empty()) {
        vector<int> possibleOffset;
        indexUserID.findElement(userID, possibleOffset);
        if (possibleOffset.empty())throw invalidCommand(SU, INEXISTACCOUNT, userID);
        UserAccount loginAccount(readData<UserAccount>(USER, possibleOffset[0]));
        int auth = loginAccount.authority;
        if (nowAuthority() > auth) {
            accountStack.push_back(loginAccount);
            selectedBookOffsetStack.push_back(-1);
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
            selectedBookOffsetStack.push_back(-1);
        }
        else throw invalidCommand(SU, WRONGPASSWORD);
    }
}

UserAccount logout() {
    if (accountStack.empty())throw invalidCommand(LOGOUT, NOUSERLOGIN);
    UserAccount logoutAccount = accountStack[accountStack.size() - 1];
    accountStack.erase(accountStack.end() - 1);
    selectedBookOffsetStack.erase(selectedBookOffsetStack.end() - 1);
    return logoutAccount;
}

void addAccount(const UserAccount &o, const string &userID) {
    int offset = writeData<UserAccount>(USER, o);
    Element temp(offset, userID);
    indexUserID.addElement(temp);
}

void registerAccount(const UserAccount &o, const string &userID) {
    int offset = writeData<UserAccount>(USER, o);
    Element temp(offset, userID);
    indexUserID.addElement(temp);
}

void deleteAccount(const string &userID) {
    if (userID == "root")throw invalidCommand(DELETE, DELETEROOTACCOUNT);
    vector<int> possibleOffset;
    indexUserID.findElement(userID, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(DELETE, INEXISTACCOUNT, userID);
    Element temp(possibleOffset[0], userID);
    indexUserID.deleteElement(temp);
}

void changePassword(const string &userID, const string &newPassword, const string &oldPassword) {
    vector<int> possibleOffset;
    indexUserID.findElement(userID, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(PASSWD, INEXISTACCOUNT, userID);
    UserAccount changeAccount = readData<UserAccount>(USER, possibleOffset[0]);
    if (!oldPassword.empty()) {
        string nowPassword = changeAccount.password;
        if (nowPassword != oldPassword)throw invalidCommand(PASSWD, WRONGOLDPASSWORD);
    }
    strcpy(changeAccount.password, newPassword.c_str());
    writeData<UserAccount>(USER, changeAccount, possibleOffset[0]);
}

//userCommand:---------/\