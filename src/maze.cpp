#include "maze.h"
#include <iostream>

Maze::Maze(int size, long seed) : size(size), gen(seed), distrib(0, size - 1), cells(size * size, true) {
    for(int i = 0; i < size * size; i++) {
        if (distrib(gen) % 2) {
            cells[i] = false;
        }
    }
}

bool Maze::getCell(int x, int y) {
    return cells[y * size + x];
}

void Maze::draw(olc::PixelGameEngine *pge, int cellSize) {
    for(int x = 0; x < size; x++) {
        for(int y = 0; y < size; y++) {
            olc::Pixel colour = getCell(x, y) ? olc::WHITE : olc::DARK_GREY;
            pge->FillRect(x * cellSize, y * cellSize, cellSize, cellSize, colour);
        }
    }
}
