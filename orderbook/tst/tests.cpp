#include "orderbook.h"

#include <gmock/gmock.h>
//#include <gtest/gtest.h>

#include <memory>

using namespace ::testing;

class OrderBookTests : public ::testing::Test {
protected:
    explicit OrderBookTests() {
        book_ = std::make_unique<OrderBook>();
    }

    ~OrderBookTests() override = default;

    std::unique_ptr<OrderBook> book_;
};

// Increasing buys followed with a sell that consumes all
TEST_F(OrderBookTests, BasicBuysAndSellMatchesAll) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1'000, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1'000));
    const uint64_t px = doubleToFixedPoint(100.000000);

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(book_->buy_side.begin()->second.limit_px, px);
    ASSERT_EQ(book_->buy_side.begin()->second.qty, 10);
    ASSERT_EQ(book_->buy_side.begin()->second.head->id, 1'000);
    ASSERT_EQ(book_->buy_side.begin()->second.tail->id, 1'000);

    book_->insertOrderInBook(Order{1'001, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(1'001));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(book_->buy_side.begin()->second.limit_px, px);
    ASSERT_EQ(book_->buy_side.begin()->second.qty, 10);
    ASSERT_EQ(book_->buy_side.begin()->second.head->id, 1'000);
    ASSERT_EQ(book_->buy_side.begin()->second.tail->id, 1'000);

    auto next = std::next(book_->buy_side.begin());
    ASSERT_EQ(next->second.limit_px, px_2);
    ASSERT_EQ(next->second.qty, 10);
    ASSERT_EQ(next->second.head->id, 1'001);
    ASSERT_EQ(next->second.tail->id, 1'001);

    book_->insertOrderInBook(Order{1'002, symbol, 'B', 10, 120.000000});

//    ASSERT_TRUE(book_->orders_.contains(1'002));
//    const uint64_t px_3 = doubleToFixedPoint(120.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_3));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'003, symbol, 'B', 20, 120.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'003));
//    ASSERT_TRUE(book_->limits_.contains(px_3));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'004, symbol, 'B', 30, 130.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'004));
//    const uint64_t px_4 = doubleToFixedPoint(130.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_4));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->limit_px, px_4);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->head->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->tail->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'005, symbol, 'S', 80, 90.000000});
//
//    ASSERT_EQ(book_->buy_side, nullptr);
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_TRUE(book_->orders_.empty());
//    ASSERT_TRUE(book_->limits_.empty());
}
//
//// Increasing buys followed with a sell that consumes all, BUT does have leftover qty
//TEST_F(OrderBookTests, BasicBuysAndSellMatchesAllWithLeftover) {
//    char symbol[8] = "IBM";
//    book_->insertOrderInBook(Order{1'000, symbol, 'B', 10, 100.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'000));
//    const uint64_t px = doubleToFixedPoint(100.000000);
//    ASSERT_TRUE(book_->limits_.contains(px));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'001, symbol, 'B', 10, 110.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'001));
//    const uint64_t px_2 = doubleToFixedPoint(110.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_2));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'002, symbol, 'B', 10, 120.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'002));
//    const uint64_t px_3 = doubleToFixedPoint(120.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_3));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'003, symbol, 'B', 20, 120.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'003));
//    ASSERT_TRUE(book_->limits_.contains(px_3));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'004, symbol, 'B', 30, 130.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'004));
//    const uint64_t px_4 = doubleToFixedPoint(130.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_4));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->limit_px, px_4);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->head->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->tail->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'005, symbol, 'S', 100, 90.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'005));
//    const uint64_t px_5 = doubleToFixedPoint(90.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_5));
//
//    ASSERT_EQ(book_->buy_side, nullptr);
//    ASSERT_EQ(book_->sell_side->limit_px, px_5);
//    ASSERT_EQ(book_->sell_side->qty, 20);
//    ASSERT_EQ(book_->sell_side->head->id, 1'005);
//    ASSERT_EQ(book_->sell_side->tail->id, 1'005);
//    ASSERT_EQ(book_->sell_side->parent, nullptr);
//    ASSERT_EQ(book_->sell_side->left_child, nullptr);
//    ASSERT_EQ(book_->sell_side->right_child, nullptr);
//}
//
//// Increasing buys followed with random RESTING sells, then a sell that consumes all buys, but has leftover qty
//TEST_F(OrderBookTests, BasicBuysAndRestingSellsThenSellMatchesAllWithLeftover) {
//    char symbol[8] = "IBM";
//    book_->insertOrderInBook(Order{1'000, symbol, 'B', 10, 100.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'000));
//    const uint64_t px = doubleToFixedPoint(100.000000);
//    ASSERT_TRUE(book_->limits_.contains(px));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'001, symbol, 'B', 10, 110.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'001));
//    const uint64_t px_2 = doubleToFixedPoint(110.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_2));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'002, symbol, 'B', 10, 120.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'002));
//    const uint64_t px_3 = doubleToFixedPoint(120.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_3));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'003, symbol, 'B', 20, 120.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'003));
//    ASSERT_TRUE(book_->limits_.contains(px_3));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->prev_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->next_order->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->prev_order->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->next_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'004, symbol, 'B', 30, 130.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'004));
//    const uint64_t px_4 = doubleToFixedPoint(130.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_4));
//
//    ASSERT_EQ(book_->sell_side, nullptr);
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->prev_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->next_order->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->prev_order->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->next_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->limit_px, px_4);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->head->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->tail->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'005, symbol, 'S', 50, 200.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'005));
//    const uint64_t px_5 = doubleToFixedPoint(200.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_5));
//
//    // Buy Side
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->prev_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->next_order->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->prev_order->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->next_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->limit_px, px_4);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->head->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->tail->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->right_child, nullptr);
//
//    // Sell Side
//    ASSERT_EQ(book_->sell_side->limit_px, px_5);
//    ASSERT_EQ(book_->sell_side->qty, 50);
//    ASSERT_EQ(book_->sell_side->head->id, 1'005);
//    ASSERT_EQ(book_->sell_side->tail->id, 1'005);
//    ASSERT_EQ(book_->sell_side->parent, nullptr);
//    ASSERT_EQ(book_->sell_side->left_child, nullptr);
//    ASSERT_EQ(book_->sell_side->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'006, symbol, 'S', 120, 210.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'006));
//    const uint64_t px_6 = doubleToFixedPoint(210.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_6));
//
//    // Buy Side
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->prev_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->next_order->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->prev_order->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->next_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->limit_px, px_4);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->head->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->tail->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->right_child, nullptr);
//
//    // Sell Side
//    ASSERT_EQ(book_->sell_side->limit_px, px_5);
//    ASSERT_EQ(book_->sell_side->qty, 50);
//    ASSERT_EQ(book_->sell_side->head->id, 1'005);
//    ASSERT_EQ(book_->sell_side->tail->id, 1'005);
//    ASSERT_EQ(book_->sell_side->parent, nullptr);
//    ASSERT_EQ(book_->sell_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->limit_px, px_6);
//    ASSERT_EQ(book_->sell_side->right_child->qty, 120);
//    ASSERT_EQ(book_->sell_side->right_child->head->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->tail->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->sell_side->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'007, symbol, 'S', 11, 205.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'007));
//    const uint64_t px_7 = doubleToFixedPoint(205.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_7));
//
//    // Buy Side
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->prev_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->next_order->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->prev_order->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->next_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->limit_px, px_4);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->head->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->tail->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->right_child, nullptr);
//
//    // Sell Side
//    ASSERT_EQ(book_->sell_side->limit_px, px_5);
//    ASSERT_EQ(book_->sell_side->qty, 50);
//    ASSERT_EQ(book_->sell_side->head->id, 1'005);
//    ASSERT_EQ(book_->sell_side->tail->id, 1'005);
//    ASSERT_EQ(book_->sell_side->parent, nullptr);
//    ASSERT_EQ(book_->sell_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->limit_px, px_6);
//    ASSERT_EQ(book_->sell_side->right_child->qty, 120);
//    ASSERT_EQ(book_->sell_side->right_child->head->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->tail->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->right_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->left_child->limit_px, px_7);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->qty, 11);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->head->id, 1'007);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->tail->id, 1'007);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->left_child, nullptr);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'008, symbol, 'S', 75, 207.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'008));
//    const uint64_t px_8 = doubleToFixedPoint(207.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_8));
//
//    // Buy Side
//    ASSERT_EQ(book_->buy_side->limit_px, px);
//    ASSERT_EQ(book_->buy_side->qty, 10);
//    ASSERT_EQ(book_->buy_side->head->id, 1'000);
//    ASSERT_EQ(book_->buy_side->tail->id, 1'000);
//    ASSERT_EQ(book_->buy_side->parent, nullptr);
//    ASSERT_EQ(book_->buy_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->limit_px, px_2);
//    ASSERT_EQ(book_->buy_side->right_child->qty, 10);
//    ASSERT_EQ(book_->buy_side->right_child->head->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->tail->id, 1'001);
//    ASSERT_EQ(book_->buy_side->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->limit_px, px_3);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->prev_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->head->next_order->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->id, 1'003);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->prev_order->id, 1'002);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->tail->next_order, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->limit_px, px_4);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->qty, 30);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->head->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->tail->id, 1'004);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->buy_side->right_child->right_child->right_child->right_child, nullptr);
//
//    // Sell Side
//    ASSERT_EQ(book_->sell_side->limit_px, px_5);
//    ASSERT_EQ(book_->sell_side->qty, 50);
//    ASSERT_EQ(book_->sell_side->head->id, 1'005);
//    ASSERT_EQ(book_->sell_side->tail->id, 1'005);
//    ASSERT_EQ(book_->sell_side->parent, nullptr);
//    ASSERT_EQ(book_->sell_side->left_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->limit_px, px_6);
//    ASSERT_EQ(book_->sell_side->right_child->qty, 120);
//    ASSERT_EQ(book_->sell_side->right_child->head->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->tail->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->right_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->left_child->limit_px, px_7);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->qty, 11);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->head->id, 1'007);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->tail->id, 1'007);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->limit_px, px_8);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->qty, 75);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->head->id, 1'008);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->tail->id, 1'008);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->right_child, nullptr);
//
//    book_->insertOrderInBook(Order{1'009, symbol, 'S', 100, 90.000000});
//
//    ASSERT_TRUE(book_->orders_.contains(1'009));
//    const uint64_t px_9 = doubleToFixedPoint(90.000000);
//    ASSERT_TRUE(book_->limits_.contains(px_9));
//
//    // Buy Side
//    ASSERT_EQ(book_->buy_side, nullptr);
//
//    // Sell Side
//    ASSERT_EQ(book_->sell_side->limit_px, px_5);
//    ASSERT_EQ(book_->sell_side->qty, 50);
//    ASSERT_EQ(book_->sell_side->head->id, 1'005);
//    ASSERT_EQ(book_->sell_side->tail->id, 1'005);
//    ASSERT_EQ(book_->sell_side->parent, nullptr);
//
//    ASSERT_EQ(book_->sell_side->left_child->limit_px, px_9);
//    ASSERT_EQ(book_->sell_side->left_child->qty, 20);
//    ASSERT_EQ(book_->sell_side->left_child->head->id, 1'009);
//    ASSERT_EQ(book_->sell_side->left_child->tail->id, 1'009);
//    ASSERT_EQ(book_->sell_side->left_child->left_child, nullptr);
//    ASSERT_EQ(book_->sell_side->left_child->right_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->limit_px, px_6);
//    ASSERT_EQ(book_->sell_side->right_child->qty, 120);
//    ASSERT_EQ(book_->sell_side->right_child->head->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->tail->id, 1'006);
//    ASSERT_EQ(book_->sell_side->right_child->right_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->left_child->limit_px, px_7);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->qty, 11);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->head->id, 1'007);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->tail->id, 1'007);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->left_child, nullptr);
//
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->limit_px, px_8);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->qty, 75);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->head->id, 1'008);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->tail->id, 1'008);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->left_child, nullptr);
//    ASSERT_EQ(book_->sell_side->right_child->left_child->right_child->right_child, nullptr);
//}