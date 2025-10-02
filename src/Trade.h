#pragma once
#include <cstdint>

struct Trade {
    // A simple static counter for unique trade IDs
    inline static uint64_t nextTradeId = 1;

    uint64_t tradeId;
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    double price;
    uint32_t quantity;

    Trade(uint64_t bId, uint64_t sId, double p, uint32_t q)
        : tradeId{++nextTradeId}, 
        buyOrderId{bId}, 
        sellOrderId{sId},
        price{p},
        quantity{q} {}
};