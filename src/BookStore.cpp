//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStore.h"
#include "UserManager.h"
#include <vector>

using std::vector;

//basicData:
int bookNumber;
int totalTransaction;
double totalExpense;
double totalIncome;

//temporaryData:
vector<UserAccount> accountStack;
int chosenBookOffset;