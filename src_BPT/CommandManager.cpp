//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

//#define debug
//#define showLogContent
#define customCommand

const string argumentName[5] = {"ISBN", "name", "author", "keyword", "price"};

//basicData: (in basicData.dat)
BasicData basicData;

//temporaryData:
vector<UserAccount> accountStack;
vector<int> selectedBookOffsetStack;

//BPlusTree:
//in userData.dat
BPlusTree indexUserID(INDEX_USERID_FILENAME);
//in bookData.dat
BPlusTree indexISBN(INDEX_ISBN_FILENAME);
BPlusTree indexAuthor(INDEX_AUTHOR_FILENAME);
BPlusTree indexName(INDEX_NAME_FILENAME);
BPlusTree indexKeyWord(INDEX_KEYWORD_FILENAME);

LRUCacheMemoryPool<Book, BasicData> BookManager(BOOK_DATA_FILENAME, 100000);
LRUCacheMemoryPool<UserAccount, BasicData> UserManager(USER_DATA_FILENAME, 100000);

//commandFunction:-----\/

void initialize() {
    fstream fs(LOG_FILENAME, ios::in);
    if (fs.fail()) {
        fs.clear();
        fs.close();
        
        //create file
        fs.open(LOG_FILENAME, ios::out);
        fs.close();
        fs.open(BILL_FILENAME, ios::out);
        fs.close();
        
        //create root account
        UserAccount root(7, "root", "root", "sjtu");
        int offset = UserManager.write(root);
        Element temp(offset, "root");
        indexUserID.addElement(temp);
        
        //set basic data
        BookManager.updateExtraMessage(basicData);
    }
    else {
        //read basic data
        basicData = BookManager.readExtraMessage();
    }
}

