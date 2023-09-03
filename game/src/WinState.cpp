#include "WinState.h"

WinState::WinState(GameGlobals *gameGlobals) : BaseState(gameGlobals) { }

WinState::~WinState() = default;

GameGlobals::State WinState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    if(pge->GetKey(olc::SPACE).bPressed) {
        return GameGlobals::State::MENU;
    }

    pge->Clear(olc::BLANK);
    if(globals->debugText) {
        pge->DrawStringDecal({10, 10}, "State: WIN", olc::RED);
    }

    const std::string playString = "You escaped maze " + std::to_string(globals->seed) + " in " + std::to_string(globals->movesTaken) + " moves!";
    const olc::vi2d textSize = pge->GetTextSize(playString);
    pge->DrawStringDecal((pge->GetScreenSize() - textSize) / 2, playString);

#if defined(OLC_PLATFORM_EMSCRIPTEN)
    const std::string shortestString = "The shortest path was " + std::to_string(globals->minMoves) + " moves.";
    const olc::vi2d shortestTextSize = pge->GetTextSize(shortestString);
    pge->DrawStringDecal((pge->GetScreenSize() - shortestTextSize) / 2 + olc::vi2d{0, 50}, shortestString);

    if(globals->scoreSubmitted) {
        const std::string submitString = "Score submitted";
        const olc::vi2d submitTextSize = pge->GetTextSize(submitString);
        pge->DrawStringDecal((pge->GetScreenSize() - submitTextSize) / 2 + olc::vi2d{0, 100}, submitString);
    } else {
        const std::string submitString = "Submitting score";
        const olc::vi2d submitTextSize = pge->GetTextSize(submitString);
        pge->DrawStringDecal((pge->GetScreenSize() - submitTextSize) / 2 + olc::vi2d{0, 100}, submitString);
    }
#endif

    const std::string continueString = "Press space to continue";
    const olc::vi2d continueSize = pge->GetTextSize(continueString);
    pge->DrawStringDecal((pge->GetScreenSize() - continueSize) / 2 + olc::vi2d{0, 200}, continueString);

    return GameGlobals::State::WIN;
}

bool WinState::onEnter(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Entering win state" << std::endl;
    }

    return true;
}

bool WinState::onExit(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Exiting win state" << std::endl;
    }

    return true;
}
