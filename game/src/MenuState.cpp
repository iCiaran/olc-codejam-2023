#include <iostream>

#include "olcPixelGameEngine.h"
#include "MenuState.h"

MenuState::MenuState(GameGlobals *gameGlobals) : BaseState(gameGlobals) {
    seedLabel = new olc::QuickGUI::Label(mainMenu, "Choose seed:", {250, 400}, {80, 30});
    randomSeed = new olc::QuickGUI::CheckBox(mainMenu, "Random", true, {350, 400}, {80, 30});
    enterSeed = new olc::QuickGUI::TextBox(mainMenu, "", {450, 400}, {80, 30});

    nameLabel = new olc::QuickGUI::Label(mainMenu, "Enter name:", {250, 450}, {80, 30});
    enterName = new olc::QuickGUI::TextBox(mainMenu, globals->name, {350, 450}, {180, 30});

    playButton = new olc::QuickGUI::Button(mainMenu, "Play", {250, 500}, {280, 30});
    scoreButton = new olc::QuickGUI::Button(mainMenu, "Leaderboard", {250, 550}, {280, 30});
}
MenuState::~MenuState() = default;

GameGlobals::State MenuState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    mainMenu.Update(pge);

    if(playButton->bPressed) {
        return GameGlobals::State::EXPLORE;
    }

    if(scoreButton->bPressed) {
        return GameGlobals::State::SCORES;
    }

    enterSeed->sText.erase(std::remove_if(enterSeed->sText.begin(), enterSeed->sText.end(),
                           []( auto const& c ) -> bool { return !std::isdigit(c); } ), enterSeed->sText.end());
    enterSeed->sText = enterSeed->sText.substr(0, 2);
    enterSeed->bVisible = !randomSeed->bChecked;

    pge->Clear(olc::BLANK);

    const std::string titleString = "Memoraze";
    const olc::vi2d titleSize = pge->GetTextSize(titleString);
    pge->DrawStringDecal(((pge->GetScreenSize() - titleSize * 2) / 2) - olc::vf2d{0, 150}, titleString, olc::WHITE, { 2.0f, 2.0f });

    const std::string instructionString = "Escape the maze by reaching the bottom right corner";
    const olc::vi2d instructionSize = pge->GetTextSize(instructionString);
    pge->DrawStringDecal(((pge->GetScreenSize() - instructionSize) / 2) - olc::vf2d{0, 100}, instructionString);

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
        globals->seed = time(nullptr) % 100;
    } else {
        globals->seed = std::stoi(enterSeed->sText) % 100;
    }

    globals->name = enterName->sText;

    return true;
}

