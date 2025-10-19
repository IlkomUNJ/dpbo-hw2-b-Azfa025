#ifndef SELLER_MENU_H
#define SELLER_MENU_H

#include <iostream>
#include <iomanip>
#include <string>
#include "seller.h"
#include "store.h"
#include "invoice.h"

using namespace std;

// Simple seller menu. Expects a pointer to a logged-in seller.
void handleSellerMenu(seller* s) {
    if (!s) return;
    bool running = true;
    while (running) {
        cout << "\nSeller Menu for " << s->getSellerName() << ":\n";
        cout << "1. Check Inventory" << endl;
        cout << "2. Add Item to Inventory" << endl;
        cout << "3. Remove Item from Inventory" << endl;
        cout << "4. View Orders (Paid)" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter choice: ";
        int choice; cin >> choice;
        switch (choice) {
            case 1: {
                cout << "\nInventory:\n";
                const auto &inv = s->getInventory();
                if (inv.empty()) {
                    cout << "No items in inventory." << endl;
                } else {
                    cout << left << setw(6) << "ID" << setw(30) << "Name" << setw(10) << "Qty" << setw(10) << "Price" << endl;
                    cout << string(60, '-') << endl;
                    for (const auto &it : inv) {
                        cout << setw(6) << it.getId() << setw(30) << it.getName() << setw(10) << it.getQuantity() << fixed << setprecision(2) << setw(10) << it.getPrice() << endl;
                    }
                }
                break;
            }
            case 2: {
                cout << "\nAdd New Item to Inventory" << endl;
                int id; string name; int qty; double price;
                cin.ignore();
                cout << "Item ID: "; cin >> id; cin.ignore();
                cout << "Item Name: "; getline(cin, name);
                cout << "Quantity: "; cin >> qty;
                cout << "Price: "; cin >> price;
                s->addNewItem(id, name, qty, price);
                cout << "Item added." << endl;
                break;
            }
            case 3: {
                cout << "\nRemove Item from Inventory" << endl;
                cout << "Enter Item ID to remove: "; int id; cin >> id;
                bool ok = s->removeItem(id);
                if (ok) cout << "Item removed." << endl; else cout << "Item not found." << endl;
                break;
            }
            case 4: {
                cout << "\nPaid Orders for this seller:\n";
                bool found = false;
                for (const auto &inv : g_invoices) {
                    if (inv.sellerId == s->getSellerId() && inv.status == InvoiceStatus::PAID) {
                        found = true;
                        cout << "Invoice ID: " << inv.id << " | Buyer ID: " << inv.buyerId << " | Total: $" << fixed << setprecision(2) << inv.totalAmount << "\n";
                        cout << "Items:\n";
                        for (const auto &it : inv.items) {
                            cout << " - " << it.itemName << " x" << it.quantity << " @ $" << it.unitPrice << " each (subtotal $" << it.totalPrice() << ")\n";
                        }
                        cout << "Order Status: PAID\n";
                        cout << string(60, '-') << endl;
                    }
                }
                if (!found) cout << "No paid orders for this seller." << endl;
                break;
            }
            case 5: {
                running = false;
                break;
            }
            default:
                cout << "Invalid choice." << endl;
        }
    }
}

#endif // SELLER_MENU_H
