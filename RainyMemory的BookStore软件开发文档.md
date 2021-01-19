# RainyMemory的BookStore软件开发文档

* 作者：夏天
* v1.0

---

## 文件存储系统

本工程共使用13个文件维护数据储存，实现中途关闭后仍可正常运行

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



* 5个由块状链表维护的索引文件
* 每个索引文件由一个块状链表维护，实现提供一个`key`返回对应的`offset`的功能，从而可以提供关键字，获取符合该关键字的元素在数据存储文件内的地址

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



定义（除块状链表与`runProgramme()`外）所有的函数，具体为

```c++
//bookStore:-----------\/

void initialize();//初始化

//bookStore:-----------/\


//commandFunction:-----\/

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



* UnrolledLinkedList类（用于维护五种索引）

```c++
//in userData.dat
UnrolledLinkedList indexUserID(INDEX_USERID_FILENAME);
//in bookData.dat
UnrolledLinkedList indexISBN(INDEX_ISBN_FILENAME);
UnrolledLinkedList indexAuthor(INDEX_AUTHOR_FILENAME);
UnrolledLinkedList indexName(INDEX_NAME_FILENAME);
UnrolledLinkedList indexKeyWord(INDEX_KEYWORD_FILENAME);
```



* 所有自定义类成员函数的实现

* `initialize()`函数



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



---

## UnrolledLinkedList .h/.cpp

实现一个块状链表类，用于维护文件系统的索引

该类中对外须实现三个接口：`addElement`、`findElement`、`deleteElement`

对内须实现两个函数：

* `splitBlock`：当一个块元素数量较多时，将其分裂为两个块

* `mergeBlock`：当两个块元素数量均较少时，将其合并为一个块



其中定义以下四个常数：

```c++
#define BLOCK_SIZE 320//每个块中元素个数
#define SPLIT_REMAIN 160//裂块后原始块的剩余元素数目
#define MAX_KEY_LENGTH 80//key的最长长度
#define MERGE_THRESHOLD 288//合块阈值
```



以及以下三个类：

```c++
//Element: 即索引的个体，存放一个key与其对应的offset，且重载小于号用于寻找元素位置
//在整个块状链表中应维护key的升序
class Element {
public:
    int offset;
    char key[MAX_KEY_LENGTH];
    
    bool operator<(const Element &o) const;
    
    explicit Element(int _offset = -1, const string &_key = "");
    
    Element(const Element &o);
    
    Element &operator=(const Element &o);
};

//Block: 块状链表中的一个块，整个块状链表就是由若干个块组成的一个链表
//一个Block中存放下一个块与上一个块在文件中的地址（不存在则为-1），目前元素个数length，长度为BLOCK_SIZE的数组，其中元素为按key升序
class Block {
public:
    int next;
    int pre;
    int length;
    Element array[BLOCK_SIZE];
    
    Block();
};

//UnrolledLinkedList: 块状链表类，负责实现三个对外接口：addElement, findElement, deleteElement
//具体实现方法见相应函数的注释
//块状链表类的文件即为若干个Block类的线性排列，其中每个块的头部存放上一个与下一个块的地址，实现一个文件系统中的链表
//这里的块的地址可以类比为内存系统中链表的指向下一个元素的指针
//第一个块（首块）默认保存在文件的开头
class UnrolledLinkedList {
    //-1 <-Block_1-> <-Block_2-> ... <-Block_n-> -1
private:
    string filename;//该块状链表类使用的文件名
    fstream fin, fout, fin_nextBlock;//几个块状链表类用于读写的fstream类对象
    //fin负责读入，fout负责写入，fin_nextBlock负责在nextBlock函数中读入
    //这里不直接使用fin读入的原因是本程序需要在fin已经打开的情况下调用nextBlock函数，而同一个fstream对象重复打开一个文件会出问题
    
    int nextBlock(int offset);//获得地址为offset的块的下一个块的地址
    
    void mergeBlock(int offset1, int offset2);//合并地址为offset1与offset2的两个块
    //实际上调用这个函数时会保证offset2是offset1的下一个块，且两个块中元素数量加起来不超过BLOCK_SIZE，因此只需实现这种情况的合并块即可
    //具体实现算法为将offset2块中所有元素都移至ofset1块中，然后删除offset2块
    //这里可以类比双向链表删除元素的算法
    
    void splitBlock(int offset);//将地址为offset的块分裂成两个块
    //具体实现算法为在文件末尾新建一个块，将offset块中多于SPLIT_REMAIN的元素移至新建块中，并将该块添加到链表内
    //这里可以类比双向链表添加元素的算法

public:
    explicit UnrolledLinkedList(const string &_filename) : filename(_filename) {}
    
    void findElement(const string &key, vector<int> &result);//寻找key值相同的元素，将其offset push_back进result中
    
    void addElement(const Element &o);//添加一个元素，注意需维护块状链表的升序
    //具体实现算法为：
    //从第一个块开始读入块的第一个Element，比较其与添加元素key的大小，不满足则按照块内存放的下个块的地址，再去找该块的第一个Element进行比较，如此找到添加元素应添加到哪一个块内
    //然后在块内利用二分查找确定元素的插入位置，将插入位置后的元素统一后移一位，再将添加元素放置到相应位置
    //若添加元素后块内数组被存满（即length==BLOCK_SIZE）则分裂被添加元素的块
    
    void deleteElement(const Element &o);//删除一个元素
    //具体实现算法为：
    //与addElement同理找到删除元素所在的块
    //在块内利用二分查找确定删除元素的位置，直接将该位置后的元素统一前移一位覆盖掉该元素即可
    //若删除元素后，被删除元素的块该块后面一个块元素个数之和小于BLOCK_SIZE，则合并这两个块
};
```























