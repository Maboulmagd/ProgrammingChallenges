#ifndef ORDERBOOK_ENUMS_H
#define ORDERBOOK_ENUMS_H

#include "utilities.h"

#include <cstdint>
#include <string>
#include <cassert>
#include <cstring>

// TODO Could reorder fields to minimize space usage, especially the Order struct if it is being sent over a network

// Forward declaration to get around circular dependency
struct Limit;

struct Order final {
    uint32_t id;
    char symbol[8];
    char side;
    uint16_t qty;
    uint64_t px;//Need to convert 7.5 format -> (7.5 format means up to 7 digits before the decimal and exactly 5 digits after the decimal)

    // Doubly linked list of orders in FIFO order at that limit price
    Order* next_order = nullptr;
    Order* prev_order = nullptr;

    Order() : id(0), symbol("garbage"), side('B'), qty(0), px(0.0) {}
    Order(const Order& order) : id(order.id), side(order.side), qty(order.qty), px(order.px) {
        strncpy(this->symbol, order.symbol, sizeof(this->symbol));
        this->symbol[sizeof(this->symbol) - 1] = '\0'; // Ensure null-terminated string
    }
    // For testing
    explicit Order(const int id, char *symbol, const char side, const int qty, const double px) noexcept :
            id(id), side(side), qty(qty), px(doubleToFixedPoint(px)) {
        strncpy(this->symbol, symbol, sizeof(this->symbol));
        this->symbol[sizeof(this->symbol) - 1] = '\0'; // Ensure null-terminated string
    }
};

// This is a single limit price for a particular symbol at either a buy or sell side
struct Limit final {
    ~Limit() {
        // Delete all orders belonging to this limit
        while (head != nullptr) {
            Order* head_next = head->next_order;
            if (head_next != nullptr) head_next->prev_order = nullptr;

            delete head;

            head = head_next;
        }

        tail = nullptr;
    }

    uint64_t limit_px;
    uint16_t qty = 0;// Total number of shares at this limit price and side

    // At each limit price, we have a doubly linked list of orders
    Order* head = nullptr;
    Order* tail = nullptr;
};

struct Fill final {
    uint32_t oid;
    char symbol[8];
    char side;
    uint16_t fill_qty;
    uint64_t fill_px;
};

#endif //ORDERBOOK_ENUMS_H
