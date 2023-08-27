#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"

#include "maze.h"

constexpr int MAZE_SIZE = 40;
constexpr int CELL_SIZE = 10;

class OneLoneCoder_Jam : public olc::PixelGameEngine {
public:
    OneLoneCoder_Jam() {
        sAppName = "Jam";
    }

private:
    olc::sound::WaveEngine waveEngine;
    Maze maze{MAZE_SIZE, 1};
    
public:
    int radius = 60;

    bool OnUserCreate() override {
        waveEngine.InitialiseAudio();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLANK);

        int mouseWheel = GetMouseWheel();
        if(mouseWheel > 0) {
            radius -= 10;
        } else if (mouseWheel < 0) {
            radius += 10;
        }

        maze.drawAroundPoint(this, CELL_SIZE, {GetMouseX(), GetMouseY()}, radius);

        DrawString({10,10}, std::to_string(radius), olc::RED);

        return !GetKey(olc::ESCAPE).bPressed;
    }
};

int main() {
    OneLoneCoder_Jam game;
    if(game.Construct(MAZE_SIZE * CELL_SIZE, MAZE_SIZE * CELL_SIZE, 2, 2))
        game.Start();

    return 0;
}

