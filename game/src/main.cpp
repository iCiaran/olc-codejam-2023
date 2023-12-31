#include <unordered_map>
#define OLC_PGEX_QUICKGUI
#define OLC_PGEX_SPLASHSCREEN

#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"

#include "GameGlobals.h"
#include "BaseState.h"
#include "MenuState.h"
#include "ExploreState.h"
#include "EscapeState.h"
#include "WinState.h"
#include "ScoreState.h"

constexpr int MAZE_SIZE = 25;
constexpr int CELL_SIZE = 32;

class OneLoneCoder_Jam : public olc::PixelGameEngine {
public:
    OneLoneCoder_Jam() {
        sAppName = "Jam";
    }

    olc::SplashScreen splash;
private:

    std::unique_ptr<olc::Sprite> tileSheetFull;
    std::unique_ptr<olc::Sprite> tileSheetRegion;
    std::unique_ptr<olc::Decal> tileSheetDecal;

    std::unordered_map<GameGlobals::State, BaseState*> stateMap;
    GameGlobals::State currentState = GameGlobals::State::MENU;
    GameGlobals::State nextState = GameGlobals::State::MENU;

    GameGlobals globals = GameGlobals(MAZE_SIZE, CELL_SIZE);

public:
    bool OnUserCreate() override {
        globals.circleMask.Load("assets/gfx/circle_mask.png");

        stateMap[GameGlobals::State::MENU] = new MenuState(&globals);
        stateMap[GameGlobals::State::EXPLORE] = new ExploreState(&globals);
        stateMap[GameGlobals::State::ESCAPE] = new EscapeState(&globals);
        stateMap[GameGlobals::State::WIN] = new WinState(&globals);
        stateMap[GameGlobals::State::SCORES] = new ScoreState(&globals);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if(nextState != currentState) {
            stateMap[currentState]->onExit(this);
            stateMap[nextState]->onEnter(this);
            currentState = nextState;
        }

        nextState = stateMap[currentState]->onUpdate(this, fElapsedTime);

        return true;
    }
};

int main() {
    OneLoneCoder_Jam game;
    if(game.Construct(MAZE_SIZE * CELL_SIZE, MAZE_SIZE * CELL_SIZE, 1, 1))
        game.Start();

    return 0;
}

