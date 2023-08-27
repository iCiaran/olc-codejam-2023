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

