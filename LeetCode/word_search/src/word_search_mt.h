//
// Created by Mo Aboulmagd on 1/18/21.
//

#ifndef PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHMT_H
#define PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHMT_H

// Complexity Analysis:
// Time: O(M*N) * O(M*N)
// Space: O(1)...

#include <vector>
#include <string>
#include <utility>
#include <unordered_set>

#include "word_search.h"

class WordSearchMT : public virtual WordSearch {
    public:
        explicit WordSearchMT(){};
        virtual ~WordSearchMT(){};
        WordSearchMT(const WordSearchMT&) = delete;
        const WordSearchMT& operator=(const WordSearchMT&) = delete;

        bool exist(std::vector<std::vector<char>> board, std::string word) const;

    private:
        void DFSHelper(std::vector<std::vector<char>> &board, const std::string& word, const size_t &rows, const size_t &cols,
                       std::vector<std::pair<int, int>> &directions, int r, int c, int currIndex, bool &found,
                       std::unordered_set<std::string> visited) const noexcept;
};

#endif //PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCHMT_H