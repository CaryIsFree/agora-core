#include "Order.h"
#include "Trade.h"

#include <unordered_map>
#include <map>
#include <list>
#include <vector>

class OrderBook {
    private:
        //Asks: sorts from LOWEST price to HIGHEST price (default behavior)
        std::map<double, std::list<Order>> asks_;

        //Bids: sorts from HIGHEST price to LOWEST price (std::greater)
        std::map<double, std::list<Order>, std::greater<double>> bids_;

        // Maps a unique OrderID to an iterator pointing to that Order in its list.
        std::unordered_map<uint64_t, std::list<Order>::iterator> orderMap_;

        //Helper function to handle matching logic
        void matchOrders(Order orderToMatch, std::vector<Trade>& trades);

    public:
        //It will take a new order and process it against the book
        std::vector<Trade> processOrder(const Order& newOrder);

        //It will cancel an order inside the orderbook
        void cancelOrder(uint64_t orderId);

        //Getters and Setters
        const std::map<double, std::list<Order>, std::greater<double>>& getBids() const { return bids_; }
        const std::map<double, std::list<Order>>& getAsks() const { return asks_; }
};