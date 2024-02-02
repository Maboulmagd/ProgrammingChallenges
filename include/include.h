//
// Created by Mo Aboulmagd on 10/17/21.
//

// Header file for includes and namespaces

#pragma once

#include <iostream>

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <system_error>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace ::testing;

// Definition for a binary tree node.
 struct TreeNode {
     int val;// TODO Try using std::variant/template here?
     TreeNode* left;
     TreeNode* right;
     TreeNode() : val(0), left(nullptr), right(nullptr) {}
     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
     TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
     ~TreeNode() {
//         if (left) {
//             delete left;
//         }
//         if (right) {
//             delete right;
//         }
     }
     //bool operator==(const TreeNode* b);
     friend bool TreesEqual(const TreeNode* lhs, const TreeNode* rhs);// NOTE: I highly suggest against overloading operator== for pointers. It has very surprising semantics.
     friend void DeleteTree(TreeNode*& tree);
 };

string serialize(TreeNode* root);
TreeNode* deserialize(string data);

// Definition for a linked list node.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}

    friend void DeleteList(ListNode*& curr);
};

template <typename T>
struct Point2D {
    T x;
    T y;
};

enum class InputError {
    OK = 0,
    ERROR
};

struct InputErrorCategory : std::error_category {
    const char* name() const noexcept override {
        return "transaction";
    }
    std::string message(int condition) const override {
        using namespace std::string_literals;
        switch (condition) {
            case 0 : return "OK"s;
            case 1 : return "INPUT ERROR"s;
        }
        std::abort();// Unreachable
    }
};

// Register the enum as an error code enum
template<>
struct std::is_error_code_enum<InputError> : public std::true_type {};

// Mapping from error code enum to category
std::error_code make_error_code(InputError e) {
    static auto category = InputErrorCategory{};
    return std::error_code(static_cast<int>(e), category);
    //return std::error_code(std::to_underlying(e), category);
}