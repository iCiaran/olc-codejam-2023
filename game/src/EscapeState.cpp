#include "EscapeState.h"

#if defined(OLC_PLATFORM_EMSCRIPTEN)
#include <emscripten/fetch.h>

void onError(struct emscripten_fetch_t *fetch) {
    emscripten_fetch_close(fetch);
}

void onSuccess(struct emscripten_fetch_t *fetch) {

    auto state = static_cast<EscapeState*>(fetch->userData);
    std::string responseBody(fetch->data, fetch->numBytes);
    state->globals->httpResponse = responseBody;

    emscripten_fetch_close(fetch);
}
#endif

EscapeState::EscapeState(GameGlobals *gameGlobals) : BaseState(gameGlobals) { }

EscapeState::~EscapeState() = default;

GameGlobals::State EscapeState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
    olc::vi2d offset;
    bool move = false;

    if(pge->GetKey(olc::Key::LEFT).bPressed) {
        offset = {-1, 0};
        move = true;
    } else if(pge->GetKey(olc::Key::RIGHT).bPressed) {
        offset = {1, 0};
        move = true;
    } else if(pge->GetKey(olc::Key::UP).bPressed) {
        offset = {0, -1};
        move = true;
    } else if(pge->GetKey(olc::Key::DOWN).bPressed) {
        offset = {0, 1};
        move = true;
    }

    if (move) {
        olc::vi2d nextPosition = playerPosition + offset;

        if(globals->maze->inBounds(nextPosition.x, nextPosition.y) && globals->maze->getCell(nextPosition.x, nextPosition.y)) {
            playerPosition = nextPosition;
            moveCounter++;
        }

        if(playerPosition == olc::vi2d{globals->maze->size - 2, globals->maze->size - 2}) {
            return GameGlobals::State::WIN;
        }
    }

    olc::vi2d screenPosition = playerPosition * globals->cellSize + olc::vi2d{globals->cellSize / 2, globals->cellSize / 2};

    pge->Clear(olc::BLANK);
    globals->maze->drawAroundPoint(pge, globals->cellSize, screenPosition, globals->minRadius);

    float scale = static_cast<float>(globals->minRadius) / 128.0f;
    pge->DrawDecal({static_cast<float>(screenPosition.x - globals->minRadius) - 64 * scale, static_cast<float>(screenPosition.y - globals->minRadius) - 64 * scale}, globals->circleMask.Decal(), {scale, scale}, olc::BLACK);

    pge->FillCircle(screenPosition, globals->cellSize / 4, olc::DARK_GREEN);

    pge->DrawStringDecal({10,10}, "State: ESCAPE", olc::RED);
    pge->DrawStringDecal({160,10}, "Moves: " + std::to_string(moveCounter), olc::RED);
    pge->DrawStringDecal({310,10}, "Player: " + playerPosition.str(), olc::RED);

    return GameGlobals::State::ESCAPE;
}

bool EscapeState::onEnter(olc::PixelGameEngine *pge) {
    std::cout << "Entering escape state" << std::endl;

    playerPosition = {1, 1};
    moveCounter = 0;
    return true;
}

bool EscapeState::onExit(olc::PixelGameEngine *pge) {
    std::cout << "Exiting escape state" << std::endl;

    globals->movesTaken = moveCounter;

#if defined(OLC_PLATFORM_EMSCRIPTEN)
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onSuccess;
    attr.onerror = onError;
    attr.userData = this;
    emscripten_fetch(&attr, "http://localhost:5000/test.txt");
#endif

    return true;
}
