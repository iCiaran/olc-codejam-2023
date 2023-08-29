#include <algorithm>

#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"

#include "maze.h"

constexpr int MAZE_SIZE = 25;
constexpr int CELL_SIZE = 32;
constexpr int TILE_SIZE = 64;

class OneLoneCoder_Jam : public olc::PixelGameEngine {
public:
    OneLoneCoder_Jam() {
        sAppName = "Jam";
    }

private:
    olc::sound::WaveEngine waveEngine;
    Maze maze{MAZE_SIZE, time(nullptr)};

    std::unique_ptr<olc::Sprite> tileSheetFull;
    std::unique_ptr<olc::Sprite> tileSheetRegion;
    std::unique_ptr<olc::Decal> tileSheetDecal;

    olc::Renderable circleMask;
    
public:
    int radius = CELL_SIZE * 4;

    bool OnUserCreate() override {
        waveEngine.InitialiseAudio();
        tileSheetFull = std::make_unique<olc::Sprite>("assets/gfx/tilesheet_complete.png");
        tileSheetDecal = std::make_unique<olc::Decal>(tileSheetFull->Duplicate({0, TILE_SIZE * 4}, {TILE_SIZE * 9, TILE_SIZE * 6}));

        circleMask.Load("assets/gfx/circle_mask.png");

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLANK);

        int mouseWheel = GetMouseWheel();
        if(mouseWheel > 0) {
            radius -= CELL_SIZE / 2;
            radius = std::max(CELL_SIZE * 4, radius);
        } else if (mouseWheel < 0) {
            radius += CELL_SIZE / 2;
            radius = std::min(CELL_SIZE * 16, radius);
        }

        maze.drawAroundPoint(this, CELL_SIZE, GetMousePos(), radius);

//        DrawPartialDecal({0, 0}, {CELL_SIZE, CELL_SIZE}, tileSheetDecal.get(), {0, 0}, {TILE_SIZE, TILE_SIZE});

        float scale = static_cast<float>(radius) / 128.0f;
        DrawDecal({static_cast<float>(GetMouseX() - radius) - 64 * scale, static_cast<float>(GetMouseY() - radius) - 64 * scale}, circleMask.Decal(), {scale, scale}, olc::BLACK);

        DrawStringDecal({10,10}, std::to_string(radius), olc::RED);

        return !GetKey(olc::ESCAPE).bPressed;
    }
};

int main() {
    OneLoneCoder_Jam game;
    if(game.Construct(MAZE_SIZE * CELL_SIZE, MAZE_SIZE * CELL_SIZE, 1, 1))
        game.Start();

    return 0;
}

