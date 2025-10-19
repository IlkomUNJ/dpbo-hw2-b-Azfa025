#ifndef CART_H
#define CART_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include "item.h"

struct CartItem {
    Item item;
    int quantity;
    seller* seller;
};

class Cart {
private:
    vector<CartItem> items;

public:
    void addItem(const Item& item, int quantity, seller* seller) {
        // Check if item already exists in cart
        for (auto& cartItem : items) {
            if (cartItem.item.getId() == item.getId() && cartItem.seller == seller) {
                cartItem.quantity += quantity;
                return;
            }
        }
        // If not found, add new item
        items.push_back({item, quantity, seller});
    }

    void removeItem(int itemId) {
        items.erase(
            remove_if(items.begin(), items.end(),
                     [itemId](const CartItem& item) { return item.item.getId() == itemId; }),
            items.end());
    }

    void updateQuantity(int itemId, int newQuantity) {
        for (auto& item : items) {
            if (item.item.getId() == itemId) {
                item.quantity = newQuantity;
                return;
            }
        }
    }

    double getTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.item.getPrice() * item.quantity;
        }
        return total;
    }

    void clear() {
        items.clear();
    }

    const vector<CartItem>& getItems() const {
        return items;
    }

    bool isEmpty() const {
        return items.empty();
    }

    void printCart() const {
        if (isEmpty()) {
            std::cout << "Cart is empty." << std::endl;
            return;
        }

        cout << "\nShopping Cart:" << endl;
        std::cout << std::setw(30) << std::left << "Item"
                  << std::setw(10) << std::right << "Quantity"
                  << std::setw(15) << std::right << "Price"
                  << std::setw(15) << std::right << "Total" << std::endl;
        std::cout << std::string(70, '-') << std::endl;

        for (const auto& item : items) {
            double itemTotal = item.item.getPrice() * item.quantity;
            std::cout << std::setw(30) << std::left << item.item.getName()
                      << std::setw(10) << std::right << item.quantity
                      << std::setw(15) << std::right << std::fixed << std::setprecision(2) << item.item.getPrice()
                      << std::setw(15) << std::right << itemTotal << std::endl;
        }
        std::cout << std::string(70, '-') << std::endl;
        std::cout << "Total: $" << std::fixed << std::setprecision(2) << getTotal() << std::endl;
    }
};

#endif