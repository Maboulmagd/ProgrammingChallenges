#include "orderbook.h"
#include "utilities.h"

#include <cassert>
#include <iomanip>

OrderBook::~OrderBook() {
    for (auto& [order_id, _] : orders_) {
        takeOrderOffBook(order_id);
    }
}

std::list<std::string> OrderBook::insertOrderInBook(const Order &new_order) {
    if (orders_.find(new_order.id) != orders_.cend()) {// Duplicate order id
        std::ostringstream error_msg_stream;
        error_msg_stream << "E " << new_order.id << " Duplicate order id";
        return {error_msg_stream.str()};
    }

    std::list<std::string> trades;

    switch (new_order.side) {
        case 'B': {
            Order *order = new Order(new_order);

            // No sell orders OR current lowest sell is too high, add buy order to buy side of the book
            if (sell_side.empty() || sell_side.begin()->second.limit_px > order->px) {
                addOrderToBook(order);
            }
            // Best sell is less than or equal to buy order's price, match buy order
            else {
                while (!sell_side.empty() && sell_side.begin()->second.limit_px <= order->px && order->qty > 0) {
                    Limit &lowest_sell = sell_side.begin()->second;

                    matchBuyOrder(lowest_sell, order, trades);//Match with all possible sell orders at lowest_sell

                    // Cleanup list...can maybe do better? Leave it as is for now
                    deleteOrdersWithZeroQty(lowest_sell);

                    // Time to delete this limit price
                    if (lowest_sell.head == nullptr) {
                        // Sanity checks
                        assert(lowest_sell.head == nullptr);
                        assert(lowest_sell.tail == nullptr);

                        sell_side.erase(lowest_sell.limit_px);
                    }
                }

                // If after matching, quantity is still > 0, add buy order to buy side of the book
                if (order->qty > 0) {
                    addOrderToBook(order);
                }
            }

            break;
        }

        case 'S': {
            Order *order = new Order(new_order);

            // No buy orders OR current highest buy is too low, add sell order to sell side of the book
            if (buy_side.empty() || buy_side.begin()->second.limit_px < order->px) {
                addOrderToBook(order);
            }

            // Best buy is greater than or equal to sell order's price, match sell order
            else {
                while (!buy_side.empty() && buy_side.begin()->second.limit_px >= order->px && order->qty > 0) {
                    Limit &highest_buy = buy_side.begin()->second;

                    matchSellOrder(highest_buy, order, trades);//Match with all possible buy orders at highest_buy

                    // Cleanup list...can maybe do better? Leave it as is for now
                    deleteOrdersWithZeroQty(highest_buy);

                    // Time to delete this limit price
                    if (highest_buy.head == nullptr) {
                        // Sanity checks
                        assert(highest_buy.head == nullptr);
                        assert(highest_buy.tail == nullptr);

                        buy_side.erase(highest_buy.limit_px);
                    }
                }

                // If after matching, quantity is still > 0, add sell order to sell side of the book
                if (order->qty > 0) {
                    addOrderToBook(order);
                }
            }

            break;
        }

        default:
            assert(false);// Should never get here
    }

    return trades;
}

void OrderBook::addOrderToBook(Order *order) {
    assert(order != nullptr);
    assert(orders_.find(order->id) == orders_.cend());

    const char side_of_book = order->side;

    switch (side_of_book) {
        case 'B': {
            // Limit price already exists, simply add order to tail of list of orders belonging to limit
            if (buy_side.find(order->px) != buy_side.cend()) {
                Limit &limit = buy_side[order->px];

                addOrderToTail(limit, order);
            }
            // Limit price doesn't exist
            else {
                assert(buy_side.find(order->px) == buy_side.cend());

                Limit new_limit;
                new_limit.limit_px = order->px;

                addOrderToTail(new_limit, order);

                buy_side[new_limit.limit_px] = new_limit;
                // Issue is Limit goes out of scope here and so calls destructor, I want to make sure we are making a copy of new_limit
            }

            break;
        }

        case 'S': {
            // Limit price already exists, simply add order to tail of list of orders belonging to limit
            if (sell_side.find(order->px) != sell_side.cend()) {
                Limit& limit = sell_side[order->px];

                addOrderToTail(limit, order);
            }
            // Limit price doesn't exist
            else {
                assert(sell_side.find(order->px) == sell_side.cend());

                Limit new_limit;
                new_limit.limit_px = order->px;

                addOrderToTail(new_limit, order);

                sell_side[new_limit.limit_px] = new_limit;
            }

            break;
        }

        default:
            assert(false);// Should never get in here
    }

    orders_[order->id] = order;
}

