#pragma once

#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <iostream>
#include <vector>
#include <list>
#include <utility>

// We want to implement the basic functionality of insert, find, and delete
// Then we want to make this container thread-safe, but with fine-grained locks (locking just the bucket of interest) on insert/find/delete

namespace DSA::hash_map {
template <typename K, typename V>
class HashMap final {
public:
    void insert(const std::pair<K, V>& key_val) {
        
    }

    std::size_t size() const {
        return 0;
    }

    V& operator[](const K& key) {
        
    }

    friend std::ostream& operator<<(std::ostream& os, const HashMap<K, V>& map) {
        return os;
    }

private:
    void resize() {

    }

    std::size_t buffer_size_;
    std::vector<std::list<std::pair<K, V>>> buffer_;
    std::size_t total_elements_;    
};
}

#endif