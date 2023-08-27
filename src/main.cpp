#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"
#include "AssetManager.h"

using am = AssetManager;

class OneLoneCoder_Jam : public olc::PixelGameEngine {
public:
    OneLoneCoder_Jam() {
        sAppName = "Jam";
    }

private:
    olc::sound::WaveEngine waveEngine;
    
public:
    bool OnUserCreate() override {
        waveEngine.InitialiseAudio();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLANK);

        return !GetKey(olc::ESCAPE).bPressed;
    }
};

int main()
{
    OneLoneCoder_Jam game;
    if(game.Construct(640, 400, 2, 2))
        game.Start();

    return 0;
}

