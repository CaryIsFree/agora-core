#include "Order.h"
#include <map>
#include <list>

class OrderBook {
    private:
        //Asks: sorts from LOWEST price to HIGHEST price (default behavior)
        std::map<double, std::list<Order>> asks_;

        //Bids: sorts from HIGHEST price to LOWEST price (std::greater)
        std::map<double, std::list<Order>, std::greater<double>> bids_;

        //Helper function to handle matching logic
        void matchOrders(Order orderToMatch);

    public:
        //It will take a new order and process it against the book
        void processOrder(const Order& newOrder);
};