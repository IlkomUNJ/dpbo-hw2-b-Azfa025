#ifndef SELLER_H
#define SELLER_H

#include "buyer.h"
#include "item.h"
#include <iostream>
#include <string>
#include <vector>

class seller {
private:
    Buyer baseBuyer;  // Composition dengan buyer
    int sellerId;
    std::string sellerName;
    std::string storeName;
    std::string storeAddress;
    std::string storePhone;
    std::string storeEmail;
    double storeBalance;
    std::vector<Item> items;

public:
    seller() = default;

    seller(const Buyer &buyer, int sellerId, const std::string &sellerName,
           const std::string &storeName, const std::string &storeAddress,
           const std::string &storePhone, const std::string &storeEmail,
           double initialBalance)
        : baseBuyer(buyer), sellerId(sellerId), sellerName(sellerName),
          storeName(storeName), storeAddress(storeAddress), 
          storePhone(storePhone), storeEmail(storeEmail),
          storeBalance(initialBalance) {}

    virtual ~seller() = default;

    // Getters
    int getSellerId() const { return sellerId; }
    const std::string& getSellerName() const { return sellerName; }
    const Buyer& getBuyer() const { return baseBuyer; }
    const std::string& getStoreName() const { return storeName; }
    const std::string& getStoreAddress() const { return storeAddress; }
    const std::string& getStorePhone() const { return storePhone; }
    const std::string& getStoreEmail() const { return storeEmail; }
    double getStoreBalance() const { return storeBalance; }
    const std::vector<Item>& getInventory() const { return items; }

    void addNewItem(int newId, const std::string& newName, int newQuantity, double newPrice) {
        Item newItem(newId, newName, newQuantity, newPrice);
        items.push_back(newItem);
    }

    // Reduce item quantity by id. Returns true if successful.
    bool reduceItemQuantity(int itemId, int qty) {
        for (auto &it : items) {
            if (it.getId() == itemId) {
                if (it.getQuantity() < qty) return false;
                it.setQuantity(it.getQuantity() - qty);
                return true;
            }
        }
        return false;
    }

    // Remove item entirely from inventory by id. Returns true if removed.
    bool removeItem(int itemId) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->getId() == itemId) {
                items.erase(it);
                return true;
            }
        }
        return false;
    }

    // Credit the store balance
    void addToBalance(double amount) { storeBalance += amount; }

    void updateItem(int itemId, const std::string& newName, int newQuantity, double newPrice) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.alterItemById(itemId, newName, newQuantity, newPrice);
            }
        }
    }

    void makeItemVisibleToCustomer(int itemId) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.setDisplay(true);
                break;
            }
        }
    }

    void printStoreInfo() const {
        std::cout << "\nStore Information:" << std::endl;
        std::cout << "Seller Name: " << sellerName << std::endl;
        std::cout << "Store Name: " << storeName << std::endl;
        std::cout << "Store Address: " << storeAddress << std::endl;
        std::cout << "Store Phone: " << storePhone << std::endl;
        std::cout << "Store Email: " << storeEmail << std::endl;
        std::cout << "Store Balance: $" << storeBalance << std::endl;
    }
};

#endif