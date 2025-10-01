# Agora Core - Engineering Decision Journal

This journal is design to record key architectural and design decisions made during the development of Agora Core. Its purpose is to explain the "why" behind the code, providing references for future development and code reviews.

---

## 1. Choice of Container for the Order Book

**Date:** 2025-09-29

**The Decision:**
The core order book was implemented using 'std::map' with a 'std::list' as the value type:
 - Bids: 'std::map<double, std::list<Order>, std::greater<double>> bids_;'
 - Asks: 'std::map<double, std::list<Order>> asks_;'

**Alternatives Considered:**
1.  `std::vector<Order>`: A single sorted vector. Good for keeping track of orders but slow O(N) operations
2.  `std::unordered_map`: A hash map. No guarantee of ordering.

**Reasoning:**
I selected 'std::map' for 3 main reasons:
 - **Price Priority:** 'std::map' represents a self-balancing binary search tree. It automatically sorts price levels, which is fundamental when we need to keep track of which orders should be fulfilled first. Using 'std::greater<double>' perfectly reverses the sort order that will allow 'bids_' book to be highest-price-first.
 - **Time Priority** The 'std::list' as the value for 'bids_' and 'asks_' will allow for a natural First-In, First-Out (FIFO) approach at each price level. In an orderbook, we'd typically want orders to be fulfilled at a first-come, first-serve basis.
 - **Performance:** Insertions, deletions, and lookups have O(log N) operations (where N is the number of price levels), which for this project, is pretty fast.