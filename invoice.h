#ifndef INVOICE_H
#define INVOICE_H

#include <vector>
#include <string>
#include "cart.h"

enum class InvoiceStatus { PENDING, PAID, CANCELLED, COMPLETE };

struct InvoiceItem {
    int itemId;
    std::string itemName;
    int quantity;
    double unitPrice;
    std::string date; // ISO date string (YYYY-MM-DD)
    double totalPrice() const { return unitPrice * quantity; }
};

struct Invoice {
    int id;
    int buyerId;
    int sellerId;
    std::vector<InvoiceItem> items;
    double totalAmount;
    InvoiceStatus status;
    std::string date; // ISO date string (YYYY-MM-DD)
    std::string paymentDate; // when payment was made
    std::string completionDate; // when order was completed
};

#endif
