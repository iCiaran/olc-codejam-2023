#ifndef OLCJAM2023_MENUSTATE_H
#define OLCJAM2023_MENUSTATE_H

#include "BaseState.h"
#include "olcPGEX_QuickGUI.h"

class MenuState : public BaseState {
public:
    explicit MenuState(GameGlobals *gameGlobals);
    ~MenuState() override;

    olc::QuickGUI::Manager mainMenu;
    olc::QuickGUI::Button *playButton;
    olc::QuickGUI::CheckBox *randomSeed;
    olc::QuickGUI::TextBox *enterSeed;
    olc::QuickGUI::TextBox *enterName;
    olc::QuickGUI::Button *scoreButton;

    GameGlobals::State onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) override;
    bool onEnter(olc::PixelGameEngine *pge) override;
    bool onExit(olc::PixelGameEngine *pge) override;
};

#endif //OLCJAM2023_MENUSTATE_H
