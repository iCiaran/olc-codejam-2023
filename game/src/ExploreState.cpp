#include "ExploreState.h"

#if defined(OLC_PLATFORM_EMSCRIPTEN)
#include <emscripten/fetch.h>
#include "json.h"
using json = nlohmann::json;

void onGetMazeError(struct emscripten_fetch_t *fetch) {
    emscripten_fetch_close(fetch);
}

void onGetMazeSuccess(struct emscripten_fetch_t *fetch) {

    auto state = static_cast<ExploreState*>(fetch->userData);
    std::string responseBody(fetch->data, fetch->numBytes);
    json response = json::parse(responseBody);

    std::vector cells = response["cells"].get<std::vector<int>>();
    int minMoves = response["shortest"].get<int>();

    state->globals->maze = new Maze(25, cells);
    state->globals->minMoves = minMoves;
    state->mazeLoaded = true;

    emscripten_fetch_close(fetch);
}
#endif

ExploreState::ExploreState(GameGlobals *gameGlobals) : BaseState(gameGlobals) { }

ExploreState::~ExploreState() = default;

int ExploreState::lerp(int start, int finish, double t) {
    return static_cast<int>(start + (finish - start) * t);
}

GameGlobals::State ExploreState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    if(!mazeLoaded) {
        pge->Clear(olc::BLANK);
        const std::string playString = "Loading maze";
        const olc::vi2d textSize = pge->GetTextSize(playString);
        pge->DrawStringDecal((pge->GetScreenSize() - textSize) / 2, playString);
        return GameGlobals::State::EXPLORE;
    }
    timer += fElapsedTime;

    if (timer > maxTime) {
        return GameGlobals::State::ESCAPE;
    }

    pge->Clear(olc::BLANK);
    radius = lerp(globals->maxRadius, globals->minRadius, timer / maxTime);
    globals->maze->drawAroundPoint(pge, globals->cellSize, pge->GetMousePos(), radius);

    float scale = static_cast<float>(radius) / 128.0f;
    pge->DrawDecal({static_cast<float>(pge->GetMouseX() - radius) - 64 * scale, static_cast<float>(pge->GetMouseY() - radius) - 64 * scale}, globals->circleMask.Decal(), {scale, scale}, olc::BLACK);

    if(globals->debugText) {
        pge->DrawStringDecal({10, 10}, "State: EXPLORE", olc::RED);
        pge->DrawStringDecal({160, 10}, "Radius: " + std::to_string(radius), olc::RED);
        pge->DrawStringDecal({310, 10}, "Time: " + std::to_string(timer), olc::RED);
        pge->DrawStringDecal({460, 10}, "Seed: " + std::to_string(globals->seed), olc::RED);
    }
    return GameGlobals::State::EXPLORE;
}

bool ExploreState::onEnter(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Entering explore state" << std::endl;
    }
    timer = 0.0f;

    delete globals->maze;
#if defined(OLC_PLATFORM_EMSCRIPTEN)
    std::string url = globals -> apiUrl + "mazes/" + std::to_string(globals->seed);
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onGetMazeSuccess;
    attr.onerror = onGetMazeError;
    attr.userData = this;
    emscripten_fetch(&attr, url.c_str());
#else
    globals->maze = new Maze(globals->mazeSize, globals->seed);
    mazeLoaded = true;
#endif

    return true;
}

bool ExploreState::onExit(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Exiting explore state" << std::endl;
    }

    mazeLoaded = false;

    return true;
}

