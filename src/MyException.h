//
// Created by Rainy Memory on 2021/1/14.
//

#ifndef BOOKSTORE_MYEXCEPTION_H
#define BOOKSTORE_MYEXCEPTION_H

#include <string>

using std::string;
using std::exception;

enum commandType {
    SU, LOGOUT, USERADD, REGISTER, DELETE, PASSWD, SELECT, MODIFY, IMPORT,
    SHOW, SHOWFINANCE, BUY, REPORTFINANCE, REPORTEMPLOYEE, REPORTMYSELF, LOG,
    UNKNOWN
};

enum errorType {
    REMAINS, MISSING, WRONGFORMAT,
    INEXISTACCOUNT,//login
    INADEQUATEAUTHORITY,//login
    WRONGPASSWORD,//login
    NOUSERLOGIN,//logout
    USERALREADYEXIST,//useradd, register
    DELETEROOTACCOUNT,//delete
    DELETEALREADYLOGINACCOUNT,//delete
    WRONGOLDPASSWORD,//passwd
    NOBOOKSELECTED,//modify, import
    REPEATEDKEYWORDS,//modify
    INEXISTBOOK,//buy
    NOENOUGHINVENTORY,//buy
    BOSSREPORTITSELF,//report myself
    UNKNOWNERROR
};

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

#endif //BOOKSTORE_MYEXCEPTION_H
