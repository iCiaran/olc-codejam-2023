#ifndef OLCJAM2023_ESCAPESTATE_H
#define OLCJAM2023_ESCAPESTATE_H

#include "BaseState.h"

class EscapeState : public BaseState {
public:
    explicit EscapeState(GameGlobals *gameGlobals);
    ~EscapeState() override;

    GameGlobals::State onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) override;
    bool onEnter(olc::PixelGameEngine *pge) override;
    bool onExit(olc::PixelGameEngine *pge) override;
private:
    olc::vi2d playerPosition = {1, 1};
    int moveCounter = 0;
};


#endif //OLCJAM2023_ESCAPESTATE_H
