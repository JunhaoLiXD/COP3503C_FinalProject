#ifndef BOARD_H
#define BOARD_H

#include<vector>
#include "Tile.h"
using namespace std;


class Board {
private:
    vector<vector<Tile>> tiles;
    int numRows;
    int numCols;
    int numMines;
    int flagCount;
    bool gameOver;
    bool victory;

public:
    Board(int rows, int cols, int mines);
    void initializeBoard();
    int getRow();
    int getCol();
    int getRemainingMines();
    bool revealTile(int row, int col);
    void toggleFlag(int row, int col);
    void calculateAdjacentMines();
    void reset();
    bool checkVictory();
    bool isGameOver();
    void printBoard(bool debugMode) const;

    Tile& getTile(int row, int col);
};



#endif //BOARD_H
