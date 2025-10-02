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