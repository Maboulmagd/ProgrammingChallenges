#ifndef ORDERBOOK_SRC_ORDER_BOOK_H
#define ORDERBOOK_SRC_ORDER_BOOK_H

#include "enums.h"

#include <list>
#include <unordered_map>
#include <map>
#include <memory>

class OrderBook final {
public:
    explicit OrderBook() = default;
    ~OrderBook();
    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;

    std::list<std::string> insertOrderInBook(const Order& new_order);
    std::list<std::string> takeOrderOffBook(const uint32_t order_id);
    std::list<std::string> printOrderBook();

    // Quick way to lookup if order already has same order_id
    std::unordered_map<uint32_t, Order*> orders_;

    struct BuySideComp {
        bool operator()(const uint64_t lhs, const uint64_t rhs) const {
            return lhs > rhs;
        }
    };

    struct SellSideComp {
        bool operator()(const uint64_t lhs, const uint64_t rhs) const {
            return lhs < rhs;
        }
    };

    std::map<uint64_t, std::unique_ptr<Limit>, BuySideComp> buy_side;
    std::map<uint64_t, std::unique_ptr<Limit>, SellSideComp> sell_side;

private:
    void addOrderToBook(Order *order);

    void matchBuyOrder(Limit *sell, Order *order, std::list<std::string>& trades);
    void matchSellOrder(Limit *buy, Order *order, std::list<std::string>& trades);

    void addOrderToTail(Limit *limit, Order *order_to_add);
    void deleteOrdersWithZeroQty(Limit *limit);
    void deleteOrderFromList(Limit *&parent_limit, Order *&order_to_remove);

    void printSellSide(std::list<std::string>& resting_orders);
    void printBuySide(std::list<std::string>& resting_orders);
};

#endif //ORDERBOOK_SRC_ORDER_BOOK_H
