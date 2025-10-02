#include <gtest/gtest.h>
#include "../src/OrderBook.h" 

// Test Fixture: A class to set up a clean OrderBook for each test
class OrderBookTest : public ::testing::Test {
protected:
    OrderBook orderBook;
};

// Test that a single BUY order correctly rests in an empty book.
TEST_F(OrderBookTest, AddBuyOrderToEmptyBook) {
    // 1. SETUP: Create a new BUY order
    Order newOrder(1, OrderSide::BUY, 100, 10.00);

    // 2. ACTION: Process the order
    orderBook.processOrder(newOrder);

    // 3. VERIFICATION: Check the state of the order book
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_FALSE(bids.empty());
    EXPECT_TRUE(asks.empty());
    EXPECT_EQ(bids.size(), 1);

    const auto& orderList = bids.at(10.00);
    EXPECT_EQ(orderList.size(), 1);

    const auto& placedOrder = orderList.front();
    EXPECT_EQ(placedOrder.orderId, 1);
    EXPECT_EQ(placedOrder.quantity, 100);
}

// Test that a single SELL order correctly rests in an empty book.
TEST_F(OrderBookTest, AddSellOrderToEmptyBook) {
    // 1. SETUP: Create a new SELL order
    Order newOrder(1, OrderSide::SELL, 100, 10.00);

    // 2. ACTION: Process the order
    orderBook.processOrder(newOrder);

    // 3. VERIFICATION: Check the state of the order book
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_TRUE(bids.empty());
    EXPECT_FALSE(asks.empty());
    EXPECT_EQ(asks.size(), 1);

    const auto& orderList = asks.at(10.00);
    EXPECT_EQ(orderList.size(), 1);

    const auto& placedOrder = orderList.front();
    EXPECT_EQ(placedOrder.orderId, 1);
    EXPECT_EQ(placedOrder.quantity, 100);
}

// --- TEST 3: Test a simple one-to-one full match ---
TEST_F(OrderBookTest, SimpleFullMatchBuyInitiated) {
    // 1. SETUP: Add a resting SELL order to the book
    Order sellOrder(1, OrderSide::SELL, 100, 10.00);
    orderBook.processOrder(sellOrder);

    // 2. ACTION: Process a new BUY order that should completely fill the SELL order
    Order buyOrder(2, OrderSide::BUY, 100, 10.00);
    std::vector<Trade> trades = orderBook.processOrder(buyOrder);

    // 3. VERIFICATION: The most important part!
    // After the trade, the book should be completely empty.
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_TRUE(bids.empty());
    EXPECT_TRUE(asks.empty());

    //Verify that a Trade object was created.
    EXPECT_FALSE(trades.empty());
}

// --- TEST 4: Test a simple one-to-one full match ---
TEST_F(OrderBookTest, SimpleFullMatchSellInitiated) {
    // 1. SETUP: Add a resting BUY order to the book
    Order buyOrder(1, OrderSide::BUY, 100, 10.00);
    orderBook.processOrder(buyOrder);

    // 2. ACTION: Process a new SELL order that should completely fill the BUY order
    Order sellOrder(2, OrderSide::SELL, 100, 10.00);
    std::vector<Trade> trades = orderBook.processOrder(sellOrder);

    // 3. VERIFICATION: The most important part!
    // After the trade, the book should be completely empty.
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_TRUE(bids.empty());
    EXPECT_TRUE(asks.empty());

    // Verify that a Trade object was created.
    EXPECT_FALSE(trades.empty());
}

// --- TEST 5: Test a partial fill where the incoming order is larger ---
TEST_F(OrderBookTest, PartialFillIncomingBuyLarger) {
    // 1. SETUP: Add a resting SELL order for 100 shares
    Order sellOrder(1, OrderSide::SELL, 100, 10.00);
    orderBook.processOrder(sellOrder);

    // 2. ACTION: Process a new, larger BUY order
    Order buyOrder(2, OrderSide::BUY, 150, 10.00);
    orderBook.processOrder(buyOrder);

    // 3. VERIFICATION:
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    // The asks book should now be completely empty.
    EXPECT_TRUE(asks.empty());

    // The bids book should now contain the partially filled remainder of the buy order.
    EXPECT_FALSE(bids.empty());
    EXPECT_EQ(bids.size(), 1);

    // Check the details of the remaining order.
    const auto& orderList = bids.at(10.00);
    EXPECT_EQ(orderList.size(), 1);

    const auto& placedOrder = orderList.front();
    EXPECT_EQ(placedOrder.orderId, 2);      // It's the buyer's order
    EXPECT_EQ(placedOrder.quantity, 50);    // The remaining quantity should be 50
    EXPECT_EQ(placedOrder.price, 10.00);    // At the original price
}

