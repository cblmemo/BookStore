# RainyMemory的BookStore软件开发文档-BPT

* 作者：夏天
* v1.0

---

## 文件存储系统

本工程共使用18个文件维护数据储存，实现中途关闭后仍可正常运行

分别为：

* 4个log相关文件：

```c++
#define LOG_FILENAME "log.dat"//存储操作日志
#define COMMAND_FILENAME "command.dat"//存储原始命令
#define STAFF_LOG_FILENAME "staffLog.dat"//存储员工的日志
#define BILL_FILENAME "bill.dat"//存储账单
```



* 4个数据存储文件：

```c++
#define BASIC_DATA_FILENAME "basicData.dat"//存放basicData（含义见下文）
#define USER_DATA_FILENAME "userData.dat"//存放所有用户信息
#define BOOK_DATA_FILENAME "bookData.dat"//存放所有书籍信息
#define STAFF_DATA_FILENAME "staffData.dat"//存放每个staff的userID，用于report employee指令
```



* 5组（10个）由B+树维护的索引文件
* 每组索引文件由一个B+树维护，由一个存放叶子节点的文件和一个存放非叶节点的文件组成，实现提供一个`key`返回对应的`offset`的功能，从而可以提供关键字，获取符合该关键字的元素在数据存储文件内的地址

```c++
#define INDEX_USERID_FILENAME "indexUserID.dat"
#define INDEX_ISBN_FILENAME "indexISBN.dat"
#define INDEX_AUTHOR_FILENAME "indexAuthor.dat"
#define INDEX_NAME_FILENAME "indexName.dat"
#define INDEX_KEYWORD_FILENAME "indexKeyWord.dat"
```



---

## main.cpp

定义一个Administrator对象，使用该对象调用`runProgramme()`函数



---

## Administrator .h/.cpp

定义一个`Administrator`类，以及一个`runProgramme()`成员函数执行`main.cpp`的功能

`runProgramme()`函数中首先调用`initialize()`函数进行初始化，之后从`stdin`接受命令调用`runCommand()`函数运行该指令

`runCommand()`函数有可能抛出异常，`runProgramme()`亦负责`catch`异常，输出`Invalid\n`并记录到日志中



---

## BookStoreHeader.h

整个项目的头文件，**每个实现文件均需include该头文件**

`#include`所有要使用的头文件

定义存储文件名称、报错信息、`basicDataType`、`saveDataType`、`argumentType`三个枚举类型、`Book`、`UserAccount`、`Entry`两个自定义类及其成员函数（构造函数、`Book`类的`show()`函数）

其中枚举类型为

```c++
enum basicDataType {
    BOOKNUMBER, TRANSACTION, EXPENSE, INCOME
};

enum saveDataType {
    USER, BOOK
};

enum argumentType {
    ISBN_, NAME_, AUTHOR_, KEYWORD_, PRICE_, INVALID_
};
```



类为

```c++
class Book {
public:
    double price = 0;
    int quantity = 0;
    char ISBN[20] = {0};
    char name[60] = {0};
    char author[60] = {0};
    char keyword[60] = {0};

public:
    Book();
    
    Book(double _price, int _quantity, const string &ISBN_, const string &_name, const string &_author, const string &_keyword);
    
    void show() const;
};

class UserAccount {
public:
    int authority = -1;
    char userID[30] = {0};
    char name[30] = {0};
    char password[30] = {0};

public:
    UserAccount();
    
    UserAccount(int _authority, const string &_userID, const string &_name, const string &_password);
};

class Entry {
public:
    char dealTime[40] = {0};
    char ISBN[20] = {0};
    char userID[30] = {0};
    int operatorAuthority = -1;
    int quantity = 0;//positive represent buy(income), negative represent import(expense)
    double totalPrice = 0;//positive represent buy(income), negative represent import(expense)

public:
    Entry();
    
    Entry(const string &ISBN_, const string &_userID, int _operatorAuthority, int _quantity, double _totalPrice);
};

```



定义（除B+树与`runProgramme()`外）所有的函数，具体为

