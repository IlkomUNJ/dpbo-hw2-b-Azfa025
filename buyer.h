#ifndef BUYER_H
#define BUYER_H

#include <cstddef>
#include <string>
#include "bank_customer.h"

using namespace std;

class Buyer {
private:
    int id;
    string name;
    string address;
    string email;
    string phoneNumber;
    BankCustomer *account; 

public:

    Buyer() : id(0), name(""), address(""), email(""), phoneNumber(""), account(nullptr) {}

  
    Buyer(int id, const string& name, const string& address, const string& email, const string& phoneNumber, BankCustomer *account)
        : id(id), name(name), address(address), email(email), phoneNumber(phoneNumber), account(account) {}

   

    int getId() const { return id; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getEmail() const { return email; }
    string getPhoneNumber() const { return phoneNumber; }
    BankCustomer* getAccount() const { return account; }

    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setAccount(BankCustomer *acct) { account = acct; }
};

#endif // BUYER_H