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
        WIN,
        SCORES
    };

    const int cellSize;
    const int mazeSize;

    const int minRadius;
    const int maxRadius;

    olc::Renderable circleMask{};

    bool debugText = false;

    int movesTaken{};
    int minMoves{};

    long long seed{};
    Maze *maze{};

    std::string name = "LoneCoder";
    bool scoreSubmitted;

    std::string apiUrl = "https://olc.iciaran.com/api/";
//    std::string apiUrl = "http://localhost:5000/api/";

};

#endif //OLCJAM2023_GAMEGLOBALS_H
