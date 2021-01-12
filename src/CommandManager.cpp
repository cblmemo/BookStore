//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

//CommandFunction:-----\/

void stringCheck(commandType type,string cmd){

}

string splitString(string &cmd, bool keyWordFlag){

}

bool privilegeCheck(int requirements){

}

void runCommand(string cmd){

}

void logRecord(){

}

//CommandFunction:-----/\



//BasicCommand:--------\/

void reportFinance(){

}

void reportEmployee(){

}

void reportMyself(){

}

void log(){

}

//BasicCommand:--------/\



//BookCommand:---------\/

void selectBook(string ISBN){

}

void modify(string arguments){

}

void import(int quantity, double cost){

}

void show(string argument){

}

void showFinance(int times){

}

void buy(string ISBN, int quantity){

}

//BookCommand:---------/\



//UserCommand:---------\/

bool login(string userID,string password){

}

bool logout(){

}

void addAccount(const UserAccount &o){

}

void registerAccount(const UserAccount &o){

}

void deleteAccount(string userID){

}

void changePassword(string userID,string newPassword,string oldPassword){

}

//UserCommand:---------/\