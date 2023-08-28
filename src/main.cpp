#include <algorithm>

#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"

#if defined(OLC_PLATFORM_EMSCRIPTEN)
#include <emscripten/fetch.h>
#endif

#include "maze.h"

constexpr int MAZE_SIZE = 41;
constexpr int CELL_SIZE = 10;

class OneLoneCoder_Jam : public olc::PixelGameEngine {
public:
    OneLoneCoder_Jam() {
        sAppName = "Jam";
    }

private:
    olc::sound::WaveEngine waveEngine;
    Maze maze{MAZE_SIZE, time(nullptr)};
    
public:
    int radius = CELL_SIZE * 5;

#if defined(OLC_PLATFORM_EMSCRIPTEN)
    static void downloadSucceeded(emscripten_fetch_t *fetch) {
        printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
        // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
        emscripten_fetch_close(fetch); // Free data associated with the fetch.
    }

    static void downloadFailed(emscripten_fetch_t *fetch) {
        printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
        emscripten_fetch_close(fetch); // Also free data on failure.
    }
#endif

    bool OnUserCreate() override {
        waveEngine.InitialiseAudio();
#if defined(OLC_PLATFORM_EMSCRIPTEN)
        emscripten_fetch_attr_t attr;
        emscripten_fetch_attr_init(&attr);
        strcpy(attr.requestMethod, "GET");
        attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
        attr.onsuccess = downloadSucceeded;
        attr.onerror = downloadFailed;
        emscripten_fetch(&attr, "http://localhost:8000/");
#endif
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLANK);

        int mouseWheel = GetMouseWheel();
        if(mouseWheel > 0) {
            radius -= CELL_SIZE / 2;
            radius = std::max(0, radius);
        } else if (mouseWheel < 0) {
            radius += CELL_SIZE / 2;
        }

        maze.drawAroundPoint(this, CELL_SIZE, GetMousePos(), radius);

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

