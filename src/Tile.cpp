#include "../include/Tile.h"

Tile::Tile() : isMine(false), isRevealed(false), isFlagged(false), adjacentMines(0){}

void Tile::setMine(bool value) {
    isMine = value;
}

void Tile::setAdjacentMines(const int count) {
    adjacentMines = count;
}

bool Tile::hasMine() const {
    return isMine;
}

int Tile::getAdjacentMines() const {
    return adjacentMines;
}

bool Tile::isTileRevealed() const {
    return isRevealed;
}

bool  Tile::isTileFlagged() const {
    return isFlagged;
}

void Tile::reveal() {
    isRevealed = true;
}

void Tile::toggleFlag() {
    isFlagged = !isFlagged;
}