```c++
//commandFunction:-----\/

void initialize();//初始化

void splitKeyWord(const string &keyWordStr, vector<string> &keyWord);//将由'|'分割开的关键字拆成单个关键字

argumentType getArgumentType(commandType type, const string &argument);//返回在modify与show函数中出现的参数的类型

void deleteArgumentType(string &argument, argumentType type, commandType _type);//将在modify与show函数中出现的参数删除-name=""等用于确认参数信息的部分，只留下参数本身

bool bookCompare(int offset1, int offset2);//实现地址为offset1与offset2的两本书的字典序排序，用于在show命令中将书按照IABN的字典序排序

int nowSelected();//返回当前登入用户选择的书，若无用户登录或未选择返回-1

int nowAuthority();//返回当前登入用户的权限，若无用户登录返回-1

void authorityCheck(int requirements, commandType type);//检测当前登入用户是否拥有足够的权限，若无足够权限直接抛出异常

void argumentCheck(const string &argument, const string &argumentNameStr, commandType type, int maxsize);//检测用户输入的参数是否合法(是否非空、是否在规定长度内，若不合法直接抛出异常)

void runCommand(const string &cmd);//运行指令，其中需进行鲁棒性处理

void entryRecord(const Entry &o);//记录账目

void calculateEntry(int start, int end, double &income, double &expense);//计算[begin,end)次数交易的收入与支出总额

void staffRecord(const string &userID);//记录新建员工账号的ID，即将对应的userID存放在staffData.dat中

void getStaff(vector<string> &staff);//获取所有staff的userID

void logRecord(string logContent, const string &cmd);//记录log，其中logContent需在主程序中记录相关信息

void staffLogRecord(const string &type, const string &arguments);//记录staff的log，用于report myself指令

//commandFunction:-----/\


//basicCommand:--------\/
void reportFinance();//实现report finance指令

void reportEmployee();//实现report employee指令

void reportMyself(const string &userID, bool flag);//实现report myself指令

void showLog();//实现log指令

//basicCommand:--------/\


//bookCommand:---------\/

void selectBook(const string &ISBN);//选择相应ISBN的书

void import(int quantity, double cost_price);//进货

double buy(const string &ISBN, int quantity);//买书

//bookCommand:---------/\


//userCommand:---------\/

void login(const string &userID, const string &password = "");//登录

UserAccount logout();//退出登录，return登出的账号用于log记录

void addAccount(const UserAccount &o, const string &userID);//添加用户

void registerAccount(const UserAccount &o, const string &userID);//注册用户

void deleteAccount(const string &userID);//删除账户

void changePassword(const string &userID, const string &newPassword, const string &oldPassword = "");//更改账户密码

//userCommand:---------/\
```



以及负责文件读写的模板函数，**实现需直接写在该头文件中**

```c++
// bastcDat存储在basicData.dat中，有以下四个：
// int bookNumber;
// int totalTransaction;
// double totalExpense;
// double totalIncome;

template<class T>
void writeBasicData(basicDataType type, const T &o);

template<class T>
T readBasicData(basicDataType type);

template<class T>
int writeData(saveDataType type, const T &o, int offset = -1);

template<class T>
T readData(saveDataType type, int offset);
```



---

## BookStore.cpp

* 所有自定义类成员函数的实现




---

## MyException.h

定义`commandType`、`errorType`两个枚举类型，具体为

```c++
enum commandType {
    SU, LOGOUT, USERADD, REGISTER, DELETE, PASSWD, SELECT, MODIFY, IMPORT,
    SHOW, SHOWFINANCE, BUY, REPORTFINANCE, REPORTEMPLOYEE, REPORTMYSELF, LOG, UNKNOWN
};

enum errorType {
    REMAINS, MISSING, WRONGFORMAT, INEXISTACCOUNT, INADEQUATEAUTHORITY, WRONGPASSWORD, NOUSERLOGIN,
    USERALREADYEXIST, DELETEROOTACCOUNT, WRONGOLDPASSWORD, NOBOOKSELECTED, REPEATEDKEYWORDS,
    INEXISTBOOK, NOENOUGHINVENTORY, BOSSREPORTITSELF, UNKNOWNERROR
};
```



定义从`std::exception`派生的`invalidCommand`类，用于鲁棒性检查与log记录中的报错信息，实现其构造函数

```c++
class invalidCommand : public exception {
public:
    commandType cmdType;
    errorType errType;
    string detail;

public:
    invalidCommand(commandType _cmdType, errorType _errType, const char *_detail = "") : cmdType(_cmdType), errType(_errType) {
        detail = _detail;
    }
    
    invalidCommand(commandType _cmdType, errorType _errType, const string &_detail) : cmdType(_cmdType), errType(_errType) {
        detail = _detail;
    }
};
```



---

## CommandManager.cpp

实现所有与command有关的函数，具体功能见`BookStoreHeader.h`的函数注释

存储程序运行需要的所有全局变量，包括

* basicData

```c++
int bookNumber;
int totalTransaction;
double totalExpense;
double totalIncome;
```

* temporaryData（用于存储登录的用户与用户所选择的书，由于需要支持遍历操作，均为以vector形式实现的栈）

```c++
vector<UserAccount> accountStack;
vector<int> selectedBookOffsetStack;//-1 represent not select

```

* BPlusTree类（用于维护五种索引）

```c++
//in userData.dat
BPlusTree indexUserID(INDEX_USERID_FILENAME);
//in bookData.dat
BPlusTree indexISBN(INDEX_ISBN_FILENAME);
BPlusTree indexAuthor(INDEX_AUTHOR_FILENAME);
BPlusTree indexName(INDEX_NAME_FILENAME);
BPlusTree indexKeyWord(INDEX_KEYWORD_FILENAME);
```



---

## MemoryPool.h

实现一个有空间回收功能（再利用被删除的空间）和存储额外信息功能（放在文件开头）的文件存储类，用于`BPlusTree`类的文件存储相关部分

只能存储同一个类的变量