void OrderBook::addOrderToTail(Limit &limit, Order *order_to_add) {
    assert(order_to_add != nullptr);

    assert(order_to_add->next_order == nullptr);
    assert(order_to_add->prev_order == nullptr);

    // Empty list
    if (limit.head == nullptr) {
        assert(limit.tail == nullptr);
        limit.head = order_to_add;
        limit.tail = order_to_add;
    }
        // Singleton list
    else if (limit.head->next_order == nullptr) {
        assert(limit.head == limit.tail);

        limit.head->next_order = order_to_add;
        order_to_add->prev_order = limit.head;

        limit.tail = order_to_add;
    }
        // List with at least 2 nodes
    else {
        assert(limit.head != nullptr);
        assert(limit.tail != nullptr);
        assert(limit.head != limit.tail);

        limit.tail->next_order = order_to_add;
        order_to_add->prev_order = limit.tail;

        limit.tail = order_to_add;
    }

    limit.qty += order_to_add->qty;
}

void OrderBook::deleteOrderFromList(Limit *&parent_limit, Order *&order_to_remove) {
    // Sanity checks
    assert(parent_limit != nullptr);
    assert(order_to_remove != nullptr);
    assert(order_to_remove->qty == 0);

    assert(parent_limit->head != nullptr);
    assert(parent_limit->tail != nullptr);

    if (parent_limit->head == order_to_remove) {
        parent_limit->head = order_to_remove->next_order;
    }
    if (parent_limit->tail == order_to_remove) {
        parent_limit->tail = order_to_remove->prev_order;
    }

    if (order_to_remove->next_order != nullptr) {
        order_to_remove->next_order->prev_order = order_to_remove->prev_order;
    }

    if (order_to_remove->prev_order != nullptr) {
        order_to_remove->prev_order->next_order = order_to_remove->next_order;
    }

    parent_limit->qty -= order_to_remove->qty;

    orders_.erase(order_to_remove->id);
    delete order_to_remove;
    order_to_remove = nullptr;
}

// This is called when we receive a cancel order request
std::list<std::string> OrderBook::takeOrderOffBook(const uint32_t order_id) {
    if (orders_.find(order_id) == orders_.cend()) {
        std::ostringstream error_msg_stream;
        error_msg_stream << "E Couldn't find order id " << order_id;
        return {error_msg_stream.str()};
    }
    assert(orders_.find(order_id) != orders_.cend());

    Order* order_to_remove = orders_[order_id];
    assert(order_to_remove != nullptr);

    std::list<std::string> cancelled;
    cancelled.push_back("X " + std::to_string(order_id));

    const char side_of_book = order_to_remove->side;

    Limit *limit = nullptr;
    if (side_of_book == 'S') {
        limit = &sell_side[order_to_remove->px];
    }
    else {
        assert(side_of_book == 'B');
        limit = &buy_side[order_to_remove->px];
    }

    deleteOrderFromList(limit, order_to_remove);
    assert(order_to_remove == nullptr);

    // If limit has no more orders, delete limit
    if (limit->head == nullptr) {
        if (side_of_book == 'S') {
            sell_side.erase(limit->limit_px);
        }
        else {
            assert(side_of_book == 'B');
            buy_side.erase(limit->limit_px);
        }
    }

    return cancelled;
}

void OrderBook::matchBuyOrder(Limit &sell, Order *order, std::list<std::string>& trades) {
    // Match buy order with sell orders associated with that limit
    Order* sell_order = sell.head;

    while (sell_order != nullptr) {
        std::ostringstream formatted_price;
        formatted_price << std::fixed << std::setprecision(5) << fixedPointToDouble(std::min(order->px, sell_order->px));
        const std::string px = formatted_price.str();

        if (sell_order->qty == order->qty) {
            // Add to trade matches
            trades.push_back("F " + std::to_string(order->id) + ' ' + order->symbol + ' ' + std::to_string(order->qty) + ' ' +
                             px);

            trades.push_back("F " + std::to_string(sell_order->id) + ' ' + sell_order->symbol + ' ' + std::to_string(sell_order->qty) + ' ' +
                            px);

            sell.qty -= sell_order->qty;

            sell_order->qty = 0;
            order->qty = 0;
            break;
        }

        else if (sell_order->qty > order->qty) {
            // Add to trade matches
            trades.push_back("F " + std::to_string(order->id) + ' ' + order->symbol + ' ' + std::to_string(order->qty) + ' ' +
                             px);

            trades.push_back("F " + std::to_string(sell_order->id) + ' ' + sell_order->symbol + ' ' + std::to_string(order->qty) + ' ' +
                             px);

            sell.qty -= order->qty;

            sell_order->qty -= order->qty;
            order->qty = 0;
            break;
        }

        else if (sell_order->qty < order->qty) {
            // Add to trade matches
            trades.push_back("F " + std::to_string(order->id) + ' ' + order->symbol + ' ' + std::to_string(sell_order->qty) + ' ' +
                             px);

            trades.push_back("F " + std::to_string(sell_order->id) + ' ' + sell_order->symbol + ' ' + std::to_string(sell_order->qty) + ' ' +
                             px);

            sell.qty -= sell_order->qty;

            order->qty -= sell_order->qty;
            sell_order->qty = 0;
        }

        sell_order = sell_order->next_order;
    }
}

