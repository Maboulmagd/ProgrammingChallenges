//
// Created by Mo Aboulmagd on 1/17/21.
//

#include "WordSearchST.h"

bool WordSearchST::exist(std::vector<std::vector<char>> board, std::string word) const {
    const size_t rows = board.size();
    const size_t cols = board[0].size();

    bool found = false;

    std::vector<std::pair<int, int>> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    for (int r = 0; !found && r < rows; ++r)
    {
        for (int c = 0; !found && c < cols; ++c)
        {
            // First character matches, then commence broader search...
            if (board[r][c] == word[0])
            {
                DFSHelper(board, word, rows, cols, directions, r, c, 0, found);
            }
        }
    }

    return found;
}

void WordSearchST::DFSHelper(std::vector<std::vector<char>>& board, std::string word, const size_t& rows, const size_t& cols,
                             std::vector<std::pair<int, int>>& directions, size_t r, size_t c, int currIndex,
                             bool &found) const {
    if (board[r][c] != word[currIndex] || board[r][c] == '~') { return; }

    else if (currIndex == static_cast<int>(word.length()) - 1)
    {
        found = true;
        return;
    }

    // Save current letter, prior to overwriting it...
    const char letterHere = board[r][c];

    // Mark as visited, such that we do NOT use this letter again (using more than once is not allowed)...
    board[r][c] = '~';

    for (const auto& direction : directions)
    {
        if (found) { return; }

        int newR = r + direction.first;
        int newC = c + direction.second;

        if ( (newR >= 0 && newR < rows) && (newC >= 0 && newC < cols) )
        {
            DFSHelper(board, word, rows, cols, directions, newR, newC, currIndex + 1, found);
        }
    }

    // Backtracking...
    board[r][c] = letterHere;
}

WordSearchST::WordSearchST() {

}

WordSearchST::~WordSearchST() {

}
