#include <iostream>

#include "olcPixelGameEngine.h"
#include "MenuState.h"

MenuState::MenuState(GameGlobals *gameGlobals) : BaseState(gameGlobals) {
    playButton = new olc::QuickGUI::Button(mainMenu,  "Play", {30, 30}, {160, 30});
    randomSeed = new olc::QuickGUI::CheckBox(mainMenu, "Random", true, {30, 70}, {75, 30});
    enterSeed = new olc::QuickGUI::TextBox(mainMenu, "", {110, 70}, {75, 30});
    enterName = new olc::QuickGUI::TextBox(mainMenu, globals->name, {30, 110}, {160, 30});
}
MenuState::~MenuState() = default;

GameGlobals::State MenuState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    mainMenu.Update(pge);

    if(playButton->bPressed) {
        return GameGlobals::State::EXPLORE;
    }

    enterSeed->sText.erase(std::remove_if(enterSeed->sText.begin(), enterSeed->sText.end(),
                           []( auto const& c ) -> bool { return !std::isdigit(c); } ), enterSeed->sText.end());
    enterSeed->sText = enterSeed->sText.substr(0, 4);
    enterSeed->bVisible = !randomSeed->bChecked;

    pge->Clear(olc::BLANK);

    if(globals->debugText) {
        pge->DrawStringDecal({10, 10}, "State: MENU", olc::RED);
    }
    mainMenu.Draw(pge);

    return GameGlobals::State::MENU;
}

bool MenuState::onEnter(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Entering menu state" << std::endl;
    }

    mainMenu.Update(pge);

    return true;
}

bool MenuState::onExit(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Exiting menu state" << std::endl;
    }

    if(randomSeed->bChecked || enterSeed->sText.empty()) {
        globals->seed = time(nullptr) % 1000;
    } else {
        globals->seed = std::stoi(enterSeed->sText) % 1000;
    }

    globals->name = enterName->sText;

    return true;
}