void OrderBook::matchSellOrder(Limit &buy, Order *order, std::list<std::string>& trades) {
    // Match sell order with buy orders associated with that limit
    Order* buy_order = buy.head;

    while (buy_order != nullptr) {
        std::ostringstream formatted_price;
        formatted_price << std::fixed << std::setprecision(5) << fixedPointToDouble(std::min(order->px, buy_order->px));
        const std::string px = formatted_price.str();

        if (buy_order->qty == order->qty) {
            // Add to trade matches
            trades.push_back("F " + std::to_string(order->id) + ' ' + order->symbol + ' ' + std::to_string(order->qty) + ' ' +
                             px);

            trades.push_back("F " + std::to_string(buy_order->id) + ' ' + buy_order->symbol + ' ' + std::to_string(buy_order->qty) + ' ' +
                             px);

            buy.qty -= buy_order->qty;

            buy_order->qty = 0;
            order->qty = 0;
            break;
        }

        else if (buy_order->qty > order->qty) {
            // Add to trade matches
            trades.push_back("F " + std::to_string(order->id) + ' ' + order->symbol + ' ' + std::to_string(order->qty) + ' ' +
                             px);

            trades.push_back("F " + std::to_string(buy_order->id) + ' ' + buy_order->symbol + ' ' + std::to_string(order->qty) + ' ' +
                             px);

            buy.qty -= order->qty;

            buy_order->qty -= order->qty;
            order->qty = 0;
            break;
        }

        else if (buy_order->qty < order->qty) {
            // Add to trade matches
            trades.push_back("F " + std::to_string(order->id) + ' ' + order->symbol + ' ' + std::to_string(buy_order->qty) + ' ' +
                             px);

            trades.push_back("F " + std::to_string(buy_order->id) + ' ' + buy_order->symbol + ' ' + std::to_string(buy_order->qty) + ' ' +
                             px);

            buy.qty -= buy_order->qty;

            order->qty -= buy_order->qty;
            buy_order->qty = 0;
        }

        buy_order = buy_order->next_order;
    }
}

void OrderBook::deleteOrdersWithZeroQty(Limit &limit) {
    // Sanity checks
    Order* current = limit.head;
    assert(current->prev_order == nullptr);

    while (current != nullptr) {
        Order* prev = current->prev_order;
        Order* next = current->next_order;

        if (current->qty == 0) {
            if (prev != nullptr) {
                prev->next_order = next;
            }
            else {
                limit.head = next;
            }

            if (next != nullptr) {
                next->prev_order = prev;
            }
            else {
                limit.tail = prev;
            }

            assert(orders_.find(current->id) != orders_.cend());
            orders_.erase(current->id);

            delete current;
            current = nullptr;
        }

        current = next;
    }
}

std::list<std::string> OrderBook::printOrderBook() {
    std::list<std::string> resting_orders;

    printSellSide(resting_orders);
    printBuySide(resting_orders);

    return resting_orders;
}

void OrderBook::printSellSide(std::list<std::string>& resting_orders) {
    for (const auto& [limit_px, limit] : sell_side) {
        const Order* curr = limit.head;
        while (curr != nullptr) {
            resting_orders.push_back("P " + std::to_string(curr->id) + ' ' + curr->symbol + ' ' + curr->side + ' ' +
                                     std::to_string(curr->qty) + ' ' + std::to_string(fixedPointToDouble(curr->px)));

            curr = curr->next_order;
        }
    }
}

void OrderBook::printBuySide(std::list<std::string>& resting_orders) {
    for (const auto& [limit_px, limit] : buy_side) {
        const Order *curr = limit.head;
        while (curr != nullptr) {
            resting_orders.push_back("P " + std::to_string(curr->id) + ' ' + curr->symbol + ' ' + curr->side + ' ' +
                                     std::to_string(curr->qty) + ' ' + std::to_string(fixedPointToDouble(curr->px)));

            curr = curr->next_order;
        }
    }
}