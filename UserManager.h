//
// Created by Rainy Memory on 2021/1/12.
//

#ifndef BOOKSTORE_USERMANAGER_H
#define BOOKSTORE_USERMANAGER_H

#include "UnrolledLinkedList.h"

class UserAccount{
public:
    int authority;
    char userID[30];
    char name[30];
    char password[30];
};

bool login(string userID,string password="");

bool logout();

void addAccount(const UserAccount &o);

void registerAccount(const UserAccount &o);

void deleteAccount(string userID);

void changePassword(string userID,string newPassword,string oldPassword="");

#endif //BOOKSTORE_USERMANAGER_H
