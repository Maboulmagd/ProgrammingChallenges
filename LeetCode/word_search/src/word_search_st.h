//
// Created by Mo Aboulmagd on 1/17/21.
//

#ifndef PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHST_H
#define PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHST_H

#include <vector>
#include <string>
#include <utility>

#include "word_search.h"

// Complexity Analysis:
// Time: O(M*N) * O(M*N)
// Space: O(1)...

class WordSearchST final : public virtual WordSearch {
public:
    explicit WordSearchST(){};
    virtual ~WordSearchST(){};
    WordSearchST(const WordSearchST&) = delete;
    const WordSearchST& operator=(const WordSearchST&) = delete;

    bool exist(std::vector<std::vector<char>> board, std::string word) const override;

private:
    bool DFSHelper(std::vector<std::vector<char>> &board, const std::string& word, const int rows, const int cols,
                   std::vector<std::pair<int, int>> &directions, int r, int c) const;
};

#endif //PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHST_H