空间回收类的具体实现为用一个`int writePoint`存储下一次写入的地址，如果删除元素就将当前的`writePoint`写入在删除元素的位置，然后更新`writePoint`为删除元素的地址，这样实现了一个内嵌的链表用来存储删除元素的位置

提供以下函数：

```c++
template<class T, class extraMessage>
class MemoryPool {
private:
    int writePoint;//下一次写入的地址，若为-1则在文件尾部写入
    const string filename;
    fstream fin, fout;
    
public:
	void clear();//清空文件

	int write(const T &o);//在writePoint位置写入一个变量，如果为-1则在文件尾部写入

	T read(int offset);//读出地址为offset的变量

	void update(const T &o, int offset);//将地址为offset的变量用传入的o更新

	void erase(int offset);//删除地址为offset的变量，在当前地址的前四个byte记录当前的writePoint，更新writePoint为当前的地址

	extraMessage readExtraMessage();//读出额外信息

	void updateExtraMessage(const extraMessage &o);//更新额外信息

	int tellWritePoint();//返回当前的写入地址，如果为-1则返回文件末地址
}
```



---

## BPlusTree.h

实现一个B+树类，用于维护索引

提供以下函数：

```
template<class key, class data, int M = 200, int L = 200>
class BPlusTree {
private:
    class basicInfo {
    public:
        int root = -1;
        int head = -1;
        int size = 0;
    };    
    enum sizeInfo {
        MAX_RECORD_NUM = L + 1,
        MIN_RECORD_NUM = (L - 1) / 2,
        MAX_KEY_NUM = M + 1,
        MIN_KEY_NUM = (M - 1) / 2,
        MAX_CHILD_NUM = M + 2
    };

private:
    MemoryPool<leafNode, basicInfo> *leafPool;
    MemoryPool<internalNode, basicInfo> *internalPool;
    basicInfo info;

private:
    class leafNode {
    public:
        int offset = -1;
        int father = -1;
        int leftBrother = -1;
        int rightBrother = -1;
        int dataNumber = 0;
        key leafKey[MAX_RECORD_NUM];
        data leafData[MAX_RECORD_NUM];
    
    public:
        void addElement(BPlusTree *tree, const key &o1, const data &o2);        
        bool deleteElement(BPlusTree *tree, const key &o1, const data &o2);        
        pair<int, key> splitNode(BPlusTree *tree);        
        void borrowLeft(BPlusTree *tree, leafNode &leftNode, internalNode &fatherNode, int index);        
        void borrowRight(BPlusTree *tree, leafNode &rightNode, internalNode &fatherNode, int index);        
        //keep left node and delete right node anyway
        //only right-most point can merge left
        void mergeLeft(BPlusTree *tree, leafNode &leftNode, internalNode &fatherNode);        
        void mergeRight(BPlusTree *tree, leafNode &rightNode, internalNode &fatherNode, int index);        
        //return father node need resize
        bool resize(BPlusTree *tree, internalNode &fatherNode, int index); \
    };
    
    class internalNode {
    public:
        int offset = -1;
        int father = -1;
        int leftBrother = -1;
        int rightBrother = -1;
        bool childNodeIsLeaf = false;
        int keyNumber = 0;
        key nodeKey[MAX_KEY_NUM];
        int childNode[MAX_CHILD_NUM] = {0};
    
    public:
        void addElement(BPlusTree *tree, const pair<int, key> &o, int pos);        
        void splitRoot(BPlusTree *tree);        
        pair<int, key> splitNode(BPlusTree *tree);        
        void borrowLeft(BPlusTree *tree, internalNode &leftNode, internalNode &fatherNode, int index);        
        void borrowRight(BPlusTree *tree, internalNode &rightNode, internalNode &fatherNode, int index);        
        //keep left node and delete right node anyway
        //only right-most point can merge left
        void mergeLeft(BPlusTree *tree, internalNode &leftNode, internalNode &fatherNode);        
        void mergeRight(BPlusTree *tree, internalNode &rightNode, internalNode &fatherNode, int index);        
        void resizeRoot(BPlusTree *tree);        
        //return father node need resize
        bool resize(BPlusTree *tree, internalNode &fatherNode, int index);
    };

private:
    void initialize(const key &o1, const data &o2);    
    //first represent child node number ++
    pair<bool, pair<int, key>> recursionInsert(int now, const key &o1, const data &o2);    
    //FIRST:
    //        first:   fatherNode offset
    //        second:  nowNode offset (to find index in fatherNode)
    //SECOND:
    //        first:   son node need resize
    //        second:  delete successfully
    pair<pair<int, int>, pair<bool, bool>> recursionErase(int now, const key &o1, const data &o2);   
    void recursionFind(int now, const key &o, vector<data> &result);

public:
    int size() const;
    bool empty() const;    
    void clear();    
    void insert(const key &o1, const data &o2);    
    //return whether erase is successful
    bool erase(const key &o1, const data &o2);    
    void find(const key &o, vector<data> &result);    
    vector<data> operator[](const key &o);    
    void traversal(vector<data> &result);
};
```















