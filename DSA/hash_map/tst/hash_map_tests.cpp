#include "include.h"
#include "hash_map.hpp"

using namespace DSA::hash_map;

// class HashMapTest : public testing::Test {
// protected:
//     void SetUp() override {

//     }

//     // void TearDown() override {}

//     HashMap<int, int> map_;
// };

// TEST_F(HashMapTest, IsEmptyInitially) {
//   EXPECT_EQ(map_.size(), 0);
// }

// TEST_F(HashMapTest, PrintEmptyMap) {
//     // Act: Use the operator<< to generate the output
//     std::stringstream output;
//     output << map_;

//     // Assert: Check the expected output
//     EXPECT_EQ(output.str(), "");
// }

// TEST_F(HashMapTest, Insert) {
//     map_.insert({10, 100});

//     EXPECT_EQ(map_.size(), 1);

//     const std::list<std::pair<int, int>>* buffer = map_.get_buffer();

//     for (std::size_t i = 0; i < map_.bucket_count(); ++i) {
//         const std::list<std::pair<int, int>>& lst = buffer[i];
//         for (auto it = lst.begin(); it != lst.end(); ++it) {
//             std::cout << "Key: " << it->first << "\t" << "Value: " << it->second << '\n';
//         }
//     }
// }

// TEST_F(HashMapTest, Get) {
//     const std::list<std::pair<int, int>>* buffer = map_.get_buffer();

//     for (std::size_t i = 0; i < map_.bucket_count(); ++i) {
//         const std::list<std::pair<int, int>>& lst = buffer[i];
//         for (auto it = lst.begin(); it != lst.end(); ++it) {
//             std::cout << "Key: " << it->first << "\t" << "Value: " << it->second << '\n';
//         }
//     }

//     EXPECT_EQ(map_[10], 100);
// }

// TEST_F(HashMapTest, PrintMapThatContainsOneElement) {
//     // Act: Use the operator<< to generate the output
//     std::stringstream output;
//     output << map_;

//     // Assert: Check the expected output
//     EXPECT_EQ(output.str(), "Key: 0\tValue: Hussein Abu Alim");
// }

// TEST_F(HashMapTest, Erase) {

// }

TEST(HashMapTest, StressTest) {
    HashMap<int, int> map;

    map.insert({10, 100});
    EXPECT_EQ(map.size(), 1);

    map.insert({20, 200});
    EXPECT_EQ(map.size(), 2);

    map.insert({30, 300});
    EXPECT_EQ(map.size(), 3);

    map.insert({40, 400});
    EXPECT_EQ(map.size(), 4);

    map.insert({50, 500});
    EXPECT_EQ(map.size(), 5);

    const std::list<std::pair<int, int>>* buffer = map.get_buffer();
    for (std::size_t i = 0; i < map.bucket_count(); ++i) {
        const std::list<std::pair<int, int>>& lst = buffer[i];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            std::cout << "Key: " << it->first << "\t" << "Value: " << it->second << '\n';
        }
    }
    
    // Try operator[] inserting
    std::cout << map[60] << '\n';
    EXPECT_EQ(map.size(), 6);

    std::cout << '\n';
    for (std::size_t i = 0; i < map.bucket_count(); ++i) {
        const std::list<std::pair<int, int>>& lst = buffer[i];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            std::cout << "Key: " << it->first << "\t" << "Value: " << it->second << '\n';
        }
    }

    // Try deleting
    map.erase(60);
    EXPECT_EQ(map.size(), 5);

    std::cout << '\n';
    for (std::size_t i = 0; i < map.bucket_count(); ++i) {
        const std::list<std::pair<int, int>>& lst = buffer[i];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            std::cout << "Key: " << it->first << "\t" << "Value: " << it->second << '\n';
        }
    }

    // Try replacing value of a key that exists
    map.insert({50, 5000});
    EXPECT_EQ(map.size(), 5);

    std::cout << '\n';
    for (std::size_t i = 0; i < map.bucket_count(); ++i) {
        const std::list<std::pair<int, int>>& lst = buffer[i];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            std::cout << "Key: " << it->first << "\t" << "Value: " << it->second << '\n';
        }
    }
}