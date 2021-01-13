#include <string>

using std::string;

enum commandType {
    SU, LOGOUT, USERADD, REGISTER, DELETE, PASSWD, SELECT, MODIFY, IMPORT,
    SHOW, SHOWFINANCE, BUY, REPORTFINANCE, REPORTEMPLOYEE, REPORTMYSELF, LOG,
    UNKNOWN
};

class invalidCommand {
public:
    commandType type;
    string detail;

public:
    invalidCommand(commandType _type, const char *_detail) : type(_type) {
        detail = _detail;
    }
};