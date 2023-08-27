#ifndef OLCJAM2023_MAZE_H
#define OLCJAM2023_MAZE_H

#include <random>
#include <vector>

class Maze {
public:
    Maze(int size, long seed);
    const int size;

    bool getCell(int x, int y);

private:
    std::mt19937 gen;
    std::uniform_int_distribution<> distrib;
    std::vector<bool> cells;
};

#endif //OLCJAM2023_MAZE_H