void splitKeyWord(const string &keyWordStr, vector<string> &keyWord) {
    std::stringstream ss(keyWordStr);
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
    else for (int i = pos + 1; i < argument.size(); i++)temp += argument[i];
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

void argumentCheck(const string &argument, const string &argumentNameStr, commandType type, int maxsize) {
    if (argument.empty())throw invalidCommand(type, MISSING, argumentNameStr);
    if (argument.size() > maxsize)throw invalidCommand(type, WRONGFORMAT, argumentNameStr);
}

void runCommand(const string &cmd) {
    TokenScanner ss(cmd);
    string cmdType {};
    string remains {};
    ss >> cmdType;
    if (cmdType.empty()) {
        //do nothing...
    }
    else if (cmdType == "su") {
        string user_id, passwd;
        ss >> user_id >> passwd;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(SU, REMAINS);
        if (user_id.empty())throw invalidCommand(SU, MISSING, "user-id");
        if (passwd.empty()) {
            if (user_id.size() > 30)throw invalidCommand(SU, WRONGFORMAT, "user-id");
            login(user_id);
        }
        else {
            ss >> remains;
            if (!remains.empty())throw invalidCommand(SU, REMAINS);
            if (user_id.size() > 30)throw invalidCommand(SU, WRONGFORMAT, "user-id");
            if (passwd.size() > 30)throw invalidCommand(SU, WRONGFORMAT, "passwd");
            login(user_id, passwd);
        }
    }
    else if (cmdType == "logout") {
        ss >> remains;
        if (!remains.empty())throw invalidCommand(LOGOUT, REMAINS);
        logout();
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
    }
    else if (cmdType == "delete") {
        string user_id;
        ss >> user_id;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(DELETE, REMAINS);
        argumentCheck(user_id, "user-id", DELETE, 30);
        authorityCheck(7, DELETE);
        deleteAccount(user_id);
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
            }
            else {
                string temp = newPasswd;
                newPasswd = oldPasswd;
                oldPasswd = temp;
                argumentCheck(user_id, "user-id", PASSWD, 30);
                argumentCheck(newPasswd, "new-passwd", PASSWD, 30);
                argumentCheck(oldPasswd, "old-passwd", PASSWD, 30);
                changePassword(user_id, newPasswd, oldPasswd);
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
    }
    else if (cmdType == "modify") {
        if (nowSelected() < 0)throw invalidCommand(MODIFY, NOBOOKSELECTED);
        Book nowSelectedBook = BookManager.read(nowSelected());
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
                BookManager.update(nowSelectedBook, nowSelected());
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
            BookManager.update(nowSelectedBook, nowSelected());
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
            BookManager.update(nowSelectedBook, nowSelected());
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
            BookManager.update(nowSelectedBook, nowSelected());
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
            std::stringstream ss_(arguments[4]);
            double price;
            ss_ >> price;
            nowSelectedBook.price = price;
            BookManager.update(nowSelectedBook, nowSelected());
        }
    }
    else if (cmdType == "import") {
        string _quantity, _cost_price;
        ss >> _quantity >> _cost_price;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(IMPORT, REMAINS);
        for (auto i:_quantity)if (i > '9' || i < '0')throw invalidCommand(IMPORT, WRONGFORMAT, "quantity");
        bool point = false;
        for (auto i:_cost_price) {
            if (i < '0' || i > '9') {
                if (i == '.' && !point)point = true;
                else throw invalidCommand(IMPORT, WRONGFORMAT, "cost_price");
            }
        }
        TokenScanner ss1(_quantity);
        std::stringstream ss2(_cost_price);
        int quantity;
        double cost_price;
        ss1 >> quantity;
        ss2 >> cost_price;
        authorityCheck(3, IMPORT);
        if (quantity >= 1000000)throw invalidCommand(BUY, WRONGFORMAT, "quantity");
        import(quantity, cost_price);
        string ISBN = BookManager.read(nowSelected()).ISBN;
        string user_id = accountStack[accountStack.size() - 1].userID;
        Entry temp(ISBN, user_id, nowAuthority(), -quantity, -cost_price);
        entryRecord(temp);
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
                cout << setiosflags(ios::fixed) << setprecision(2) << "+ " << basicData.totalIncome << " - " << basicData.totalExpense << "\n";
            }
            else {
                TokenScanner ss0(_times);
                int times;
                ss0 >> times;
                if (times > basicData.totalTransaction)throw invalidCommand(SHOW, WRONGFORMAT, "times");
                double income = 0, expense = 0;
                calculateEntry(basicData.totalTransaction - times, basicData.totalTransaction, income, expense);
                cout << setiosflags(ios::fixed) << setprecision(2) << "+ " << income << " - " << expense << "\n";
            }
        }
        else {
            authorityCheck(1, SHOW);
            if (argument.empty()) {
                if (basicData.bookNumber == 0)cout << "\n";
                else {
                    vector<Book> allBook;
                    vector<int> allOffset;
                    indexISBN.traversal(allOffset);
                    for (auto i:allOffset) {
                        Book tempBook = BookManager.read(i);
                        allBook.push_back(tempBook);
                    }
                    sort(allBook.begin(), allBook.end());
                    for (Book i:allBook)i.show();
                }
            }
            else {
                ss >> remains;
                if (!remains.empty())throw invalidCommand(SHOW, REMAINS);
                argumentType type = getArgumentType(SHOW, argument);
                if (type > 3)throw invalidCommand(SHOW, WRONGFORMAT, "arguments");
                deleteArgumentType(argument, type, SHOW);
                vector<int> possibleOffset;
                if (type == ISBN_) {
                    argumentCheck(argument, "ISBN", SHOW, 20);
                    indexISBN.findElement(argument, possibleOffset);
                }
                else if (type == NAME_) {
                    argumentCheck(argument, "name", SHOW, 60);
                    indexName.findElement(argument, possibleOffset);
                }
                else if (type == AUTHOR_) {
                    argumentCheck(argument, "author", SHOW, 60);
                    indexAuthor.findElement(argument, possibleOffset);
                }
                else {
                    argumentCheck(argument, "keyword", SHOW, 60);
                    vector<string> keyWord;
                    splitKeyWord(argument, keyWord);
                    if (keyWord.size() > 1 || keyWord.empty())throw invalidCommand(SHOW, WRONGFORMAT, "arguments");
                    for (const string &i:keyWord) {
                        vector<int> temp;
                        indexKeyWord.findElement(i, temp);
                        for (int j:temp) {
                            if (find(possibleOffset.begin(), possibleOffset.end(), j) == possibleOffset.end())possibleOffset.push_back(j);
                        }
                    }
                }
                if (possibleOffset.empty())cout << "\n";
                else {
                    vector<Book> possibleBook;
                    for (int i:possibleOffset) {
                        Book temp = BookManager.read(i);
                        possibleBook.push_back(temp);
                    }
                    sort(possibleBook.begin(), possibleBook.end());
                    for (Book i:possibleBook)i.show();
                }
            }
        }
    }
    else if (cmdType == "buy") {
        string ISBN, _quantity;
        ss >> ISBN >> _quantity;
        ss >> remains;
        if (!remains.empty())throw invalidCommand(BUY, REMAINS);
        for (auto i:_quantity)if (i > '9' || i < '0')throw invalidCommand(BUY, WRONGFORMAT, "quantity");
        argumentCheck(ISBN, "ISBN", BUY, 20);
        authorityCheck(1, BUY);
        int quantity;
        TokenScanner ss0(_quantity);
        ss0 >> quantity;
        if (quantity >= 1000000)throw invalidCommand(BUY, WRONGFORMAT, "quantity");
        double singlePrice = buy(ISBN, quantity);
        double totalPrice = singlePrice * quantity;
        cout << setiosflags(ios::fixed) << setprecision(2) << totalPrice << "\n";
        string user_id = accountStack[accountStack.size() - 1].userID;
        Entry temp(ISBN, user_id, nowAuthority(), quantity, totalPrice);
        entryRecord(temp);
    }
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

