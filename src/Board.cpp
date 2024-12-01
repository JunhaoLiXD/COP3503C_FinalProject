#include"../include/Board.h"
#include<cstdlib>
#include <iostream>
#include<random>

Board::Board() : flagCount(0), gameOver(false), victory(false) {}

void Board::initializeBoard(int rows, int cols, int mines) {
    numRows = rows;
    numCols = cols;
    numMines = mines;
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
        Tile& tile = tiles[row][col];

        if(tile.isTileFlagged()) {
            tile.toggleFlag();
            flagCount--;
        }
        else {
            tile.toggleFlag();
            flagCount++;
        }
    }
}

int Board::getRemainingMines() {
    return numMines - flagCount;
}

bool Board::checkVictory() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            const Tile& tile = tiles[row][col];

            if (!tile.hasMine() && !tile.isTileRevealed()) {
                return false;
            }

            if (tile.hasMine() && !tile.isTileFlagged()) {
                return false;
            }
        }
    }

    victory = true;
    return true;
}

bool Board::isGameOver() {
    return gameOver;
}

void Board::reset() {
    tiles.clear();
    initializeBoard(numRows, numCols, numMines);
    flagCount = 0;
    gameOver = false;
    victory = false;
}
