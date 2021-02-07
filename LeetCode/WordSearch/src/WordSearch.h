//
// Created by Mo Aboulmagd on 1/18/21.
//

#ifndef PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCH_H
#define PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCH_H

#include <vector>
#include <string>

class WordSearch {
public:
    explicit WordSearch();
    virtual ~WordSearch();
    WordSearch(const WordSearch&) = delete;
    const WordSearch& operator=(const WordSearch&) = delete;

    void SetStrategy(WordSearch* strategy);
    WordSearch* GetStrategy();
    virtual bool exist(std::vector<std::vector<char>> board, std::string word) const;

private:
    WordSearch* strategy_;
};


#endif //PROGRAMMINGCHALLENGES_LEETCODE_WORDSEARCH_H
