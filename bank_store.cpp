#include "bank_store.h"

std::vector<std::unique_ptr<BankCustomer>> g_bankAccounts;

BankCustomer* createGlobalBankCustomer(int id, const std::string &name, double balance) {
    auto bc = std::make_unique<BankCustomer>(id, name, balance);
    BankCustomer* ptr = bc.get();
    g_bankAccounts.push_back(std::move(bc));
    return ptr;
}

BankCustomer* findBankByName(const std::string &name) {
    for (const auto &p : g_bankAccounts) {
        if (p && p->getName() == name) return p.get();
    }
    return nullptr;
}
