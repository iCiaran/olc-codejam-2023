#ifndef OLCJAM2023_BASESTATE_H
#define OLCJAM2023_BASESTATE_H

#include "olcPixelGameEngine.h"
#include "GameGlobals.h"

class BaseState{
public:
    explicit BaseState(GameGlobals *gameGlobals);
    virtual ~BaseState() = default;

    virtual GameGlobals::State onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) = 0;
    virtual bool onEnter(olc::PixelGameEngine *pge) = 0;
    virtual bool onExit(olc::PixelGameEngine *pge) = 0;

protected:
    GameGlobals *globals;
};

#endif //OLCJAM2023_BASESTATE_H
