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
        WIN
    };

    const int cellSize;
    const int mazeSize;

    const int minRadius;
    const int maxRadius;

    olc::Renderable circleMask{};

    int movesTaken{};
    int minMoves{};

    long long seed{};
    Maze *maze{};

    std::string httpResponse;
};

#endif //OLCJAM2023_GAMEGLOBALS_H
