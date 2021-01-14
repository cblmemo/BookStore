#include <string>

using std::string;

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
    UNKNOWNERROR
};

class invalidCommand {
public:
    commandType cmdType;
    errorType errType;
    string detail;

public:
    invalidCommand(commandType _cmdType, errorType _errType, const char *_detail = "") : cmdType(_cmdType), errType(_errType) {
        detail = _detail;
    }
};