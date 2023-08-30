#ifndef OLCJAM2023_EXPLORESTATE_H
#define OLCJAM2023_EXPLORESTATE_H

#include "BaseState.h"

class ExploreState : public BaseState {
public:
    explicit ExploreState(GameGlobals *gameGlobals);
    ~ExploreState() override;

    GameGlobals::State onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) override;
    bool onEnter(olc::PixelGameEngine *pge) override;
    bool onExit(olc::PixelGameEngine *pge) override;

private:
    float timer{};
    float maxTime = 20.0f;
    int radius{};

    static int lerp(int start, int finish, double t);
};

#endif //OLCJAM2023_EXPLORESTATE_H
