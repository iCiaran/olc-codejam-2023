#include <iostream>

#include "olcPixelGameEngine.h"
#include "MenuState.h"

MenuState::MenuState(GameGlobals *gameGlobals) : BaseState(gameGlobals) {
    globals = gameGlobals;
}
MenuState::~MenuState() = default;

GameGlobals::State MenuState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    if(pge->GetKey(olc::SPACE).bPressed) {
        return GameGlobals::State::EXPLORE;
    }

    pge->Clear(olc::BLANK);
    pge->DrawStringDecal({10,10}, "State: MENU", olc::RED);

    const std::string playString = "Press SPACE to start";
    const olc::vi2d textSize = pge->GetTextSize(playString);
    pge->DrawStringDecal((pge->GetScreenSize() - textSize) / 2, playString);

    return GameGlobals::State::MENU;
}

bool MenuState::onEnter(olc::PixelGameEngine *pge) {
    std::cout << "Entering menu state" << std::endl;

    return true;
}

bool MenuState::onExit(olc::PixelGameEngine *pge) {
    std::cout << "Exiting menu state" << std::endl;

    return true;
}

