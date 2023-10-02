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
