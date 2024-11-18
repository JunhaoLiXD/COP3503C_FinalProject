#include"../include/Board.h"
#include<cstdlib>
#include <iostream>
#include<random>

Board::Board(const int rows, const int cols, const int mines):
numRows(rows), numCols(cols), numMines(mines), gameOver(false), victory(false) {initializeBoard();}

void Board::initializeBoard() {
    tiles = vector<vector<Tile>>(numRows, vector<Tile>(numCols));

    int minesPlaced = 0;
    while(minesPlaced < numMines) {
        int row = rand() % numRows;
        int col = rand() % numCols;

        if(!tiles[row][col].hasMine()) {
            tiles[row][col].setMine(true);
            minesPlaced++;
        }
    }

    calculateAdjacentMines();
}

int Board::getRow() {
    return numRows;
}

int Board::getCol() {
    return numCols;
}

Tile& Board::getTile(int row, int col) {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
        throw std::out_of_range("Invalid row or column index.");
    }
    return tiles[row][col];
}

void Board::calculateAdjacentMines() {
    for(int row = 0; row < numRows; row++) {
        for(int col = 0; col < numCols; col++) {
            if(tiles[row][col].hasMine()) continue;

            int count = 0;

            // From [row-1][col-1] to [row+1][col+1]
            for(int dr = -1; dr <= 1; dr++) {
                for(int dc = -1; dc <= 1; dc++) {
                    int r = row + dr, c = col + dc;
                    if(r >=0 && r < numRows && c >=0 && c < numCols && tiles[r][c].hasMine()) {
                        count++;
                    }
                }
            }
            tiles[row][col].setAdjacentMines(count);
        }
    }
}

bool Board::revealTile(int row, int col) {
    if(row < 0 || row >= numRows || col < 0 || col >= numCols || tiles[row][col].isTileRevealed()) {
        return false;
    }

    tiles[row][col].reveal();

    if(tiles[row][col].hasMine()) {
        gameOver = true;
        return true;
    }

    if(tiles[row][col].getAdjacentMines() == 0) {
        for(int dr = -1; dr <= 1; dr++) {
            for(int dc = -1; dc <= 1; dc++) {
                revealTile(row + dr, col + dc); // Recursion
            }
        }
    }

    return true;
}

void Board::toggleFlag(int row, int col) {
    if(row >= 0 && row < numRows && col >= 0 && col < numCols && !tiles[row][col].isTileRevealed()) {
        tiles[row][col].toggleFlag();
    }
}

bool Board::checkVictory() {
    // Check if any non-mine tile remains unrevealed; if so, the game is not yet won.
    for(int row = 0; row < numRows; row++) {
        for(int col = 0; col < numCols; col++) {
            if(!tiles[row][col].hasMine() && !tiles[row][col].isTileRevealed()) {
                return false;
            }
        }
    }

    victory = true;
    return true;
}

void Board::reset() {
    tiles.clear();

    initializeBoard();

    gameOver = false;
    victory = false;
}

void Board::printBoard(bool debugMode) const {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            const Tile& tile = tiles[row][col];

            if (debugMode && tile.hasMine()) {
                std::cout << "M ";
            }
            else if (!tile.isTileRevealed()) {
                if (tile.isTileFlagged()) {
                    std::cout << "F ";
                }
                else {
                    std::cout << ". ";
                }
            }
            else {
                if (tile.hasMine()) {
                    std::cout << "M ";
                }
                else if (tile.getAdjacentMines() == 0) {
                    std::cout << "  ";
                }
                else {
                    std::cout << tile.getAdjacentMines() << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}


// class Board {
// private:
//     vector<vector<Tile>> tiles;
//     int numRows;
//     int numCols;
//     int numMines;
//
//     bool gameOver;
//     bool victory;
//
// public:
//     Board(int rows, int cols, int mines);
//     void initializeBoard();
//     int getRow();
//     int getCol();
//     Tile& getTile(int row, int col);
//     bool revealTile(int row, int col);
//     void toggleFlag(int row, int col);
//     void calculateAdjacentMines();
//     void reset();
//     bool checkVictory();
//     void printBoard(bool debugMode) const;
// };