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
    [[nodiscard]] bool inBounds(int x, int y) const;
    void draw(olc::PixelGameEngine *pge, int cellSize);
    void drawAroundPoint(olc::PixelGameEngine *pge, int cellSize, const olc::vi2d &, int radius);

private:
    std::mt19937 gen;
    std::uniform_int_distribution<> distrib;
    std::vector<bool> cells;
    std::map<olc::vi2d, int> cellGroup;

    void generate();
    void iterateKruskal();

    olc::vi2d getRandomInitialCell();
    olc::vi2d getRandomDirection();

    void setCell(int x, int y, bool open);
};

#endif //OLCJAM2023_MAZE_H
