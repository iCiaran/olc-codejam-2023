#include "maze.h"

Maze::Maze(int size, long seed) : size(size), gen(seed), distrib(0, size - 3), cells(size * size, false) {
    generate();
}

bool Maze::getCell(int x, int y) {
    return cells[y * size + x];
}

void Maze::setCell(int x, int y, bool open) {
    cells[y * size + x] = open;
}

bool Maze::inBounds(int x, int y) const {
    return x > 0 && x < size - 1 && y > 0 && y < size - 1;
}

void Maze::draw(olc::PixelGameEngine *pge, int cellSize) {
    for(int x = 0; x < size; x++) {
        for(int y = 0; y < size; y++) {
            olc::Pixel colour = getCell(x, y) ? olc::DARK_GREY : olc::BLACK;
            pge->FillRect(x * cellSize, y * cellSize, cellSize, cellSize, colour);
        }
    }
}

void Maze::drawAroundPoint(olc::PixelGameEngine *pge, int cellSize, const olc::vi2d & point, int radius) {
    int halfCellSize = cellSize / 2;
    int xCoord = cellSize * (point.x / cellSize) + halfCellSize;
    int yCoord = cellSize * (point.y / cellSize) + halfCellSize;

    for(int x = 0; x < size; x++) {
        for(int y = 0; y < size; y++) {
            int xSquared = (x * cellSize + halfCellSize - xCoord) * (x * cellSize + halfCellSize - xCoord);
            int ySquared = (y * cellSize + halfCellSize - yCoord) * (y * cellSize + halfCellSize - yCoord);

            if(xSquared + ySquared < (radius + halfCellSize) * (radius + halfCellSize)) {
                olc::Pixel colour = getCell(x, y) ? olc::WHITE : olc::BLACK;
                pge->FillRect(x * cellSize, y * cellSize, cellSize, cellSize, colour);
            }
        }
    }

    pge->Draw(xCoord, yCoord, olc::RED);
    pge->DrawCircle(xCoord, yCoord, radius, olc::BLUE);
}

void Maze::generate() {
    int group = 0;
    for(int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if(x % 2 && y % 2) {
                setCell(x, y, true);
                cellGroup[{x, y}] = group;
                group++;
            }
        }
    }

    int iterations = (size - 3) * (size + 1) / 4;
    for (int i = 0; i < iterations ; i++) {
        iterateKruskal();
    }

    cellGroup.clear();
}

olc::vi2d Maze::getRandomInitialCell() {
    int x = distrib(gen) / 2 * 2 + 1;
    int y = distrib(gen) / 2 * 2 + 1;
    return {x, y};
}

olc::vi2d Maze::getRandomDirection() {
    int r = distrib(gen) % 4;
    switch(r){
        case 0: return {1, 0};
        case 1: return {0, 1};
        case 2: return {-1, 0};
        case 3: return {0, -1};
        default: return {0, 0};
    }
}

void Maze::iterateKruskal() {
    while(true){
        olc::vi2d initialCell = getRandomInitialCell();
        olc::vi2d direction = getRandomDirection();

        olc::vi2d wall = initialCell + direction;
        if (getCell(wall.x, wall.y)) {
            continue;
        }

        olc::vi2d nextCell = wall + direction;
        if (!inBounds(nextCell.x, nextCell.y)) {
            continue;
        }

        if(cellGroup[initialCell] == cellGroup[nextCell]) {
            continue;
        }

        setCell(wall.x, wall.y, true);

        int initialGroup = cellGroup[initialCell];
        int nextGroup = cellGroup[nextCell];
        for(auto &[key, value]: cellGroup) {
            if(value == initialGroup){
                cellGroup[key] = nextGroup;
            }
        }

        break;
    }
}
