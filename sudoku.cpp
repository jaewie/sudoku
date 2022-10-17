#include "sudoku.h"
#include <stdio.h>
#include <iostream>
#include <string>


const int ROW_COUNT = 9;
const int COL_COUNT = 9;

const int SOLVED = 1;
const int UNSOLVED = -1;
const int INVALID = 0;

using namespace std;

class Board {
    public:
    vector<vector<Node>> board;

    Board() {
        board = vector(ROW_COUNT, vector<Node> (COL_COUNT, Node()));
    }

    bool solve() {
        int result = solved();
        if (result == SOLVED) {
            return true;
        } else if (result == INVALID) {
            return false;
        }
        auto [row, col] = pick_node();
        auto node = board[row][col];
        vector<vector<Node>> copy = board;
        for (auto &v: node.possible) {
            pick_and_propagate(row, col, v);
            if (solve()) {
                return true;
            }
            board = copy;
        }

        return false;
    }

    pair<int, int> pick_node() {
        pair<int, int> res(-1, -1);
        int least_possible_length = INT_MAX;

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                int possible_length = board[i][j].possible.size();
                if (possible_length > 1 && possible_length < least_possible_length) {
                    res = pair(i, j);
                    least_possible_length = possible_length;
                }
            }
        }
        return res;
    }

    int solved() {
        int result = SOLVED;
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                int possible_length = board[i][j].possible.size();
                if (possible_length == 0) {
                    return INVALID;
                } else if (possible_length != 1) {
                    result = UNSOLVED;
                }
            }
        }
        return result;
    }

    void pick_and_propagate(const int row, const int col, const int val) {
        vector<pair<int, int>> props;
        for (int r = 0; r < board.size(); r++) {
            if (r != row) {
                bool new_prop = board[r][col].possible.contains(val) && board[r][col].possible.size() == 2;
                board[r][col].possible.erase(val);
                if (new_prop) {
                    props.push_back(pair(r, col));
                }
            }
        }

        for (int c = 0; c < board[row].size(); c++) {
            if (c != col) {
                bool new_prop = board[row][c].possible.contains(val) && board[row][c].possible.size() == 2;
                board[row][c].possible.erase(val);
                if (new_prop) {
                    props.push_back(pair(row, c));
                }
            }
        }

        board[row][col].possible = {val};
        for (auto &[r, c] : props) {
            if (board[r][c].possible.size() == 0) {
                break;
            }
            pick_and_propagate(r, c, *board[r][c].possible.begin());
        }
    }

    void print_board() {
        for (int i = 0; i < board.size(); i++) {
            vector<int> vals;
            for (int j = 0; j < board[i].size(); j++) {
                if (board[i][j].possible.size() == 1) {
                    vals.push_back(*board[i][j].possible.begin());
                } else if (board[i][j].possible.size() == 0) {
                    vals.push_back(-1);
                } else {
                    vals.push_back(0);
                }
            }

            for (auto &v: vals) {
                std::cout << v << " ";
            }

            std::cout << endl;
        }
    }
};

Board make_board(vector<vector<int>> board) {
    Board res;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] > 0) {
                res.pick_and_propagate(i, j, board[i][j]);
            }
        }
    }
    return res;
}

int main() {
    vector<vector<int>> matrix = {
        {0, 5, 0, 0, 7, 8, 0, 0, 1},
        {2, 1, 0, 4, 0, 9, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 3, 0},
        {8, 0, 9, 0, 0, 0, 0, 2, 0},
        {1, 0, 5, 2, 8, 6, 4, 0, 9},
        {6, 0, 7, 5, 0, 4, 1, 8, 3},
        {0, 8, 0, 7, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 4, 2, 3, 5, 0},
        {5, 0, 0, 0, 0, 0, 7, 0, 4}
    };

    Board b = make_board(matrix);
    b.solve();
    b.print_board();

    return 0;
}