# Agora Core - Engineering Decision Journal

This journal is design to record key architectural and design decisions made during the development of Agora Core. Its purpose is to explain the "why" behind the code, providing references for future development and code reviews.

---

## 1. Initial Data Structure for `Order`

**Date:** 2025-09-29

**The Decision:**
The data structure for `Order` will be implemented as a `struct` for the initial MVP.

**Alternatives Considered:**
 - `class` with private data members and public getters/setters.

**Reasoning:**
 - The `Order` type is simply **Plain Old Data (POD)**. It lacks any internal logicthat it requires it to be protected through encapsulation.
 - `struct` clearly communicates this idea.
 - **NOTE**: this decision will be revisited later. If the `Order` type has internal logic, then the `Order` type will refactored into a `class` with proper encapsulation. Only use `class` when there's complexity involved.

---

## 2. Choice of Container for the Order Book

**Date:** 2025-10-01

**The Decision:**
The core order book was implemented using `std::map` with a `std::list` as the value type:
 - Bids: `std::map<double, std::list<Order>, std::greater<double>> bids_;`
 - Asks: `std::map<double, std::list<Order>> asks_;`

**Alternatives Considered:**
1.  `std::vector<Order>`: A single sorted vector.
2.  `std::unordered_map`: A hash map.

**Reasoning:**
I selected `std::map` for 3 main reasons:
 - **Price Priority:** `std::map` represents a self-balancing binary search tree. It automatically sorts price levels, which is fundamental when we need to keep track of which orders should be fulfilled first. Using 'std::greater<double>' perfectly reverses the sort order that will allow 'bids_' book to be highest-price-first.
 - **Time Priority** The `std::list` as the value for `bids_` and `asks_` will allow for a natural **First-In, First-Out (FIFO)** approach at each price level. In an orderbook, we'd typically want orders to be fulfilled at a first-come, first-serve basis.
 - **Performance:** Insertions, deletions, and lookups have O(log N) operations (where N is the number of price levels), which for this project, is pretty fast.

The alternatives were rejected because `std::vector` has slow O(N) operations and `std::unordered_map` provides no sorting.

---
