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
TEST_F(OrderBookTests, IncreasingBuysAndSellMatchesAll) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1'000, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1'000));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'001, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(1'001));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'002, symbol, 'B', 10, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'002));
    const uint64_t px_3 = doubleToFixedPoint(120.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'002);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'003, symbol, 'B', 20, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'003));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'004, symbol, 'B', 30, 130.000000});

    ASSERT_TRUE(book_->orders_.contains(1'004));
    const uint64_t px_4 = doubleToFixedPoint(130.000000);
    ASSERT_TRUE(book_->buy_side.contains(px_4));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'005, symbol, 'S', 80, 90.000000});

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_TRUE(book_->orders_.empty());
}

// Increasing buys followed with a sell that consumes all, BUT does have leftover qty
TEST_F(OrderBookTests, BasicBuysAndSellMatchesAllWithLeftover) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1'000, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1'000));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'001, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(1'001));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'002, symbol, 'B', 10, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'002));
    const uint64_t px_3 = doubleToFixedPoint(120.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'002);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'003, symbol, 'B', 20, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'003));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'004, symbol, 'B', 30, 130.000000});

    ASSERT_TRUE(book_->orders_.contains(1'004));
    const uint64_t px_4 = doubleToFixedPoint(130.000000);
    ASSERT_TRUE(book_->buy_side.contains(px_4));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'005, symbol, 'S', 100, 90.000000});

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_TRUE(book_->orders_.contains(1'005));
    const uint64_t px_5 = doubleToFixedPoint(90.000000);

    it = book_->sell_side.begin();

    ASSERT_EQ(it->second->limit_px, px_5);
    ASSERT_EQ(it->second->qty, 20);
    ASSERT_EQ(it->second->head->id, 1'005);
    ASSERT_EQ(it->second->tail->id, 1'005);
}

// Increasing buys followed with random RESTING sells, then a sell that consumes all buys, but has leftover qty
TEST_F(OrderBookTests, BasicBuysAndRestingSellsThenSellMatchesAllWithLeftover) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1'000, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1'000));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'001, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(1'001));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'002, symbol, 'B', 10, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'002));
    const uint64_t px_3 = doubleToFixedPoint(120.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'002);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'003, symbol, 'B', 20, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'003));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'004, symbol, 'B', 30, 130.000000});

    ASSERT_TRUE(book_->orders_.contains(1'004));
    const uint64_t px_4 = doubleToFixedPoint(130.000000);
    ASSERT_TRUE(book_->buy_side.contains(px_4));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'005, symbol, 'S', 50, 200.000000});

    ASSERT_TRUE(book_->orders_.contains(1'005));
    const uint64_t px_5 = doubleToFixedPoint(200.000000);

    // Buy Side
    it = book_->buy_side.begin();

    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    // Sell Side
    it = book_->sell_side.begin();
    ASSERT_EQ(it->second->limit_px, px_5);
    ASSERT_EQ(it->second->qty, 50);
    ASSERT_EQ(it->second->head->id, 1'005);
    ASSERT_EQ(it->second->tail->id, 1'005);

    book_->insertOrderInBook(Order{1'006, symbol, 'S', 120, 210.000000});

    ASSERT_TRUE(book_->orders_.contains(1'006));
    const uint64_t px_6 = doubleToFixedPoint(210.000000);

    // Buy Side
    it = book_->buy_side.begin();

    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    // Sell Side
    it = book_->sell_side.begin();

    ASSERT_EQ(it->second->limit_px, px_5);
    ASSERT_EQ(it->second->qty, 50);
    ASSERT_EQ(it->second->head->id, 1'005);
    ASSERT_EQ(it->second->tail->id, 1'005);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_6);
    ASSERT_EQ(it->second->qty, 120);
    ASSERT_EQ(it->second->head->id, 1'006);
    ASSERT_EQ(it->second->tail->id, 1'006);

    book_->insertOrderInBook(Order{1'007, symbol, 'S', 11, 205.000000});

    ASSERT_TRUE(book_->orders_.contains(1'007));
    const uint64_t px_7 = doubleToFixedPoint(205.000000);

    // Buy Side
    it = book_->buy_side.begin();

    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    // Sell Side
    it = book_->sell_side.begin();

    ASSERT_EQ(it->second->limit_px, px_5);
    ASSERT_EQ(it->second->qty, 50);
    ASSERT_EQ(it->second->head->id, 1'005);
    ASSERT_EQ(it->second->tail->id, 1'005);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_7);
    ASSERT_EQ(it->second->qty, 11);
    ASSERT_EQ(it->second->head->id, 1'007);
    ASSERT_EQ(it->second->tail->id, 1'007);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_6);
    ASSERT_EQ(it->second->qty, 120);
    ASSERT_EQ(it->second->head->id, 1'006);
    ASSERT_EQ(it->second->tail->id, 1'006);

    book_->insertOrderInBook(Order{1'008, symbol, 'S', 75, 207.000000});

    ASSERT_TRUE(book_->orders_.contains(1'008));
    const uint64_t px_8 = doubleToFixedPoint(207.000000);

    // Buy Side
    it = book_->buy_side.begin();

    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    // Sell Side
    it = book_->sell_side.begin();

    ASSERT_EQ(it->second->limit_px, px_5);
    ASSERT_EQ(it->second->qty, 50);
    ASSERT_EQ(it->second->head->id, 1'005);
    ASSERT_EQ(it->second->tail->id, 1'005);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_7);
    ASSERT_EQ(it->second->qty, 11);
    ASSERT_EQ(it->second->head->id, 1'007);
    ASSERT_EQ(it->second->tail->id, 1'007);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_8);
    ASSERT_EQ(it->second->qty, 75);
    ASSERT_EQ(it->second->head->id, 1'008);
    ASSERT_EQ(it->second->tail->id, 1'008);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_6);
    ASSERT_EQ(it->second->qty, 120);
    ASSERT_EQ(it->second->head->id, 1'006);
    ASSERT_EQ(it->second->tail->id, 1'006);

    book_->insertOrderInBook(Order{1'009, symbol, 'S', 100, 90.000000});

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_TRUE(book_->orders_.contains(1'009));
    const uint64_t px_9 = doubleToFixedPoint(90.000000);

    // Sell Side
    it = book_->sell_side.begin();

    ASSERT_EQ(it->second->limit_px, px_9);
    ASSERT_EQ(it->second->qty, 20);
    ASSERT_EQ(it->second->head->id, 1'009);
    ASSERT_EQ(it->second->tail->id, 1'009);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_5);
    ASSERT_EQ(it->second->qty, 50);
    ASSERT_EQ(it->second->head->id, 1'005);
    ASSERT_EQ(it->second->tail->id, 1'005);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_7);
    ASSERT_EQ(it->second->qty, 11);
    ASSERT_EQ(it->second->head->id, 1'007);
    ASSERT_EQ(it->second->tail->id, 1'007);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_8);
    ASSERT_EQ(it->second->qty, 75);
    ASSERT_EQ(it->second->head->id, 1'008);
    ASSERT_EQ(it->second->tail->id, 1'008);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_6);
    ASSERT_EQ(it->second->qty, 120);
    ASSERT_EQ(it->second->head->id, 1'006);
    ASSERT_EQ(it->second->tail->id, 1'006);
}

// Increasing sells followed with a buy that consumes all, with no leftover qty
TEST_F(OrderBookTests, BasicSellsThenBuyMatchesAll) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1'000, symbol, 'S', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1'000));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'001, symbol, 'S', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(1'001));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    book_->insertOrderInBook(Order{1'002, symbol, 'S', 10, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'002));
    const uint64_t px_3 = doubleToFixedPoint(120.000000);

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'002);

    book_->insertOrderInBook(Order{1'003, symbol, 'S', 20, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'003));

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    book_->insertOrderInBook(Order{1'004, symbol, 'S', 30, 130.000000});

    ASSERT_TRUE(book_->orders_.contains(1'004));
    const uint64_t px_4 = doubleToFixedPoint(130.000000);

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    book_->insertOrderInBook(Order{1'005, symbol, 'S', 10, 130.000000});

    ASSERT_TRUE(book_->orders_.contains(1'005));

    ASSERT_TRUE(book_->buy_side.empty());

    // Sell Side
    it = book_->sell_side.begin();

    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 40);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'005);

    book_->insertOrderInBook(Order{1'006, symbol, 'B', 90, 200.000000});

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_TRUE(book_->orders_.empty());
}

// Increasing sells followed with a buy that consumes all, but does have leftover qty
TEST_F(OrderBookTests, IncreasingSellsThenBuyMatchesAll) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1'000, symbol, 'S', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1'000));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    book_->insertOrderInBook(Order{1'001, symbol, 'S', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(1'001));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    book_->insertOrderInBook(Order{1'002, symbol, 'S', 10, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'002));
    const uint64_t px_3 = doubleToFixedPoint(120.000000);

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'002);

    book_->insertOrderInBook(Order{1'003, symbol, 'S', 20, 120.000000});

    ASSERT_TRUE(book_->orders_.contains(1'003));

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    book_->insertOrderInBook(Order{1'004, symbol, 'S', 30, 130.000000});

    ASSERT_TRUE(book_->orders_.contains(1'004));
    const uint64_t px_4 = doubleToFixedPoint(130.000000);

    it = book_->sell_side.begin();

    ASSERT_TRUE(book_->buy_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'004);

    book_->insertOrderInBook(Order{1'005, symbol, 'S', 10, 130.000000});

    ASSERT_TRUE(book_->orders_.contains(1'005));

    ASSERT_TRUE(book_->buy_side.empty());

    // Sell Side
    it = book_->sell_side.begin();

    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'000);
    ASSERT_EQ(it->second->tail->id, 1'000);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'001);
    ASSERT_EQ(it->second->tail->id, 1'001);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 1'002);
    ASSERT_EQ(it->second->tail->id, 1'003);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_4);
    ASSERT_EQ(it->second->qty, 40);
    ASSERT_EQ(it->second->head->id, 1'004);
    ASSERT_EQ(it->second->tail->id, 1'005);

    book_->insertOrderInBook(Order{1'006, symbol, 'B', 100, 200.000000});

    ASSERT_FALSE(book_->buy_side.empty());
    ASSERT_TRUE(book_->orders_.contains(1'006));
    const uint64_t px_5 = doubleToFixedPoint(200.000000);

    it = book_->buy_side.begin();

    ASSERT_EQ(it->second->limit_px, px_5);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1'006);
    ASSERT_EQ(it->second->tail->id, 1'006);
}

TEST_F(OrderBookTests, RandomBuysFollowedWithCancellingHighestBid) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{2, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(2));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->tail->id, 2);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{3, symbol, 'B', 10, 105.000000});

    ASSERT_TRUE(book_->orders_.contains(3));
    const uint64_t px_3 = doubleToFixedPoint(105.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->tail->id, 2);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->takeOrderOffBook(2);

    ASSERT_TRUE(book_->orders_.contains(1));
    ASSERT_TRUE(book_->orders_.contains(3));

    it = book_->buy_side.begin();
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);
}

TEST_F(OrderBookTests, RandomBuysFollowedWithCancellingHeadOrder) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{2, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(2));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->tail->id, 2);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{3, symbol, 'B', 20, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(3));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);
    ASSERT_EQ(it->second->tail->prev_order->id, 2);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{4, symbol, 'B', 10, 105.000000});

    ASSERT_TRUE(book_->orders_.contains(4));
    const uint64_t px_3 = doubleToFixedPoint(105.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);
    ASSERT_EQ(it->second->tail->prev_order->id, 2);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 4);
    ASSERT_EQ(it->second->tail->id, 4);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->takeOrderOffBook(2);

    ASSERT_TRUE(book_->orders_.contains(1));
    ASSERT_TRUE(book_->orders_.contains(3));
    ASSERT_TRUE(book_->orders_.contains(4));

    it = book_->buy_side.begin();
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 20);
    ASSERT_EQ(it->second->head->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 4);
    ASSERT_EQ(it->second->tail->id, 4);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);
}

TEST_F(OrderBookTests, RandomBuysFollowedWithCancellingMiddleOrder) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{2, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(2));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->tail->id, 2);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{3, symbol, 'B', 20, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(3));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);
    ASSERT_EQ(it->second->tail->prev_order->id, 2);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{4, symbol, 'B', 30, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(4));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 60);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->head->next_order->prev_order->id, 2);
    ASSERT_EQ(it->second->head->next_order->next_order->id, 4);
    ASSERT_EQ(it->second->tail->id, 4);
    ASSERT_EQ(it->second->tail->prev_order->id, 3);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{5, symbol, 'B', 10, 105.000000});

    ASSERT_TRUE(book_->orders_.contains(5));
    const uint64_t px_3 = doubleToFixedPoint(105.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 60);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->head->next_order->prev_order->id, 2);
    ASSERT_EQ(it->second->head->next_order->next_order->id, 4);
    ASSERT_EQ(it->second->tail->id, 4);
    ASSERT_EQ(it->second->tail->prev_order->id, 3);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 5);
    ASSERT_EQ(it->second->tail->id, 5);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->takeOrderOffBook(3);

    ASSERT_TRUE(book_->orders_.contains(1));
    ASSERT_TRUE(book_->orders_.contains(2));
    ASSERT_TRUE(book_->orders_.contains(4));
    ASSERT_TRUE(book_->orders_.contains(5));

    it = book_->buy_side.begin();
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 40);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 4);
    ASSERT_EQ(it->second->tail->id, 4);
    ASSERT_EQ(it->second->tail->prev_order->id, 2);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 5);
    ASSERT_EQ(it->second->tail->id, 5);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);
}

TEST_F(OrderBookTests, RandomBuysFollowedWithCancellingTailOrder) {
    char symbol[8] = "IBM";
    book_->insertOrderInBook(Order{1, symbol, 'B', 10, 100.000000});

    ASSERT_TRUE(book_->orders_.contains(1));
    const uint64_t px = doubleToFixedPoint(100.000000);

    auto it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{2, symbol, 'B', 10, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(2));
    const uint64_t px_2 = doubleToFixedPoint(110.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->tail->id, 2);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{3, symbol, 'B', 20, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(3));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);
    ASSERT_EQ(it->second->tail->prev_order->id, 2);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{4, symbol, 'B', 30, 110.000000});

    ASSERT_TRUE(book_->orders_.contains(4));

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 60);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->head->next_order->prev_order->id, 2);
    ASSERT_EQ(it->second->head->next_order->next_order->id, 4);
    ASSERT_EQ(it->second->tail->id, 4);
    ASSERT_EQ(it->second->tail->prev_order->id, 3);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->insertOrderInBook(Order{5, symbol, 'B', 10, 105.000000});

    ASSERT_TRUE(book_->orders_.contains(5));
    const uint64_t px_3 = doubleToFixedPoint(105.000000);

    it = book_->buy_side.begin();

    ASSERT_TRUE(book_->sell_side.empty());
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 60);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->head->next_order->prev_order->id, 2);
    ASSERT_EQ(it->second->head->next_order->next_order->id, 4);
    ASSERT_EQ(it->second->tail->id, 4);
    ASSERT_EQ(it->second->tail->prev_order->id, 3);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 5);
    ASSERT_EQ(it->second->tail->id, 5);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);

    book_->takeOrderOffBook(4);

    ASSERT_TRUE(book_->orders_.contains(1));
    ASSERT_TRUE(book_->orders_.contains(2));
    ASSERT_TRUE(book_->orders_.contains(3));
    ASSERT_TRUE(book_->orders_.contains(5));

    it = book_->buy_side.begin();
    ASSERT_EQ(it->second->limit_px, px_2);
    ASSERT_EQ(it->second->qty, 30);
    ASSERT_EQ(it->second->head->id, 2);
    ASSERT_EQ(it->second->head->prev_order, nullptr);
    ASSERT_EQ(it->second->head->next_order->id, 3);
    ASSERT_EQ(it->second->tail->id, 3);
    ASSERT_EQ(it->second->tail->prev_order->id, 2);
    ASSERT_EQ(it->second->tail->next_order, nullptr);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px_3);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 5);
    ASSERT_EQ(it->second->tail->id, 5);

    it = std::next(it);
    ASSERT_EQ(it->second->limit_px, px);
    ASSERT_EQ(it->second->qty, 10);
    ASSERT_EQ(it->second->head->id, 1);
    ASSERT_EQ(it->second->tail->id, 1);
}