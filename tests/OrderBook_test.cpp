#include <gtest/gtest.h>
#include "../src/OrderBook.h" // Include the class we want to test

// Test Fixture: A class to set up a clean state for each test
class OrderBookTest : public ::testing::Test {
protected:
    OrderBook orderBook;
};

// --- TEST 1: Test that a new order is added to an empty book correctly ---
TEST_F(OrderBookTest, AddOrderToEmptyBook) {
    Order newOrder(1, OrderSide::BUY, 100, 10.00);
    orderBook.processOrder(newOrder);

    // Here we would normally check the internal state of the orderBook,
    // but since our members are private, we can't directly.
    // For now, our test is simply that the program doesn't crash.
    // We will add getter functions later to make this more testable.
    SUCCEED(); 
}

// --- TEST 2: Test that a simple match occurs correctly ---
TEST_F(OrderBookTest, SimpleMatch) {
    Order sellOrder(1, OrderSide::SELL, 100, 10.00);
    orderBook.processOrder(sellOrder);

    Order buyOrder(2, OrderSide::BUY, 100, 10.00);
    orderBook.processOrder(buyOrder);

    // How can we verify the trade happened? We need to see the output.
    // This is a great reason to refactor our OrderBook to return trades
    // instead of just printing them. For now, we will run the test and
    // manually check the console output for the "TRADE EXECUTED" message.
    // A more advanced test would capture stdout.
    SUCCEED();
}

// --- TEST 3: Add a single BUY order to an empty book ---
// GTest Syntax: TEST_F(FixtureName, TestName)
TEST_F(OrderBookTest, AddBuyOrderToEmptyBook) {
    // 1. SETUP: Create a new BUY order
    Order newOrder(1, OrderSide::BUY, 100, 10.00);

    // 2. ACTION: Process the order
    orderBook.processOrder(newOrder);

    // 3. VERIFICATION: Check the state of the order book
    const auto& bids = orderBook.getBids(); // We need a getter for this!
    const auto& asks = orderBook.getAsks(); // And this one too!

    // EXPECT that the bids map is NOT empty and the asks map IS empty.
    EXPECT_FALSE(bids.empty());
    EXPECT_TRUE(asks.empty());

    // EXPECT that there is exactly one price level in the bid book.
    EXPECT_EQ(bids.size(), 1);

    // Get the order list at the price level 10.00
    const auto& orderList = bids.at(10.00);
    
    // EXPECT that there is exactly one order in that list.
    EXPECT_EQ(orderList.size(), 1);

    // Get the single order from the list
    const auto& placedOrder = orderList.front();

    // EXPECT that the order in the book has the correct properties.
    EXPECT_EQ(placedOrder.orderId, 1);
    EXPECT_EQ(placedOrder.quantity, 100);
}

// --- TEST 3: Add a single Sell order to an empty book ---
// GTest Syntax: TEST_F(FixtureName, TestName)
TEST_F(OrderBookTest, AddSellOrderToEmptyBook) {
    // 1. SETUP: Create a new BUY order
    Order newOrder(1, OrderSide::SELL, 100, 10.00);

    // 2. ACTION: Process the order
    orderBook.processOrder(newOrder);

    // 3. VERIFICATION: Check the state of the order book
    const auto& bids = orderBook.getBids(); // We need a getter for this!
    const auto& asks = orderBook.getAsks(); // And this one too!

    // EXPECT that the bids map is empty and the asks map IS NOT empty.
    EXPECT_TRUE(bids.empty());
    EXPECT_FALSE(asks.empty());

    // EXPECT that there is exactly one price level in the bid book.
    EXPECT_EQ(asks.size(), 1);

    // Get the order list at the price level 10.00
    const auto& orderList = asks.at(10.00);
    
    // EXPECT that there is exactly one order in that list.
    EXPECT_EQ(orderList.size(), 1);

    // Get the single order from the list
    const auto& placedOrder = orderList.front();

    // EXPECT that the order in the book has the correct properties.
    EXPECT_EQ(placedOrder.orderId, 1);
    EXPECT_EQ(placedOrder.quantity, 100);
}