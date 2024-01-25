#pragma once

#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>

// We want to implement the basic functionality of insert, find, and delete
// Then we want to make this container thread-safe, but with fine-grained locks (locking just the bucket of interest) on insert/find/delete

namespace DSA::hash_map {
template <typename K, typename V>
class HashMap final {
public:
    void Insert(std::pair<K, V>&& p);
    bool Contains(const K& key) const;
    void Erase(const K& key);

private:
    
};
}

#endif