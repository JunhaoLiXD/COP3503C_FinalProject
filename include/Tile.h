#ifndef TILE_H
#define TILE_H

#include<vector>
using namespace std;


class Tile {
private:
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
    vector<Tile*> neighbors;

public:
    Tile();

    void setMine(bool value);
    void setAdjacentMines(int count);
    bool hasMine() const;
    int getAdjacentMines() const;
    bool isTileRevealed() const;
    bool isTileFlagged() const;

    void reveal();
    void toggleFlag();
};



#endif //TILE_H
