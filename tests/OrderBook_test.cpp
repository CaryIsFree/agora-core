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
TEST_F(OrderBookTest, SimpleFullMatch) {
    // 1. SETUP: Add a resting SELL order to the book
    Order sellOrder(1, OrderSide::SELL, 100, 10.00);
    orderBook.processOrder(sellOrder);

    // 2. ACTION: Process a new BUY order that should completely fill the SELL order
    Order buyOrder(2, OrderSide::BUY, 100, 10.00);
    orderBook.processOrder(buyOrder);

    // 3. VERIFICATION: The most important part!
    // After the trade, the book should be completely empty.
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();

    EXPECT_TRUE(bids.empty());
    EXPECT_TRUE(asks.empty());

    // In the future, we will also verify that a Trade object was created.
}

// --- TEST 4: Test a partial fill where the incoming order is larger ---
TEST_F(OrderBookTest, PartialFillIncomingOrderLarger) {
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