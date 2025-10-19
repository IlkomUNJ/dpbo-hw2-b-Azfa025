#ifndef BUYER_MENU_H
#define BUYER_MENU_H

#include "buyer.h"
#include "user_role.h"
#include "cart.h"
#include "store.h"
#include "bank_store.h"
#include <iostream>
#include <iomanip>
#include <map>

using namespace std;

// per-session cart for buyer interactions
static Cart g_cart;

void handleBuyerMenu(Buyer* buyer, UserRole& currentRole) {
    if (!buyer) {
        currentRole = UserRole::NONE;
        return;
    }

    bool staying = true;
    while (staying) {
        cout << "\nBuyer Menu:" << endl;
        cout << "1. Check Account Status" << endl;
        cout << "2. Upgrade to Seller" << endl;
        cout << "3. Banking Functions" << endl;
        cout << "4. Browse Store" << endl;
        cout << "5. View Cart" << endl;
        cout << "6. View Orders" << endl;
        cout << "7. Logout" << endl;
        cout << "8. Delete Account" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch(choice) {
            case 1: { // Check Account Status
                cout << "\nAccount Status:" << endl;
                cout << "Name: " << buyer->getName() << endl;
                cout << "Email: " << buyer->getEmail() << endl;
                if (buyer->getAccount()) {
                    cout << "\nLinked Bank Account:" << endl;
                    buyer->getAccount()->printInfo();
                } else {
                    cout << "\nNo bank account linked." << endl;
                }
                break;
            }
            case 4: { // Browse Store
                cout << "\nAvailable Stores:" << endl;
                cout << "No stores available yet." << endl;
                // TODO: Implement store browsing
                break;
            }

            case 5: { // View Cart
                g_cart.printCart();
                if (!g_cart.isEmpty()) {
                    cout << "\nCart Options:" << endl;
                    cout << "1. Update Quantity" << endl;
                    cout << "2. Remove Item" << endl;
                    cout << "3. Clear Cart" << endl;
                    cout << "4. Checkout" << endl;
                    cout << "5. Back" << endl;
                    cout << "Enter your choice: ";

                    int cartChoice;
                    cin >> cartChoice;

                    switch(cartChoice) {
                        case 1: { // Update Quantity
                            int itemId, newQuantity;
                            cout << "Enter Item ID: ";
                            cin >> itemId;
                            cout << "Enter new quantity: ";
                            cin >> newQuantity;
                            g_cart.updateQuantity(itemId, newQuantity);
                            break;
                        }
                        case 2: { // Remove Item
                            int itemId;
                            cout << "Enter Item ID to remove: ";
                            cin >> itemId;
                            g_cart.removeItem(itemId);
                            break;
                        }
                        case 3: // Clear Cart
                            g_cart.clear();
                            cout << "Cart cleared." << endl;
                            break;
                        case 4: { // Checkout
                            // Group cart items by seller and create invoices
                            if (g_cart.isEmpty()) { cout << "Cart is empty." << endl; break; }
                            // For simplicity, assume all items in cart go to one seller
                            std::map<seller*, std::vector<CartItem>> sellerItems;
                            for (const auto& ci : g_cart.getItems()) {
                                sellerItems[ci.seller].push_back(ci);
                            }
                            int nextInvoiceId = g_invoices.empty() ? 1 : g_invoices.back().id + 1;
                            time_t now = time(nullptr);
                            char buf[20];
                            strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
                            std::string today(buf);
                            for (auto& pair : sellerItems) {
                                seller* s = pair.first;
                                std::vector<InvoiceItem> items;
                                double total = 0;
                                for (const auto& ci : pair.second) {
                                    items.push_back({ci.item.getId(), ci.item.getName(), ci.quantity, ci.item.getPrice(), today});
                                    total += ci.item.getPrice() * ci.quantity;
                                }
                                Invoice inv{nextInvoiceId++, buyer->getId(), s->getSellerId(), items, total, InvoiceStatus::PENDING, today};
                                g_invoices.push_back(inv);
                            }
                            g_cart.clear();
                            cout << "Checkout complete! Invoices created." << endl;
                            break;
                        }
                    }
                }
                break;
            }

            case 6: { // View Orders / Payments
                // List invoices for this buyer
                bool foundAny = false;
                for (const auto &inv : g_invoices) {
                    if (inv.buyerId == buyer->getId()) {
                        foundAny = true;
                        std::cout << "Invoice ID: " << inv.id << " | Seller ID: " << inv.sellerId << " | Total: $" << std::fixed << std::setprecision(2) << inv.totalAmount << " | Status: " << (inv.status==InvoiceStatus::PAID?"PAID":"PENDING") << std::endl;
                    }
                }
                if (!foundAny) { std::cout << "No invoices found." << std::endl; break; }

                cout << "Enter invoice ID to view/pay (0 to cancel): ";
                int invId; cin >> invId;
                if (invId == 0) break;
                Invoice* invPtr = nullptr;
                for (auto &inv : g_invoices) if (inv.id == invId && inv.buyerId == buyer->getId()) { invPtr = &inv; break; }
                if (!invPtr) { cout << "Invoice not found." << endl; break; }
                auto &inv = *invPtr;
                cout << "Invoice " << inv.id << " details:" << endl;
                for (const auto &ii : inv.items) cout << " - " << ii.itemName << " x" << ii.quantity << " @ $" << ii.unitPrice << " => $" << ii.totalPrice() << endl;
                cout << "Total: $" << inv.totalAmount << " | Status: ";
                if (inv.status == InvoiceStatus::PAID) cout << "PAID";
                else if (inv.status == InvoiceStatus::COMPLETE) cout << "COMPLETE";
                else if (inv.status == InvoiceStatus::CANCELLED) cout << "CANCELLED";
                else cout << "PENDING";
                cout << " | Date: " << inv.date << endl;
                if (inv.status == InvoiceStatus::PAID) {
                    cout << "Mark this order as COMPLETE? (y/n): "; char compc; cin >> compc;
                    if (compc == 'y' || compc == 'Y') {
                        time_t nowc = time(nullptr);
                        char cb[20];
                        strftime(cb, sizeof(cb), "%Y-%m-%d", localtime(&nowc));
                        inv.status = InvoiceStatus::COMPLETE;
                        inv.completionDate = std::string(cb);
                        cout << "Order marked as COMPLETE (" << inv.completionDate << ")." << endl;
                    }
                    break;
                }
                if (inv.status == InvoiceStatus::COMPLETE) { cout << "Order already completed." << endl; break; }
                if (inv.status == InvoiceStatus::CANCELLED) { cout << "Order was cancelled." << endl; break; }
                cout << "Pay this invoice now? (y/n): "; char payc; cin >> payc;
                if (payc != 'y' && payc != 'Y') break;
                if (!buyer->getAccount()) { cout << "You need a bank account to pay." << endl; break; }
                std::cout << "Your account balance: $" << buyer->getAccount()->getBalance() << std::endl;
                if (buyer->getAccount()->getBalance() < inv.totalAmount) { cout << "Insufficient funds." << endl; break; }
                // Deduct buyer
                buyer->getAccount()->withdrawBalance(inv.totalAmount);
                // Credit seller and reduce stock
                seller* s = findSellerById(inv.sellerId);
                if (s) {
                    s->addToBalance(inv.totalAmount);
                    for (const auto &ii : inv.items) {
                        s->reduceItemQuantity(ii.itemId, ii.quantity);
                    }
                }
                inv.status = InvoiceStatus::PAID;
                time_t payt = time(nullptr);
                char pbuf[20];
                strftime(pbuf, sizeof(pbuf), "%Y-%m-%d", localtime(&payt));
                inv.paymentDate = std::string(pbuf);
                cout << "Payment successful. Invoice marked PAID (" << inv.paymentDate << ")." << endl;
                break;
            }

            case 3: { // Banking Functions
                if (!buyer->getAccount()) {
                    cout << "\nYou don't have a bank account linked. Create one now? (y/n): ";
                    char choice;
                    cin >> choice;
                    if (choice == 'y' || choice == 'Y') {
                        double initialDeposit;
                        cout << "\nCreating Bank Account..." << endl;
                        cout << "Enter Initial Deposit Amount: $";
                        cin >> initialDeposit;
                        if (initialDeposit <= 0) {
                            cout << "Error: Initial deposit must be positive!" << endl;
                            break;
                        }
                        static int bankAccountCounter = 0;
                        bankAccountCounter++;
                        BankCustomer* newAccount = createGlobalBankCustomer(bankAccountCounter, buyer->getName(), initialDeposit);
                        buyer->setAccount(newAccount);
                        cout << "\nBank account created successfully!" << endl;
                    } else {
                        break;
                    }
                }

                // Banking Menu
                bool bankingMenu = true;
                while (bankingMenu) {
                    cout << "\nBanking Menu:" << endl;
                    cout << "1. Check Balance" << endl;
                    cout << "2. Deposit" << endl;
                    cout << "3. Withdraw" << endl;
                    cout << "4. Transaction History" << endl;
                    cout << "5. Back to Main Menu" << endl;
                    cout << "Enter your choice: ";

                    int bankChoice;
                    cin >> bankChoice;

                    switch(bankChoice) {
                        case 1: // Check Balance
                            cout << "\nCurrent Balance:" << endl;
                            buyer->getAccount()->printInfo();
                            break;

                        case 2: { // Deposit
                            double amount;
                            cout << "Enter amount to deposit: $";
                            cin >> amount;
                            if (amount <= 0) {
                                cout << "Error: Amount must be positive!" << endl;
                                break;
                            }
                            buyer->getAccount()->addBalance(amount);
                            cout << "Deposit successful!" << endl;
                            buyer->getAccount()->printInfo();
                            break;
                        }

                        case 3: { // Withdraw
                            double amount;
                            cout << "Enter amount to withdraw: $";
                            cin >> amount;
                            if (amount <= 0) {
                                cout << "Error: Amount must be positive!" << endl;
                                break;
                            }
                            if (buyer->getAccount()->withdrawBalance(amount)) {
                                cout << "Withdrawal successful!" << endl;
                                buyer->getAccount()->printInfo();
                            }
                            break;
                        }

                        case 4: // Transaction History
                            buyer->getAccount()->printTransactionHistory();
                            break;

                        case 5:
                            bankingMenu = false;
                            break;

                        default:
                            cout << "Invalid choice!" << endl;
                            break;
                    }
                }
                break;
            }

            case 2: { // Upgrade to Seller
                cout << "\nUpgrade to Seller Account" << endl;
                if (!buyer->getAccount()) {
                    cout << "Error: You need to have a bank account linked first!" << endl;
                    break;
                }

                // Collect seller information
                string sellerName, storeName, storeAddress, storePhone, storeEmail;
                double initialDeposit;
                static int sellerIdCounter = 0;

                cout << "\nEnter Seller Details:" << endl;
                cout << "Enter Seller Name: ";
                cin.ignore();
                getline(cin, sellerName);
                cout << "Enter Store Name: ";
                getline(cin, storeName);
                cout << "Enter Store Address: ";
                getline(cin, storeAddress);
                cout << "Enter Store Phone: ";
                getline(cin, storePhone);
                cout << "Enter Store Email: ";
                getline(cin, storeEmail);
                cout << "Enter Initial Store Balance: $";
                cin >> initialDeposit;

                if (initialDeposit <= 0) {
                    cout << "Error: Initial deposit must be positive!" << endl;
                    break;
                }

                // Withdraw initial deposit from buyer's bank account
                if (!buyer->getAccount()->withdrawBalance(initialDeposit)) {
                    cout << "Error: Insufficient funds in your bank account!" << endl;
                    break;
                }

                // Create seller account
                sellerIdCounter++;
                Buyer buyerCopy(*buyer); // Create a copy of the buyer
                seller* newSeller = new seller(
                    buyerCopy,
                    sellerIdCounter,
                    sellerName,
                    storeName,
                    storeAddress,
                    storePhone,
                    storeEmail,
                    initialDeposit
                );

                cout << "\nSeller account created successfully!" << endl;
                newSeller->printStoreInfo();
                cout << "\nYou can now switch between Buyer and Seller roles!" << endl;
                break;
            }
            case 7: { // Logout
                cout << "Are you sure you want to logout? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    staying = false;
                    currentRole = UserRole::NONE;
                }
                break;
            }
            default:
                cout << "Feature coming soon!" << endl;
                break;
        }
    }
}

#endif