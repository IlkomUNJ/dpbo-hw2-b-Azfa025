#ifndef STORE_H
#define STORE_H

#include <vector>
#include <memory>
#include "seller.h"
#include "invoice.h"

// Global storage for sellers and invoices. Main will instantiate these.
extern std::vector<std::unique_ptr<seller>> g_sellers;
extern std::vector<Invoice> g_invoices;

// Helpers
seller* findSellerById(int id);

#endif
