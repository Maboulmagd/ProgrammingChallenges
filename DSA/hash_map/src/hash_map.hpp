#pragma once

#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include "include.h"

#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <concepts>

// We want to implement the basic functionality of insert, find, and delete
// Then we want to make this container thread-safe, but with fine-grained locks (locking just the bucket of interest) on insert/find/delete
namespace DSA::hash_map {
static constexpr float EPSILON_FLOAT = 0.0000001f;

template <typename Key>
concept HashableKey = requires(const Key& key) {
    { std::hash<Key>{}(key) } -> std::convertible_to<int64_t>;
};

// Alias HashableKey as K for convenience
template <HashableKey K>
using Key = K;

template <typename Value>
concept DefaultConstructibleValue = std::default_initializable<Value>;

// Alias Value as V for convenience
template <DefaultConstructibleValue V>
using Value = V;

template <typename K, typename V>
class HashMap final {
public:
    explicit HashMap() : num_buckets_(2), total_elements_(0), load_factor_(0.75f), growth_factor_(2) {
        buffer_ = new std::list<std::pair<K, V>>[num_buckets_];
        assert(buffer_ != nullptr);
    }

    ~HashMap() {
        delete[] buffer_;
    }

    void insert(const std::pair<K, V>& key_val) {
        std::size_t bucket = std::hash<K>{}(key_val.first) % num_buckets_;

        // Need to check if key_val.first already exists in that list, if exists, simply update value
        auto it = get_iterator(bucket, key_val.first);
        if (it == buffer_[bucket].end()) {// Insert
            // Prior to inserting, let's check the load factor and resize, if need be

            const float lf = total_elements_ / num_buckets_;
            if ((lf - load_factor_) >= EPSILON_FLOAT) {// Time to resize
                resize();
            }

            // Before inserting, let's recompute hash, then finally insert
            bucket = std::hash<K>{}(key_val.first) % num_buckets_;
            buffer_[bucket].push_back(key_val);

            assert(buffer_[bucket].size() > 0);
            assert(buffer_[bucket].back().first == key_val.first);
            assert(buffer_[bucket].back().second == key_val.second);

            ++total_elements_;
        }
        else {// Simply update value
            it->second = key_val.second;
        }
    }

    void erase(const K& key_to_erase) {
        // First let's check if key even exists
        std::size_t bucket = std::hash<K>{}(key_to_erase) % num_buckets_;

        auto it = get_iterator(bucket, key_to_erase);
        if (it == buffer_[bucket].cend()) { return; }// Don't do anything, nothing to erase

        buffer_[bucket].erase(it);
        --total_elements_;

        assert(get_iterator(bucket, key_to_erase) == buffer_[bucket].cend());
    }

    const std::list<std::pair<K, V>>* get_buffer() const {
        return buffer_;
    }

    std::size_t bucket_count() const {
        return num_buckets_;
    }

    std::size_t size() const {
        return total_elements_;
    }

    V& operator[](const K& key) {
        // Check if its in our map first, if not, then insert with default value V
        std::size_t bucket = std::hash<K>{}(key) % num_buckets_;

        auto it = get_iterator(bucket, key);
        if (it == buffer_[bucket].end()) {// Insert
            insert(std::make_pair(key, V{}));

            // Need to recalculate hash, since buffer may have been resized after possible insert
            bucket = std::hash<K>{}(key) % num_buckets_;
            it = get_iterator(bucket, key);
            assert(it != buffer_[bucket].end());
        }

        return it->second;
    }

    friend std::ostream& operator<<(std::ostream& os, const HashMap<K, V>& map) {
        const std::list<std::pair<K, V>>* buffer = map.get_buffer();

        for (std::size_t i = 0; i < map.bucket_count(); ++i) {
            const std::list<std::pair<K, V>>& lst = buffer[i];
            for (auto it = lst.begin(); it != lst.end(); ++it) {
                os << "Key: " << it->first << "\t" << "Value: " << it->second << '\n';
            }
        }

        return os;
    }

private:
    auto get_iterator(const std::size_t lookup_bucket, const K& key) const {
        std::list<std::pair<K, V>>& lst = buffer_[lookup_bucket];

        auto it = lst.begin();
        for (; it != lst.end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }

        return it;// Could return end
    }

    void resize() {
        // Allocate new array
        const std::size_t new_num_buckets = num_buckets_ * growth_factor_;
        std::list<std::pair<K, V>>* new_buffer = new std::list<std::pair<K, V>>[new_num_buckets];

        // Copy over all old elements
        for (std::size_t i = 0; i < num_buckets_; ++i) {
            const std::list<std::pair<K, V>>& lst = buffer_[i];
            for (auto it = lst.begin(); it != lst.end(); ++it) {
                const std::size_t key_hash = std::hash<K>{}(it->first) % new_num_buckets;// Make sure to modulo by new_num_buckets_!

                new_buffer[key_hash].push_back(*it);
            }
        }

        // Update num_buckets_ and buffer_ to point to new_buffer
        delete[] buffer_;
        buffer_ = new_buffer;
        num_buckets_ = new_num_buckets;
    }

    std::size_t num_buckets_;
    std::list<std::pair<K, V>>* buffer_;
    std::size_t total_elements_;
    const float load_factor_;
    const std::size_t growth_factor_;
};
}

#endif