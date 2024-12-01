#ifndef BOARD_H
#define BOARD_H

#include<vector>
#include "Tile.h"
using namespace std;


class Board {
private:
    vector<vector<Tile>> tiles;
    int numRows = 16;
    int numCols = 25;
    int numMines = 50;
    int flagCount;
    bool gameOver;
    bool victory;

public:
    Board();

    void initializeBoard(int rows, int cols, int mines);
    int getRow();
    int getCol();
    int getRemainingMines();
    bool revealTile(int row, int col);
    void toggleFlag(int row, int col);
    void calculateAdjacentMines();
    void reset();
    bool checkVictory();
    bool isGameOver();

    Tile& getTile(int row, int col);
};



#endif //BOARD_H
