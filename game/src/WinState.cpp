#include "WinState.h"

WinState::WinState(GameGlobals *gameGlobals) : BaseState(gameGlobals) { }

WinState::~WinState() = default;

GameGlobals::State WinState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    if(pge->GetKey(olc::SPACE).bPressed) {
        return GameGlobals::State::MENU;
    }

    pge->Clear(olc::BLANK);
    pge->DrawStringDecal({10,10}, "State: WIN", olc::RED);

    const std::string playString = "You escaped in " + std::to_string(globals->movesTaken) + " moves!";
    const olc::vi2d textSize = pge->GetTextSize(playString);
    pge->DrawStringDecal((pge->GetScreenSize() - textSize) / 2, playString);

#if defined(OLC_PLATFORM_EMSCRIPTEN)
    const std::string shortestString = "The shortest path was " + std::to_string(globals->minMoves) + " moves.";
    const olc::vi2d shortestTextSize = pge->GetTextSize(shortestString);
    pge->DrawStringDecal((pge->GetScreenSize() - shortestTextSize) / 2 + olc::vi2d{0, 50}, shortestString);
#endif

    return GameGlobals::State::WIN;
}

bool WinState::onEnter(olc::PixelGameEngine *pge) {
    std::cout << "Entering win state" << std::endl;

    return true;
}

bool WinState::onExit(olc::PixelGameEngine *pge) {
    std::cout << "Exiting win state" << std::endl;

    return true;
}