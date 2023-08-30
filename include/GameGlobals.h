#ifndef OLCJAM2023_GAMEGLOBALS_H
#define OLCJAM2023_GAMEGLOBALS_H

#include "Maze.h"

class GameGlobals {
public:
    explicit GameGlobals(int mazeSize, int cellSize);
    enum class State {
        MENU,
        EXPLORE,
        ESCAPE,
        MANUAL,
        WIN
    };

    long long seed{};
    Maze *maze{};

    int cellSize;
    int mazeSize;

    int minRadius;
    int maxRadius;

    olc::Renderable circleMask{};
};

#endif //OLCJAM2023_GAMEGLOBALS_H
