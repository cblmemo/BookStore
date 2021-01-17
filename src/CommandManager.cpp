//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

//#define debug
//#define showLogContent
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

void splitKeyWord(const string &keyWordStr, vector<string> &keyWord) {
    stringstream ss(keyWordStr);
    string temp;
    while (getline(ss, temp, '|'))keyWord.push_back(temp);
}

argumentType getArgumentType(commandType type, const string &argument) {
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
        if (argument[pos + 1] != '\"' || argument[argument.size() - 1] != '\"')throw invalidCommand(_type, WRONGFORMAT, argumentName[type]);
        for (int i = pos + 2; i < argument.size() - 1; i++)temp += argument[i];
    }
    else {
        for (int i = pos + 1; i < argument.size(); i++)temp += argument[i];
    }
    argument = temp;
}

bool bookCompare(int offset1, int offset2) {
    Book book1 = readData<Book>(BOOK, offset1), book2 = readData<Book>(BOOK, offset2);
    return strcmp(book1.ISBN, book2.ISBN) < 0;
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

void argumentCheck(const string &argument, const string &argumentNameStr, commandType type, int maxsize) {
    if (argument.empty())throw invalidCommand(type, MISSING, argumentNameStr);
    if (argument.size() > maxsize)throw invalidCommand(type, WRONGFORMAT, argumentNameStr);
}

void runCommand(const string &cmd) {
    if (cmd == "exit" || cmd == "quit")exit(0);
    //cout << "# " << cmd << "\n";
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
        if (user_id.empty())throw invalidCommand(SU, MISSING, "user-id");
        if (passwd.empty()) {
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
        logContent += char(logoutAccount.authority + '0');
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
        logContent += char(_addAccount.authority + '0');
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
        logContent += char(_registerAccount.authority + '0');
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
        authorityCheck(1, PASSWD);
        string nowUser = accountStack[accountStack.size() - 1].userID;
        if (nowUser == "root") {
            string user_id, newPasswd, oldPasswd;
            ss >> user_id >> newPasswd >> oldPasswd;
            ss >> remains;
            if (!remains.empty())throw invalidCommand(PASSWD, REMAINS);
            if (oldPasswd.empty()) {
                argumentCheck(user_id, "user-id", PASSWD, 30);
                argumentCheck(newPasswd, "new-passwd", PASSWD, 30);
                changePassword(user_id, newPasswd);
                logContent += "[log] [root] change password successful.\n";
                logContent += "user-id: " + user_id + "\n";
                logContent += "new-passwd: " + newPasswd + "\n";
                logRecord(logContent, cmd);
            }
            else {
                string temp = newPasswd;
                newPasswd = oldPasswd;
                oldPasswd = temp;
                argumentCheck(user_id, "user-id", PASSWD, 30);
                argumentCheck(newPasswd, "new-passwd", PASSWD, 30);
                argumentCheck(oldPasswd, "old-passwd", PASSWD, 30);
                changePassword(user_id, newPasswd, oldPasswd);
                logContent += "[log] change password successful.\n";
                logContent += "user-id: " + user_id + "\n";
                logContent += "old-passwd: " + oldPasswd + "\n";
                logContent += "new-passwd: " + newPasswd + "\n";
                logRecord(logContent, cmd);
            }
        }
        else {
            string user_id, oldPasswd, newPasswd;
            ss >> user_id >> oldPasswd >> newPasswd;
            ss >> remains;
            if (!remains.empty())throw invalidCommand(PASSWD, REMAINS);
            argumentCheck(user_id, "user-id", PASSWD, 30);
            argumentCheck(newPasswd, "new-passwd", PASSWD, 30);
            argumentCheck(oldPasswd, "old-passwd", PASSWD, 30);
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
        if (nowAuthority() == 3)staffLogRecord(cmdType, ISBN);
    }
    else if (cmdType == "modify") {
        if (nowSelected() < 0)throw invalidCommand(MODIFY, NOBOOKSELECTED);
        Book nowSelectedBook = readData<Book>(BOOK, nowSelected());
        string arg[5];
        ss >> arg[0] >> arg[1] >> arg[2] >> arg[3] >> arg[4];
        ss >> remains;
        if (!remains.empty())throw invalidCommand(MODIFY, REMAINS);
        argumentType argType[5] = {ISBN_, NAME_, AUTHOR_, KEYWORD_, PRICE_};
        string arguments[6];
        int appearTime[6] = {0};
        bool haveThisArgument[6] = {false};
        for (const string &i:arg) {
            if (!i.empty()) {
                argumentType type = getArgumentType(MODIFY, i);
                arguments[type] = i;
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
            if (oldISBN == arguments[0])throw invalidCommand(MODIFY, WRONGFORMAT, "ISBN");
            vector<int> possibleOffset;
            indexISBN.findElement(arguments[0], possibleOffset);
            if (possibleOffset.empty()) {
                Element oldIndex(nowSelected(), oldISBN);
                indexISBN.deleteElement(oldIndex);
                Element newIndex(nowSelected(), arguments[0]);
                indexISBN.addElement(newIndex);
                strcpy(nowSelectedBook.ISBN, arguments[0].c_str());
                writeData<Book>(BOOK, nowSelectedBook, nowSelected());
            }
            else throw invalidCommand(MODIFY, WRONGFORMAT, "ISBN");
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
            writeData<Book>(BOOK, nowSelectedBook, nowSelected());
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
            writeData<Book>(BOOK, nowSelectedBook, nowSelected());
        }
        if (haveThisArgument[3]) {
            argumentCheck(arguments[3], "keyword", MODIFY, 60);
            keyWordStr = nowSelectedBook.keyword;
            splitKeyWord(keyWordStr, oldKeyWord);
            vector<string> newKeyWord;
            splitKeyWord(arguments[3], newKeyWord);
            sort(newKeyWord.begin(), newKeyWord.end());
            for (int i = 0; i < newKeyWord.size() - 1; i++) if (newKeyWord[i] == newKeyWord[i + 1])throw invalidCommand(MODIFY, REPEATEDKEYWORDS);
            if (!oldKeyWord.empty()) {
                for (const string &i:oldKeyWord) {
                    Element oldIndex(nowSelected(), i);
                    indexKeyWord.deleteElement(oldIndex);
                }
            }
            for (const string &i:newKeyWord) {
                Element newIndex(nowSelected(), i);
                indexKeyWord.addElement(newIndex);
            }
            strcpy(nowSelectedBook.keyword, arguments[3].c_str());
            writeData<Book>(BOOK, nowSelectedBook, nowSelected());
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
            writeData<Book>(BOOK, nowSelectedBook, nowSelected());
        }
        
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
        if (nowAuthority() == 3)staffLogRecord(cmdType, argForLog);
    }
    else if (cmdType == "import") {
        string _quantity, _cost_price;
        ss >> _quantity >> _cost_price;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(IMPORT, REMAINS);
        if (_quantity.size() > 5)throw invalidCommand(IMPORT, WRONGFORMAT, "quantity");
        for (auto i:_quantity)if (i > '9' || i < '0')throw invalidCommand(IMPORT, WRONGFORMAT, "quantity");
        bool point = false;
        for (auto i:_cost_price) {
            if (i < '0' || i > '9') {
                if (i == '.' && !point)point = true;
                else throw invalidCommand(IMPORT, WRONGFORMAT, "cost_price");
            }
        }
        stringstream ss1(_quantity), ss2(_cost_price);
        int quantity;
        double cost_price;
        ss1 >> quantity;
        ss2 >> cost_price;
        authorityCheck(3, IMPORT);
        //if (quantity >= 100000)throw invalidCommand(BUY, WRONGFORMAT, "quantity");
        import(quantity, cost_price);
        string ISBN = readData<Book>(BOOK, nowSelected()).ISBN;
        string user_id = accountStack[accountStack.size() - 1].userID;
        Entry temp(ISBN, user_id, nowAuthority(), -quantity, -cost_price);
        entryRecord(temp);
        logContent += "[log] import successful.\n";
        logContent += "ISBN: " + ISBN + "\n";
        logContent += "quantity: " + _quantity + "\n";
        logContent += "coat_price: " + _cost_price + "\n";
        logRecord(logContent, cmd);
        if (nowAuthority() == 3)staffLogRecord(cmdType, ISBN + " " + _quantity + " " + _cost_price);
    }
    else if (cmdType == "show") {
        string argument;
        ss >> argument;
        if (argument == "finance") {
            string _times;
            ss >> _times;
            ss >> remains;
            if (!remains.empty())throw invalidCommand(SHOW, REMAINS);
            authorityCheck(7, SHOW);
            if (_times.empty()) {
                cout << setiosflags(ios::fixed) << setprecision(2) << "+ " << totalIncome << " - " << totalExpense << "\n";
                logContent += "[log] show all finance successful.\n";
                logRecord(logContent, cmd);
            }
            else {
                stringstream ss0(_times);
                int times;
                ss0 >> times;
                if (times > totalTransaction)throw invalidCommand(SHOW, WRONGFORMAT, "times");
                double income = 0, expense = 0;
                calculateEntry(totalTransaction - times, totalTransaction, income, expense);
                cout << setiosflags(ios::fixed) << setprecision(2) << "+ " << income << " - " << expense << "\n";
                logContent += "[log] show recent " + _times + " times finance successful.\n";
                logContent += "times: " + _times + "\n";
                logRecord(logContent, cmd);
            }
        }
        else {
            authorityCheck(1, SHOW);
            if (argument.empty()) {
                if (bookNumber == 0)cout << "\n";
                else {
                    vector<int> possibleOffset;
                    for (int i = 0; i < bookNumber; i++) possibleOffset.push_back(i * sizeof(Book));
                    sort(possibleOffset.begin(), possibleOffset.end(), bookCompare);
                    for (int i:possibleOffset) {
                        Book temp = readData<Book>(BOOK, i);
                        temp.show();
                    }
                }
                logContent += "[log] show all books successful.\n";
                logRecord(logContent, cmd);
                if (nowAuthority() == 3)staffLogRecord(cmdType, "-- -- -- --");
            }
            else {
                ss >> remains;
                if (!remains.empty())throw invalidCommand(SHOW, REMAINS);
                argumentType type = getArgumentType(SHOW, argument);
                if (type > 3)throw invalidCommand(SHOW, WRONGFORMAT, "arguments");
                logContent += "[log] show books successful.\n";
                string argForLog;
                deleteArgumentType(argument, type, SHOW);
                vector<int> possibleOffset;
                if (type == ISBN_) {
                    argumentCheck(argument, "ISBN", SHOW, 20);
                    indexISBN.findElement(argument, possibleOffset);
                    logContent += "ISBN: [" + argument + "]\n";
                    argForLog += argument + " -- -- --";
                }
                else if (type == NAME_) {
                    argumentCheck(argument, "name", SHOW, 60);
                    indexName.findElement(argument, possibleOffset);
                    logContent += "name: [" + argument + "]\n";
                    argForLog += "-- " + argument + " -- --";
                }
                else if (type == AUTHOR_) {
                    argumentCheck(argument, "author", SHOW, 60);
                    indexAuthor.findElement(argument, possibleOffset);
                    logContent += "author: [" + argument + "]\n";
                    argForLog += "-- -- " + argument + " --";
                }
                else {
                    argumentCheck(argument, "keyword", SHOW, 60);
                    vector<string> keyWord;
                    splitKeyWord(argument, keyWord);
                    for (const string &i:keyWord) {
                        vector<int> temp;
                        indexKeyWord.findElement(i, temp);
                        for (int j:temp) {
                            if (find(possibleOffset.begin(), possibleOffset.end(), j) == possibleOffset.end())possibleOffset.push_back(j);
                        }
                    }
                    logContent += "keyword: [" + argument + "]\n";
                    argForLog += "-- -- -- " + argument;
                }
                if (possibleOffset.empty())cout << "\n";
                else {
                    //sort as ISBN's lexicographical order
                    sort(possibleOffset.begin(), possibleOffset.end(), bookCompare);
                    for (int i:possibleOffset) {
                        Book temp = readData<Book>(BOOK, i);
                        temp.show();
                    }
                }
                logRecord(logContent, cmd);
                if (nowAuthority() == 3)staffLogRecord(cmdType, argForLog);
            }
        }
    }
    else if (cmdType == "buy") {
        string ISBN, _quantity;
        ss >> ISBN >> _quantity;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(BUY, REMAINS);
        if (_quantity.size() > 5)throw invalidCommand(BUY, WRONGFORMAT, "quantity");
        for (auto i:_quantity)if (i > '9' || i < '0')throw invalidCommand(BUY, WRONGFORMAT, "quantity");
        argumentCheck(ISBN, "ISBN", BUY, 20);
        authorityCheck(1, BUY);
        int quantity;
        stringstream ss0(_quantity);
        ss0 >> quantity;
        if (quantity >= 100000)throw invalidCommand(BUY, WRONGFORMAT, "quantity");
        double singlePrice = buy(ISBN, quantity);
        double totalPrice = singlePrice * quantity;
        cout << setiosflags(ios::fixed) << setprecision(2) << totalPrice << "\n";
        string user_id = accountStack[accountStack.size() - 1].userID;
        Entry temp(ISBN, user_id, nowAuthority(), quantity, totalPrice);
        entryRecord(temp);
        logContent += "[log] buy books successful.\n";
        logContent += "ISBN: " + ISBN + "\n";
        logContent += "quantity: " + _quantity + "\n";
        logRecord(logContent, cmd);
        if (nowAuthority() == 3)staffLogRecord(cmdType, ISBN + " " + _quantity);
    }
    else if (cmdType == "report") {
        string reportType;
        ss >> reportType;
        if (reportType == "finance") {
            ss >> remains;
            if (!remains.empty())throw invalidCommand(REPORTFINANCE, REMAINS);
            authorityCheck(7, REPORTFINANCE);
            reportFinance();
        }
        else if (reportType == "employee") {
            ss >> remains;
            if (!remains.empty())throw invalidCommand(REPORTEMPLOYEE, REMAINS);
            authorityCheck(7, REPORTEMPLOYEE);
            reportEmployee();
        }
        else if (reportType == "myself") {
            ss >> remains;
            if (!remains.empty())throw invalidCommand(REPORTMYSELF, REMAINS);
            if (nowAuthority() < 3)throw invalidCommand(REPORTMYSELF, INADEQUATEAUTHORITY);
            if (nowAuthority() == 7)throw invalidCommand(REPORTMYSELF, BOSSREPORTITSELF);
            string userID = accountStack[accountStack.size() - 1].userID;
            reportMyself(userID, true);
        }
        else throw invalidCommand(UNKNOWN, UNKNOWNERROR);
    }
    else if (cmdType == "log") {
        ss >> remains;
        if (!remains.empty())throw invalidCommand(LOG, REMAINS);
        authorityCheck(7, LOG);
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

void entryRecord(const Entry &o) {
    fstream fs;
    fs.open(BILL_FILENAME, ios::in | ios::out | ios::binary);
    fs.seekp(0, ios::end);
    fs.write(reinterpret_cast<const char *>(&o), sizeof(Entry));
    fs.close();
}

void calculateEntry(int start, int end, double &income, double &expense) {
    fstream fs;
    fs.open(BILL_FILENAME, ios::in | ios::out | ios::binary);
    fs.seekg(start * sizeof(Entry));
    Entry tempEntry;
    for (int i = start; i < end; i++) {
        fs.read(reinterpret_cast<char *>(&tempEntry), sizeof(Entry));
        income += (tempEntry.totalPrice > 0 ? tempEntry.totalPrice : 0);
        expense -= (tempEntry.totalPrice < 0 ? tempEntry.totalPrice : 0);
    }
    fs.close();
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
    fs.seekg(0);
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
    
    time_t now = time(nullptr);
    string timeStr = ctime(&now);
    logContent += "[operating time] " + timeStr + "\n";
    
    fstream fs;
    fs.open(LOG_FILENAME, ios::in | ios::out | ios::app);
    fs << logContent;
    fs.close();
    fs.open(COMMAND_FILENAME, ios::in | ios::out | ios::app);
    fs << cmd << endl;
    fs.close();

#ifdef showLogContent
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
    cout << "----------------------------------------" << endl;
    Entry tempEntry;
    cout << "[function] report finance:" << endl;
    fstream fs;
    fs.open(BILL_FILENAME, ios::in | ios::binary);
    for (int i = 0; i < totalTransaction; i++) {
        fs.read(reinterpret_cast<char *>(&tempEntry), sizeof(Entry));
        cout << "[cnt] No." << i << " entry:" << endl;
        if (tempEntry.quantity < 0) {
            cout << "[import] during " << tempEntry.dealTime;
            if (tempEntry.operatorAuthority == 3)cout << "employee";
            else cout << "boss";
            cout << " [" << tempEntry.userID << "] import ISBN: [" << tempEntry.ISBN << "] for quantity: [" << -tempEntry.quantity << "]" << endl;
            cout << "[cost] " << setiosflags(ios::fixed) << setprecision(2) << -tempEntry.totalPrice << endl;
        }
        else {
            cout << "[buy] during " << tempEntry.dealTime;
            cout << "[operator] ";
            if (tempEntry.operatorAuthority == 1)cout << "customer";
            else if (tempEntry.operatorAuthority == 3)cout << "employee";
            else cout << "boss";
            cout << " [" << tempEntry.userID << "] buy ISBN: [" << tempEntry.ISBN << "[ for quantity: [" << tempEntry.quantity << "]" << endl;
            cout << "[income] " << setiosflags(ios::fixed) << setprecision(2) << tempEntry.totalPrice << endl;
        }
    }
    cout << "----------------------------------------" << endl;
    fs.close();
}

void reportEmployee() {
    cout << "----------------------------------------" << endl;
    cout << "[function] report employee:" << endl;
    vector<string> staff;
    getStaff(staff);
    int cnt = 0;
    for (const string &i:staff) {
        cout << "[cnt] staff No." << ++cnt << ":" << endl;
        reportMyself(i, false);
        cout << endl;
    }
    cout << "----------------------------------------" << endl;
}

void reportMyself(const string &userID, bool flag) {
    //useradd(4 user-id, passwd, authority, name)
    //select(1 ISBN)
    //modify(5 ISBN, name, author, keyword, price)
    //import(2 quantity, cost_price)
    //show(4 ISBN, name, author, keyword)
    //buy(2 ISBN, quantity)
    cout << "--------------------" << endl;
    if (flag) cout << "[function] report myself:" << endl;
    cout << "[user-id] " << userID << endl;
    int cnt = 0;
    string staffCmd;
    fstream fs;
    fs.open(STAFF_LOG_FILENAME, ios::in);
    while (getline(fs, staffCmd)) {
        stringstream ss(staffCmd);
        string nowUserID;
        ss >> nowUserID;
        if (nowUserID != userID)continue;
        cout << endl;
        string cmdType;
        ss >> cmdType;
        cout << "[cnt] No." << ++cnt << " operation:" << endl;
        if (cmdType == "useradd") {
            string user_id, passwd, authority, name;
            ss >> user_id >> passwd >> authority >> name;
            cout << "[useradd]" << endl;
            cout << "[user-id] " << user_id << endl;
            cout << "[passwd] " << passwd << endl;
            cout << "[authority] " << authority << endl;
            cout << "[name] " << name << endl;
        }
        else if (cmdType == "select") {
            string ISBN;
            ss >> ISBN;
            cout << "[select]" << endl;
            cout << "[ISBN] " << ISBN << endl;
        }
        else if (cmdType == "modify") {
            string ISBN, name, author, keyword, price;
            ss >> ISBN >> name >> author >> keyword >> price;
            cout << "[modify]" << endl;
            cout << "[ISBN] " << ISBN << endl;
            cout << "[name] " << name << endl;
            cout << "[author] " << author << endl;
            cout << "[keyword] " << keyword << endl;
            cout << "[price] " << price << endl;
        }
        else if (cmdType == "import") {
            string quantity, cost_price;
            ss >> quantity >> cost_price;
            cout << "[import]" << endl;
            cout << "[quantity] " << quantity << endl;
            cout << "[cost_price] " << cost_price << endl;
        }
        else if (cmdType == "show") {
            string ISBN, name, author, keyword;
            ss >> ISBN >> name >> author >> keyword;
            cout << "[show]" << endl;
            cout << "[ISBN] " << ISBN << endl;
            cout << "[name] " << name << endl;
            cout << "[author] " << author << endl;
            cout << "[keyword] " << keyword << endl;
        }
        else if (cmdType == "buy") {
            string ISBN, quantity;
            ss >> ISBN >> quantity;
            cout << "[buy]" << endl;
            cout << "[ISBN] " << ISBN << endl;
            cout << "[quantity] " << quantity << endl;
        }
    }
    if (flag)cout << "[error] you have no operation" << endl;
    else {
        if (cnt == 0)cout << "[error] this employee has no operation" << endl;
    }
    cout << "--------------------" << endl;
    fs.close();
}

void showLog() {
    string log;
    fstream fs;
    fs.open(LOG_FILENAME, ios::in);
    while (getline(fs, log))cout << log << endl;
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

void import(int quantity, double cost_price) {
    if (nowSelected() < 0)throw invalidCommand(IMPORT, NOBOOKSELECTED);
    Book nowSelectedBook = readData<Book>(BOOK, nowSelected());
    nowSelectedBook.quantity += quantity;
    writeData<Book>(BOOK, nowSelectedBook, nowSelected());
    totalExpense += cost_price;
    writeBasicData<double>(EXPENSE, totalExpense);
    totalTransaction++;
    writeBasicData<int>(TRANSACTION, totalTransaction);
}

double buy(const string &ISBN, int quantity) {
    vector<int> possibleOffset;
    indexISBN.findElement(ISBN, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(BUY, INEXISTBOOK);
    Book buyBook = readData<Book>(BOOK, possibleOffset[0]);
    if (buyBook.quantity < quantity)throw invalidCommand(BUY, NOENOUGHINVENTORY);
    buyBook.quantity -= quantity;
    writeData<Book>(BOOK, buyBook, possibleOffset[0]);
    totalIncome += quantity * buyBook.price;
    writeBasicData<double>(INCOME, totalIncome);
    totalTransaction++;
    writeBasicData<int>(TRANSACTION, totalTransaction);
    return buyBook.price;
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