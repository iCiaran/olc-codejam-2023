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
    bool OnUserCreate() override {
        waveEngine.InitialiseAudio();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLANK);

        for(int x = 0; x < maze.size; x++) {
            for(int y = 0; y < maze.size; y++) {
                olc::Pixel colour = maze.getCell(x, y) ? olc::WHITE : olc::DARK_GREY;
                FillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, colour);
            }
        }

        return !GetKey(olc::ESCAPE).bPressed;
    }
};

int main() {
    OneLoneCoder_Jam game;
    if(game.Construct(MAZE_SIZE * CELL_SIZE, MAZE_SIZE * CELL_SIZE, 2, 2))
        game.Start();

    return 0;
}

