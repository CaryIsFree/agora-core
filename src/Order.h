#pragma once

#include <cstdint>

enum class OrderSide {BUY, SELL};

struct Order {
    uint64_t orderID;
    OrderSide side;
    uint32_t quantity;
    double price;

    Order(uint64_t id, OrderSide orderSide, uint32_t qty, double p)
        : orderID{id}, side{orderSide}, quantity{qty}, price{p} {} 
};