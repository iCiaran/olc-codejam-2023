#ifndef OLCJAM2023_MENUSTATE_H
#define OLCJAM2023_MENUSTATE_H

#include "BaseState.h"

class MenuState : public BaseState {
public:
    explicit MenuState(GameGlobals *gameGlobals);
    ~MenuState() override;

    GameGlobals::State onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) override;
    bool onEnter(olc::PixelGameEngine *pge) override;
    bool onExit(olc::PixelGameEngine *pge) override;
};

#endif //OLCJAM2023_MENUSTATE_H