//commandFunction:-----/\



//bookCommand:---------\/

void selectBook(const string &ISBN) {
    vector<int> possibleOffset;
    indexISBN.findElement(ISBN, possibleOffset);
    if (possibleOffset.empty()) {
        Book addBook(0, 0, ISBN, "", "", "");
        int offset = BookManager.write(addBook);
        Element temp(offset, ISBN);
        indexISBN.addElement(temp);
        selectedBookOffsetStack[selectedBookOffsetStack.size() - 1] = offset;
        basicData.bookNumber++;
        BookManager.updateExtraMessage(basicData);
    }
    else selectedBookOffsetStack[selectedBookOffsetStack.size() - 1] = possibleOffset[0];
}

void import(int quantity, double cost_price) {
    if (nowSelected() < 0)throw invalidCommand(IMPORT, NOBOOKSELECTED);
    Book nowSelectedBook = BookManager.read(nowSelected());
    nowSelectedBook.quantity += quantity;
    BookManager.update(nowSelectedBook, nowSelected());
    basicData.totalExpense += cost_price;
    basicData.totalTransaction++;
    BookManager.updateExtraMessage(basicData);
}

double buy(const string &ISBN, int quantity) {
    vector<int> possibleOffset;
    indexISBN.findElement(ISBN, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(BUY, INEXISTBOOK);
    Book buyBook = BookManager.read(possibleOffset[0]);
    if (buyBook.quantity < quantity)throw invalidCommand(BUY, NOENOUGHINVENTORY);
    buyBook.quantity -= quantity;
    BookManager.update(buyBook, possibleOffset[0]);
    basicData.totalIncome += quantity * buyBook.price;
    basicData.totalTransaction++;
    BookManager.updateExtraMessage(basicData);
    return buyBook.price;
}

//bookCommand:---------/\



//userCommand:---------\/

void login(const string &userID, const string &password) {
    if (password.empty()) {
        vector<int> possibleOffset;
        indexUserID.findElement(userID, possibleOffset);
        if (possibleOffset.empty())throw invalidCommand(SU, INEXISTACCOUNT, userID);
        UserAccount loginAccount = UserManager.read(possibleOffset[0]);
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
        UserAccount loginAccount = UserManager.read(possibleOffset[0]);
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
    int offset = UserManager.write(o);
    Element temp(offset, userID);
    indexUserID.addElement(temp);
}

void registerAccount(const UserAccount &o, const string &userID) {
    int offset = UserManager.write(o);
    Element temp(offset, userID);
    indexUserID.addElement(temp);
}

void deleteAccount(const string &userID) {
    if (userID == "root")throw invalidCommand(DELETE, DELETEROOTACCOUNT);
    vector<int> possibleOffset;
    indexUserID.findElement(userID, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(DELETE, INEXISTACCOUNT, userID);
    UserAccount deletedAccount = UserManager.read(possibleOffset[0]);
    if (find(accountStack.begin(), accountStack.end(), deletedAccount) != accountStack.end())throw invalidCommand(DELETE, DELETEALREADYLOGINACCOUNT);
    Element temp(possibleOffset[0], userID);
    indexUserID.deleteElement(temp);
}

void changePassword(const string &userID, const string &newPassword, const string &oldPassword) {
    vector<int> possibleOffset;
    indexUserID.findElement(userID, possibleOffset);
    if (possibleOffset.empty())throw invalidCommand(PASSWD, INEXISTACCOUNT, userID);
    UserAccount changeAccount = UserManager.read(possibleOffset[0]);
    if (!oldPassword.empty()) {
        string nowPassword = changeAccount.password;
        if (nowPassword != oldPassword)throw invalidCommand(PASSWD, WRONGOLDPASSWORD);
    }
    strcpy(changeAccount.password, newPassword.c_str());
    UserManager.update(changeAccount, possibleOffset[0]);
}

//userCommand:---------/\