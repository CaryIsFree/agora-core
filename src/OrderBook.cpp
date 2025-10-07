#include "OrderBook.h"
#include <iostream>

std::vector<Trade> OrderBook::processOrder(const Order& newOrder) {
    //Create a new instance called trades
    std::vector<Trade> trades;

    if (newOrder.side == OrderSide::BUY) {
        // Logic for a new buy order
        /*
        1. Check: if anyone IS NOT selling 
        or trade is NOT possible (buy price is less than the lowest ask price) 
        then rest it in the bids_ book
        */
        if (asks_.empty() || newOrder.price < asks_.begin()->first) {
            auto& bidList = bids_[newOrder.price];
            bidList.push_back(newOrder);
            orderMap_[newOrder.orderId] = std::prev(bidList.end());
            return trades;
        }

        //Otherwise, a trade possible!
        std::cout << "Match is possible!" << std::endl;
        matchOrders(newOrder, trades);

    }
    
    else if (newOrder.side == OrderSide::SELL) {
        //Logic for a new sell order

        /*
        Check: if there isn't a buy order
        or sell price is HIGHER than the largest buy price (Trade is not possible),
        then rest it in the asks_ book
        */
        if (bids_.empty() || newOrder.price > bids_.begin()->first) {
            auto& askList = asks_[newOrder.price];
            askList.push_back(newOrder);
            orderMap_[newOrder.orderId] = std::prev(askList.end());
            return trades;
        }

        //Otherwise, a trade is possible
        std::cout << "Match is possible!" << std::endl;
        matchOrders(newOrder, trades);
        
    }

    return trades;

}

void OrderBook::matchOrders(Order orderToMatch, std::vector<Trade>& trades) {
    //Logic for fulfilling orders

    /*
    If the order was a buy, enter the buy loop
    */
    if (orderToMatch.side == OrderSide::BUY) {
        /*
        Continue to complete orders until the current order no longer has quantity nor
        the buying price is less than the ask price
        */
        while (orderToMatch.quantity > 0 && !asks_.empty() && orderToMatch.price >= asks_.begin()->first) {
            //Grab all the orders at the beginning of the asks_ book (SAME price)
            auto& bestAskLevel = asks_.begin()->second;

            //Get the oldest order from that level
            auto& oldestAsk = bestAskLevel.front();
            
            //Find the lowest trade quantity between both orders
            uint32_t tradeQuantity = std::min(orderToMatch.quantity, oldestAsk.quantity);
                     
            trades.emplace_back(orderToMatch.orderId, oldestAsk.orderId, oldestAsk.price, tradeQuantity);

            //Subtract the quantity for both orders
            orderToMatch.quantity -= tradeQuantity;
            oldestAsk.quantity -= tradeQuantity;

            //If the oldestAsk quantity is zero, remove it from the book and map
            if (oldestAsk.quantity == 0) {
                orderMap_.erase(oldestAsk.orderId);
                bestAskLevel.pop_front();
            }
                
            //If the level is empty, remove the level as well
            if (bestAskLevel.empty())
                asks_.erase(asks_.begin());
        }

        //If the orderToMatch hasn't been fulfilled, rest it in the book and map
        if (orderToMatch.quantity > 0) {
            auto& bidList = bids_[orderToMatch.price];
            bidList.push_back(orderToMatch);
            orderMap_[orderToMatch.orderId] = std::prev(bidList.end());
        }

    }

    /*
    If the order was an ask, enter the ask loop
    */
    else if (orderToMatch.side == OrderSide::SELL) {
        /*
        Continue to complete orders until the current order no longer has quantity nor
        the selling price is GREATER than the buy price
        */
        while (orderToMatch.quantity > 0 && !bids_.empty() && orderToMatch.price <= bids_.begin()->first) {
            //Grab all the orders at the beginning of the bids_ book (SAME price)
            auto& bestBidLevel = bids_.begin()->second;

            //Get the oldest order from that level
            auto& oldestBid = bestBidLevel.front();
            
            //Find the lowest trade quantity between both orders
            uint32_t tradeQuantity = std::min(orderToMatch.quantity, oldestBid.quantity);
            
            trades.emplace_back(orderToMatch.orderId, oldestBid.orderId, oldestBid.price, tradeQuantity);

            //Subtract the quantity for both orders
            orderToMatch.quantity -= tradeQuantity;
            oldestBid.quantity -= tradeQuantity;

            //If the oldestAsk quantity is zero, remove it from the book
            if (oldestBid.quantity == 0) {
                orderMap_.erase(oldestBid.orderId); //Clean up the map
                bestBidLevel.pop_front();
            }
                
            //If the level is empty, remove the level as well
            if (bestBidLevel.empty())
                bids_.erase(bids_.begin());
        }

        //If the orderToMatch hasn't been fulfilled, rest it in the book and map
        if (orderToMatch.quantity > 0) {
            auto& askList = asks_[orderToMatch.price];
            askList.push_back(orderToMatch);
            orderMap_[orderToMatch.orderId] = std::prev(askList.end());
        }

    }
}

void OrderBook::cancelOrder(uint64_t orderId) {
    // 1. O(1) lookup to find the order's location
    auto it = orderMap_.find(orderId);

    // Order ID not found, maybe it was already filled or cancelled.
    // In a real system, you might log this. For now, we just return.
    if (it == orderMap_.end()) {
        //TODO: ADD SOMETHING TO DO MORE THAN JUST RETURN
        return;
    }

    // 2. Get the iterator to the order in the std::list
    auto& orderIterator = it->second;

    // 3. Get the order's data to know which book to look in
    const Order& order = *orderIterator;
    
    // 4. Erase the order from the correct book
    if (order.side == OrderSide::BUY) {
        // Get the list at the order's price and erase the order using its iterator
        bids_.at(order.price).erase(orderIterator);
        // If the price level is now empty, remove it
        if (bids_.at(order.price).empty()) {
            bids_.erase(order.price);
        }
    } 
    else if (order.side == OrderSide::SELL) {
        asks_.at(order.price).erase(orderIterator);
        if (asks_.at(order.price).empty()) {
            asks_.erase(order.price);
        }
    }

    // 5. O(1) cleanup of the map
    orderMap_.erase(it);
}