#include "OrderBook.h"
#include <iostream>

void OrderBook::processOrder(const Order& newOrder) {
    if (newOrder.side == OrderSide::BUY) {
        // Logic for a new buy order
        /*
        1. Check: if anyone IS NOT selling 
        or trade is NOT possible (buy price is less than the lowest ask price) 
        then rest it in the bids_ book
        */
        if (asks_.empty() || newOrder.price < asks_.begin()->first) {
            bids_[newOrder.price].push_back(newOrder);
            return;
        }

        //Otherwise, a trade possible!
        std::cout << "Match is possible!" << std::endl;
        matchOrders(newOrder);

    }
    
    else if (newOrder.side == OrderSide::SELL) {
        //Logic for a new sell order

        /*
        Check: if there isn't a buy order
        or sell price is HIGHER than the largest buy price (Trade is not possible),
        then rest it in the asks_ book
        */
        if (bids_.empty() || newOrder.price > bids_.begin()->first) {
            asks_[newOrder.price].push_back(newOrder);
            return;
        }

        //Otherwise, a trade is possible
        std::cout << "Match is possible!" << std::endl;
        matchOrders(newOrder);
        
    }
}

void OrderBook::matchOrders(Order orderToMatch) {
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

            //Print a statement to show that this trade has occurred
            std::cout << "TRADE EXECUTED: " << tradeQuantity << " shares @ " << oldestAsk.price << std::endl;

            //Subtract the quantity for both orders
            orderToMatch.quantity -= tradeQuantity;
            oldestAsk.quantity -= tradeQuantity;

            //If the oldestAsk quantity is zero, remove it from the book
            if (oldestAsk.quantity == 0)
                bestAskLevel.pop_front();
            
            //If the level is empty, remove the level as well
            if (bestAskLevel.empty())
                asks_.erase(asks_.begin());
        }

        //If the orderToMatch hasn't been fulfilled, rest it in the book
        if (orderToMatch.quantity > 0)
            bids_[orderToMatch.price].push_back(orderToMatch);

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
            
            //Print a statement to show that this trade has occurred
            std::cout << "TRADE EXECUTED: " << tradeQuantity << " shares @ " << oldestBid.price << std::endl;

            //Subtract the quantity for both orders
            orderToMatch.quantity -= tradeQuantity;
            oldestBid.quantity -= tradeQuantity;

            //If the oldestAsk quantity is zero, remove it from the book
            if (oldestBid.quantity == 0)
                bestBidLevel.pop_front();
            
            //If the level is empty, remove the level as well
            if (bestBidLevel.empty())
                bids_.erase(bids_.begin());
        }

        //If the orderToMatch hasn't been fulfilled, rest it in the book
        if (orderToMatch.quantity > 0)
            asks_[orderToMatch.price].push_back(orderToMatch);

    }
}