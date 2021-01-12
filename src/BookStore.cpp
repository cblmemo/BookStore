//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

//basicData:(in basicData.dat)
int bookNumber;
int totalTransaction;
double totalExpense;
double totalIncome;

//temporaryData:
vector<UserAccount> accountStack;
int chosenBookOffset;

//UnrolledLinkedList:
//in userData.dat
UnrolledLinkedList indexUserID("indexUserID.dat");
//in bookData.dat
UnrolledLinkedList indexISBN("indexISBN.dat");
UnrolledLinkedList indexAuthor("indexAuthor.dat");
UnrolledLinkedList indexName("indexName.dat");
UnrolledLinkedList indexKeyWord("indexKeyWord.dat");

void initialize(){

}