// --- TEST 6: Test a partial fill where the incoming order is larger ---
TEST_F(OrderBookTest, PartialFillIncomingSellLarger) {
    // 1. SETUP: Add a resting BUY order for 100 shares
    Order buyOrder(1, OrderSide::BUY, 100, 10.00);
    orderBook.processOrder(buyOrder);

    // 2. ACTION: Process a new, larger SELL order
    Order sellOrder(2, OrderSide::SELL, 150, 10.00);
    orderBook.processOrder(sellOrder);

    // 3. VERIFICATION:
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    // The bids book should now be completely empty.
    EXPECT_TRUE(bids.empty());

    // The asks book should now contain the partially filled remainder of the sell order.
    EXPECT_FALSE(asks.empty());
    EXPECT_EQ(asks.size(), 1);

    // Check the details of the remaining order.
    const auto& orderList = asks.at(10.00);
    EXPECT_EQ(orderList.size(), 1);

    const auto& placedOrder = orderList.front();
    EXPECT_EQ(placedOrder.orderId, 2);      // It's the seller's order
    EXPECT_EQ(placedOrder.quantity, 50);    // The remaining quantity should be 50
    EXPECT_EQ(placedOrder.price, 10.00);    // At the original price
}

// --- TEST 7: Test a partial fill where the RESTING BUY order is larger ---
TEST_F(OrderBookTest, PartialFillRestingBuyLarger) {
    // 1. SETUP: Add a large resting BUY order for 200 shares
    Order buyOrder(1, OrderSide::BUY, 200, 10.00);
    orderBook.processOrder(buyOrder);

    // 2. ACTION: Process a new, smaller BUY order
    Order sellOrder(2, OrderSide::SELL, 75, 10.00);
    orderBook.processOrder(sellOrder);

    // 3. VERIFICATION:
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    // The asks book should be empty, as the incoming order was fully filled.
    EXPECT_TRUE(asks.empty());

    // The bids book should NOT be empty.
    EXPECT_FALSE(bids.empty());
    EXPECT_EQ(bids.size(), 1);

    // Check the details of the remaining resting order.
    const auto& orderList = bids.at(10.00);
    EXPECT_EQ(orderList.size(), 1);

    const auto& placedOrder = orderList.front();
    EXPECT_EQ(placedOrder.orderId, 1);      // It's the original buyer's order
    EXPECT_EQ(placedOrder.quantity, 125);   // The remaining quantity should be 125 (200 - 75)
}

// --- TEST 8: Test a partial fill where the RESTING SELL order is larger ---
// This test name in your file was a duplicate. I've corrected it.
TEST_F(OrderBookTest, PartialFillRestingSellLarger) {
    Order sellOrder(1, OrderSide::SELL, 200, 10.00);
    orderBook.processOrder(sellOrder);
    Order buyOrder(2, OrderSide::BUY, 75, 10.00);
    orderBook.processOrder(buyOrder);

    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_TRUE(bids.empty());
    EXPECT_FALSE(asks.empty());
    EXPECT_EQ(asks.size(), 1);

    const auto& orderList = asks.at(10.00);
    EXPECT_EQ(orderList.size(), 1);
    const auto& placedOrder = orderList.front();
    EXPECT_EQ(placedOrder.orderId, 1);
    EXPECT_EQ(placedOrder.quantity, 125);
}

// --- TEST 9: Test a multi-level match that "walks the book" for an aggressive BUY ---
TEST_F(OrderBookTest, WalkTheBookBuyInitiated) {
    Order sellOrder1(1, OrderSide::SELL, 50, 10.00);
    Order sellOrder2(2, OrderSide::SELL, 50, 10.01);
    orderBook.processOrder(sellOrder1);
    orderBook.processOrder(sellOrder2);

    Order buyOrder(3, OrderSide::BUY, 100, 10.01);
    orderBook.processOrder(buyOrder);

    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_TRUE(bids.empty());
    EXPECT_TRUE(asks.empty());
}

// --- TEST 10: Test a multi-level match that "walks the book" for an aggressive SELL ---
// Renamed from "TEST 9" to "TEST 10"
TEST_F(OrderBookTest, WalkTheBookSellInitiated) {
    // SETUP: The best buy price should be the HIGHEST price.
    Order buyOrder1(1, OrderSide::BUY, 50, 10.01); // Best price
    Order buyOrder2(2, OrderSide::BUY, 50, 10.00); // Second best price
    orderBook.processOrder(buyOrder1);
    orderBook.processOrder(buyOrder2);

    // ACTION: An aggressive SELL order that should fill both.
    Order sellOrder(3, OrderSide::SELL, 100, 10.00); // Price is low enough to match both
    orderBook.processOrder(sellOrder);

    // VERIFICATION: After both trades, the book should be completely empty.
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_TRUE(bids.empty());
    EXPECT_TRUE(asks.empty());
}