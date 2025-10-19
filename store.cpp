#include "store.h"

std::vector<std::unique_ptr<seller>> g_sellers;
std::vector<Invoice> g_invoices;

seller* findSellerById(int id) {
    for (auto &s : g_sellers) {
        if (s && s->getSellerId() == id) return s.get();
    }
    return nullptr;
}
