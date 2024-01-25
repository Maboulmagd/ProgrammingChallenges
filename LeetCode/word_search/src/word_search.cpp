//
// Created by Mo Aboulmagd on 1/18/21.
//

#include "word_search.h"

WordSearch::WordSearch() {
    strategy_ = nullptr;
}

WordSearch::~WordSearch() {
    if (strategy_ != nullptr) {
        delete strategy_;
    }
}

void WordSearch::SetStrategy(WordSearch *strategy) {
    strategy_ = strategy;
}

WordSearch *WordSearch::GetStrategy() {
    return strategy_;
}

bool WordSearch::exist(std::vector<std::vector<char>> board, std::string word) const {
    return strategy_->exist(board, word);
}
