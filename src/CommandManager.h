//
// Created by Rainy Memory on 2021/1/12.
//

#ifndef BOOKSTORE_COMMANDMANAGER_H
#define BOOKSTORE_COMMANDMANAGER_H

#include "BookStore.h"
#include "UnrolledLinkedList.h"

enum commandType {
    SU, LOGOUT, USERADD, REGISTER, DELETE, PASSWD, SELECT, MODIFY, IMPORT,
    SHOW, SHOWFINANCE, BUY, REPORTFINANCE, REPORTEMPLOYEE, REPORTMYSELF, LOG
};

void stringCheck(commandType type,string cmd);

string splitString(string &cmd, bool keyWordFlag = false);//delete and return the first word of cmd

bool privilegeCheck(int requirements);

void runCommand(string cmd);

void logRecord();

#endif //BOOKSTORE_COMMANDMANAGER_H
