#ifndef OLCJAM2023_WINSTATE_H
#define OLCJAM2023_WINSTATE_H

#include "BaseState.h"

class WinState : public BaseState {
public:
    explicit WinState(GameGlobals *gameGlobals);
    ~WinState() override;

    GameGlobals::State onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) override;
    bool onEnter(olc::PixelGameEngine *pge) override;
    bool onExit(olc::PixelGameEngine *pge) override;
};

#endif //OLCJAM2023_WINSTATE_H
