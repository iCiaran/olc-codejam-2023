#ifndef OLCJAM2023_MAZE_H
#define OLCJAM2023_MAZE_H

#include <random>
#include <vector>
#include "olcPixelGameEngine.h"

class Maze {
public:
    Maze(int size, long seed);
    const int size;

    bool getCell(int x, int y);
    void draw(olc::PixelGameEngine *pge, int cellSize);
    void drawAroundPoint(olc::PixelGameEngine *pge, int cellSize, const olc::vi2d &, int radius);

private:
    std::mt19937 gen;
    std::uniform_int_distribution<> distrib;
    std::vector<bool> cells;
};

#endif //OLCJAM2023_MAZE_H
