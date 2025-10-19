#ifndef BANK_STORE_H
#define BANK_STORE_H

#include <vector>
#include <memory>
#include "bank_customer.h"

extern std::vector<std::unique_ptr<BankCustomer>> g_bankAccounts;

BankCustomer* createGlobalBankCustomer(int id, const std::string &name, double balance);
BankCustomer* findBankByName(const std::string &name);

#endif
