//
// Created by Mo Aboulmagd on 1/17/21.
//

#ifndef PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHST_H
#define PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHST_H

#include <vector>
#include <string>
#include <utility>

#include "WordSearch.h"

// Complexity Analysis:
// Time: O(M*N) * O(M*N)
// Space: O(1)...

class WordSearchST : public virtual WordSearch {
public:
    explicit WordSearchST();
    virtual ~WordSearchST();
    WordSearchST(const WordSearchST&) = delete;
    const WordSearchST& operator=(const WordSearchST&) = delete;

    bool exist(std::vector<std::vector<char>> board, std::string word) const override;

private:
    void DFSHelper(std::vector<std::vector<char>> &board, std::string word, const size_t &rows, const size_t &cols,
                   std::vector<std::pair<int, int>> &directions, size_t r, size_t c, int currIndex, bool &found) const;
};

#endif //PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHST_H
