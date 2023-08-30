#include "GameGlobals.h"


GameGlobals::GameGlobals(int mazeSize, int cellSize) : mazeSize(mazeSize), cellSize(cellSize), minRadius(cellSize * 4), maxRadius(cellSize * 16) {

}