//
// Created by Mo Aboulmagd on 1/18/21.
//

#include "WordSearchMT.h"
#include <thread>

WordSearchMT::WordSearchMT() {

}

WordSearchMT::~WordSearchMT() {

}

// TODO: Try using an atomic_bool.
//       Also need to try defining std::hash that works with std::pair.
//       Remember to test performance differences!
bool WordSearchMT::exist(std::vector<std::vector<char>> board, std::string word) const {
    const size_t rows = board.size();
    const size_t cols = board[0].size();

    bool found = false;

    std::vector<std::pair<int, int>> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    std::vector<std::thread> workers;

    std::unordered_set<std::string> visited;

    for (int r = 0; !found && r < rows; ++r)
    {
        for (int c = 0; !found && c < cols; ++c)
        {
            // First character matches, then commence broader search...
            if (board[r][c] == word[0])
            {
                // Parallelize...
                workers.push_back(std::thread([this, &board, &word, &rows, &cols, &directions, r, c, &found, visited]()
                {
                    DFSHelper(board, word, rows, cols, directions, r, c, 0, found, visited);
                }));
            }
        }
    }

    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    return found;
}

void WordSearchMT::DFSHelper(std::vector<std::vector<char>>& board, const std::string& word, const size_t& rows, const size_t& cols,
                        std::vector<std::pair<int, int>>& directions, int r, int c, int currIndex,
                        bool& found, std::unordered_set<std::string> visited) const noexcept {
    if (board[r][c] != word[currIndex] || visited.find(std::to_string(r) + "-" + std::to_string(c)) != visited.end() || found) { return; }

    else if (currIndex == static_cast<int>(word.length()) - 1)
    {
        found = true;
        return;
    }

    // Form unique string to mark current cell as visited...
    visited.insert(std::to_string(r) + "-" + std::to_string(c));

    for (const auto& direction : directions)
    {
        if (found) { return; }

        int newR = r + direction.first;
        int newC = c + direction.second;

        if ( (newR >= 0 && newR < rows) && (newC >= 0 && newC < cols) )
        {
            DFSHelper(board, word, rows, cols, directions, newR, newC, currIndex + 1, found, visited);
        }
    }

    // Backtracking...
    visited.erase(std::to_string(r) + "-" + std::to_string(c));
}
