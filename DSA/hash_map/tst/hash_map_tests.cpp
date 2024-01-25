#include "include.h"
#include "hash_map.hpp"

#include <iostream>

using namespace DSA::hash_map;

int main() {
    // Test the following:
    // .insert(std::pair) -> also implement resize here
    // operator[](const K& key) -> insert if not found 
    // 
    HashMap<std::string, int> m;
    
    std::cout << m << std::endl;

    return 0;
}

//TEST()