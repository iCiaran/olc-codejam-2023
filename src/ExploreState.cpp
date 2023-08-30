#include "ExploreState.h"

int ExploreState::lerp(int start, int finish, double t) {
    return static_cast<int>(start + (finish - start) * t);
}

ExploreState::ExploreState(GameGlobals *gameGlobals) : BaseState(gameGlobals) {
    globals = gameGlobals;
}

ExploreState::~ExploreState() = default;

GameGlobals::State ExploreState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    timer += fElapsedTime;

    if (timer > maxTime) {
        return GameGlobals::State::ESCAPE;
    }

    pge->Clear(olc::BLANK);
    radius = lerp(globals->maxRadius, globals->minRadius, timer / maxTime);
    globals->maze->drawAroundPoint(pge, globals->cellSize, pge->GetMousePos(), radius);

    float scale = static_cast<float>(radius) / 128.0f;
    pge->DrawDecal({static_cast<float>(pge->GetMouseX() - radius) - 64 * scale, static_cast<float>(pge->GetMouseY() - radius) - 64 * scale}, globals->circleMask.Decal(), {scale, scale}, olc::BLACK);

    pge->DrawStringDecal({10,10}, "State: EXPLORE", olc::RED);
    pge->DrawStringDecal({160,10}, "Radius: " + std::to_string(radius), olc::RED);
    pge->DrawStringDecal({310,10}, "Time: " + std::to_string(timer), olc::RED);

    return GameGlobals::State::EXPLORE;
}

bool ExploreState::onEnter(olc::PixelGameEngine *pge) {
    std::cout << "Entering explore state" << std::endl;
    timer = 0.0f;
    delete globals->maze;
    globals->seed = time(nullptr);
    globals->maze = new Maze(globals->mazeSize, globals->seed);

    return true;
}

bool ExploreState::onExit(olc::PixelGameEngine *pge) {
    std::cout << "Exiting explore state" << std::endl;

    return true;
}

