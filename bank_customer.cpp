#include "bank_customer.h"
#include <iostream>
#include <cstring>


string BankCustomer::getName() const {
    return this->name;
}

int BankCustomer::getId() const {
    return this->id ;
}

double BankCustomer::getBalance() const {
    return this->balance;
}

void BankCustomer::setBalance(double amount) {
    this->balance = amount;
}

void BankCustomer::addBalance(double amount) {
    this->balance += amount;
    // record transaction
    time_t now = time(nullptr);
    transactionHistory.push_back({"DEPOSIT", amount, this->balance, now});
}

bool BankCustomer::withdrawBalance(double amount){
    if (amount > this->balance) {
        std::cout << "Rejected: Insufficient funds!" << std::endl;
        return false;
    }
    this->balance -= amount;
    time_t now = time(nullptr);
    transactionHistory.push_back({"WITHDRAW", amount, this->balance, now});
    return true;
}

void BankCustomer::printInfo() const {
    std::cout << "Customer Name: " << this->name << std::endl;
    std::cout << "Customer ID: " << this->id << std::endl;
    std::cout << "Balance: $" << this->balance << std::endl;
}

void BankCustomer::printTransactionHistory() const {
    using std::cout; using std::endl;
    cout << "\nTransaction History for " << name << ":" << endl;
    cout << std::left << std::setw(25) << "Timestamp" << std::setw(10) << "Type" << std::setw(15) << "Amount" << std::setw(15) << "Balance" << endl;
    cout << string(65, '-') << endl;
    for (const auto &t : transactionHistory) {
        char buf[20];
        std::tm *tm = std::localtime(&t.timestamp);
        if (tm) strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", tm); else strcpy(buf, "-");
        cout << std::left << std::setw(25) << buf << std::setw(10) << t.type << std::setw(15) << std::fixed << std::setprecision(2) << t.amount << std::setw(15) << t.balanceAfter << endl;
    }
}