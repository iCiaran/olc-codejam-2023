#include <algorithm>

#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"

#include "maze.h"

constexpr int MAZE_SIZE = 25;
constexpr int CELL_SIZE = 32;
//constexpr int TILE_SIZE = 64;

constexpr int MIN_RADIUS = CELL_SIZE * 4;
constexpr int MAX_RADIUS = CELL_SIZE * 16;
constexpr float EXPLORE_TIME = 20.0f;

int lerp(int start, int finish, double t) {
    return static_cast<int>(start + (finish - start) * t);
}

class OneLoneCoder_Jam : public olc::PixelGameEngine {
public:
    OneLoneCoder_Jam() {
        sAppName = "Jam";
    }

private:
    enum class State {
        MENU,
        EXPLORE,
        ESCAPE,
        MANUAL,
        WIN
    };

    olc::sound::WaveEngine waveEngine;
    Maze maze{MAZE_SIZE, time(nullptr)};

    std::unique_ptr<olc::Sprite> tileSheetFull;
    std::unique_ptr<olc::Sprite> tileSheetRegion;
    std::unique_ptr<olc::Decal> tileSheetDecal;

    olc::Renderable circleMask;

    State currentState = State::MENU;

    float exploreTime = 0.0f;

    olc::vi2d playerPosition = {1, 1};
    int moveCounter = 0;

    State menuState([[maybe_unused]] float fElapsedTime);
    State exploreState(float fElapsedTime);
    State manualState(float fElapsedTime);
    State escapeState(float fElapsedTime);
    State winState(float fElapsedTime);

public:
    int radius = CELL_SIZE * 4;

    bool OnUserCreate() override {
        waveEngine.InitialiseAudio();
//        tileSheetFull = std::make_unique<olc::Sprite>("assets/gfx/tilesheet_complete.png");
//        tileSheetDecal = std::make_unique<olc::Decal>(tileSheetFull->Duplicate({0, TILE_SIZE * 4}, {TILE_SIZE * 9, TILE_SIZE * 6}));

        circleMask.Load("assets/gfx/circle_mask.png");

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLANK);

        switch (currentState) {
            case State::MENU:
                currentState = menuState(fElapsedTime);
                break;
            case State::EXPLORE:
                currentState = exploreState(fElapsedTime);
                break;
            case State::MANUAL:
                currentState = manualState(fElapsedTime);
                break;
            case State::ESCAPE:
                currentState = escapeState(fElapsedTime);
                break;
            case State::WIN:
                currentState = winState(fElapsedTime);
                break;
        }

        return !GetKey(olc::ESCAPE).bPressed;
    }
};

OneLoneCoder_Jam::State OneLoneCoder_Jam::menuState([[maybe_unused]] float fElapsedTime) {
    DrawStringDecal({10,10}, "State: MENU", olc::RED);

    const std::string playString = "Press SPACE to start";
    const olc::vi2d textSize = GetTextSize(playString);

    DrawStringDecal((GetScreenSize() - textSize) / 2, playString);

    if(GetKey(olc::SPACE).bPressed) {
        return State::EXPLORE;
    }

    return State::MENU;
}

OneLoneCoder_Jam::State OneLoneCoder_Jam::exploreState(float fElapsedTime) {
    exploreTime += fElapsedTime;

    if (exploreTime > EXPLORE_TIME) {
        return State::ESCAPE;
    }

    radius = lerp(MAX_RADIUS, MIN_RADIUS, exploreTime / EXPLORE_TIME);

    maze.drawAroundPoint(this, CELL_SIZE, GetMousePos(), radius);

    float scale = static_cast<float>(radius) / 128.0f;
    DrawDecal({static_cast<float>(GetMouseX() - radius) - 64 * scale, static_cast<float>(GetMouseY() - radius) - 64 * scale}, circleMask.Decal(), {scale, scale}, olc::BLACK);

    DrawStringDecal({10,10}, "State: EXPLORE", olc::RED);
    DrawStringDecal({160,10}, "Radius: " + std::to_string(radius), olc::RED);
    DrawStringDecal({310,10}, "Time: " + std::to_string(exploreTime), olc::RED);

    return State::EXPLORE;
}

