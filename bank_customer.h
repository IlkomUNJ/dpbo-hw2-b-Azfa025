#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

class BankCustomer {
private:
    int id;
    string name;
    double balance;
public:
    struct Transaction {
        string type;  // "DEPOSIT" or "WITHDRAW"
        double amount;
        double balanceAfter;
        time_t timestamp; // epoch
    };
    
private:
    vector<Transaction> transactionHistory;

public:
    BankCustomer(int id, const string& name, double balance) : id(id), name(name), balance(balance) {
        this->id = id;
        this->name = name;
        this->balance = balance;
    }

    int getId() const;
    string getName() const;
    double getBalance() const;

    void printInfo() const;
    void setName(const string& name);
    void setBalance(double balance);
    void addBalance(double amount);
    bool withdrawBalance(double amount);
    void printTransactionHistory() const;
    const std::vector<Transaction>& getTransactionHistory() const { return transactionHistory; }
};

#endif // BANK_CUSTOMER_H