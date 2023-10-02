//
// Created by Mo Aboulmagd on 1/17/21.
//

#include "WordSearchST.h"

#include <unordered_set>
#include <stack>

bool WordSearchST::exist(std::vector<std::vector<char>> board, std::string word) const {
    const int rows = static_cast<int>(board.size());
    const int cols = static_cast<int>(board[0].size());

    bool found = false;

    std::vector<std::pair<int, int>> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    for (int r = 0; !found && r < rows; ++r) {
        for (int c = 0; !found && c < cols; ++c) {
            // First character matches, then commence broader search...
            if (board[r][c] == word[0]) {
                if (DFSHelper(board, word, rows, cols, directions, r, c)) {
                    found = true;
                    break;
                }
            }
        }
    }

    return found;
}

bool WordSearchST::DFSHelper(std::vector<std::vector<char>>& board, const std::string& word, const int rows, const int cols,
                             std::vector<std::pair<int, int>>& directions, int r, int c) const {
    //std::unordered_set<Cell> visited;
    std::stack<Cell> st;

    const Cell start = Cell(r, c, 1);
    st.push(start);

    bool found = false;
    const int wordLen = static_cast<int>(word.length());

    while (!st.empty()) {
        const Cell cell = st.top();
        st.pop();

        if (cell.currIndex_ == wordLen - 1) {
            found = true;
            break;
        }

        // Mark cell as visited.
        //visited.insert(cell);

        for (const auto& direction : directions) {
            const int newR = r + direction.first;
            const int newC = c + direction.second;

            const Cell newCell = Cell(newR, newC, cell.currIndex_ + 1);
//            if ( (newR >= 0 && newR < rows) && (newC >= 0 && newC < cols) && (board[newR][newC] == word[cell.currIndex_]) &&
//                 (visited.find(newCell) == visited.end()) ) {
//                st.push(newCell);
//            }
        }
    }

    // Clear stack and set
    while (!st.empty()) { st.pop(); }
    //visited.clear();

    return found;
}