OneLoneCoder_Jam::State OneLoneCoder_Jam::escapeState([[maybe_unused]] float fElapsedTime) {
    olc::vi2d offset;
    bool move = false;

    if(GetKey(olc::Key::LEFT).bPressed) {
        offset = {-1, 0};
        move = true;
    } else if(GetKey(olc::Key::RIGHT).bPressed) {
        offset = {1, 0};
        move = true;
    } else if(GetKey(olc::Key::UP).bPressed) {
        offset = {0, -1};
        move = true;
    } else if(GetKey(olc::Key::DOWN).bPressed) {
        offset = {0, 1};
        move = true;
    }

    if (move) {
        olc::vi2d nextPosition = playerPosition + offset;

        if(maze.inBounds(nextPosition.x, nextPosition.y) && maze.getCell(nextPosition.x, nextPosition.y)) {
            playerPosition = nextPosition;
            moveCounter++;
        }

        if(playerPosition == olc::vi2d{MAZE_SIZE - 2, MAZE_SIZE - 2}) {
            return State::WIN;
        }
    }

    olc::vi2d screenPosition = playerPosition * CELL_SIZE + olc::vi2d{CELL_SIZE / 2, CELL_SIZE / 2};
    maze.drawAroundPoint(this, CELL_SIZE, screenPosition, MIN_RADIUS);

    float scale = static_cast<float>(radius) / 128.0f;
    DrawDecal({static_cast<float>(screenPosition.x - radius) - 64 * scale, static_cast<float>(screenPosition.y - radius) - 64 * scale}, circleMask.Decal(), {scale, scale}, olc::BLACK);

    FillCircle(screenPosition, CELL_SIZE / 4, olc::DARK_GREEN);

    DrawStringDecal({10,10}, "State: ESCAPE", olc::RED);
    DrawStringDecal({160,10}, "Moves: " + std::to_string(moveCounter), olc::RED);
    DrawStringDecal({310,10}, "Player: " + playerPosition.str(), olc::RED);

    return State::ESCAPE;
}

OneLoneCoder_Jam::State OneLoneCoder_Jam::manualState([[maybe_unused]] float fElapsedTime) {
    int mouseWheel = GetMouseWheel();
    if(mouseWheel > 0) {
        radius -= CELL_SIZE / 2;
        radius = std::max(CELL_SIZE * 4, radius);
    } else if (mouseWheel < 0) {
        radius += CELL_SIZE / 2;
        radius = std::min(CELL_SIZE * 16, radius);
    }

    maze.drawAroundPoint(this, CELL_SIZE, GetMousePos(), radius);

    float scale = static_cast<float>(radius) / 128.0f;
    DrawDecal({static_cast<float>(GetMouseX() - radius) - 64 * scale, static_cast<float>(GetMouseY() - radius) - 64 * scale}, circleMask.Decal(), {scale, scale}, olc::BLACK);

    DrawStringDecal({10,10}, "State: MANUAL", olc::RED);
    DrawStringDecal({160,10}, "Radius: " + std::to_string(radius), olc::RED);

    return OneLoneCoder_Jam::State::MANUAL;
}

OneLoneCoder_Jam::State OneLoneCoder_Jam::winState([[maybe_unused]] float fElapsedTime) {
    const std::string playString = "You win!";
    const olc::vi2d textSize = GetTextSize(playString);

    DrawStringDecal((GetScreenSize() - textSize) / 2, playString);

    DrawStringDecal({10,10}, "State: WIN", olc::RED);

    return OneLoneCoder_Jam::State::WIN;
}

int main() {
    OneLoneCoder_Jam game;
    if(game.Construct(MAZE_SIZE * CELL_SIZE, MAZE_SIZE * CELL_SIZE, 1, 1))
        game.Start();

    return 0;
}

