
#include "ScoreState.h"

#if defined(OLC_PLATFORM_EMSCRIPTEN)
#include <emscripten/fetch.h>
#include "json.h"
using json = nlohmann::json;

void onListScoresError(struct emscripten_fetch_t *fetch) {
    emscripten_fetch_close(fetch);
}

void onListScoresSuccess(struct emscripten_fetch_t *fetch) {
    auto state = static_cast<ScoreState*>(fetch->userData);
    std::string responseBody(fetch->data, fetch->numBytes);
    json response = json::parse(responseBody);

    for (auto p : state->playButtons) {
        delete p;
    }
    for (auto p : state->idLabels) {
        delete p;
    }
    for (auto p : state->nameLabels) {
        delete p;
    }
    for (auto p : state->scoreLabels) {
        delete p;
    }

    state->scores.clear();
    state->playButtons.clear();
    state->idLabels.clear();
    state->nameLabels.clear();
    state->scoreLabels.clear();

    state->scoreDisplay = olc::QuickGUI::Manager();
    int offset = 0;
    for(auto& jScore : response) {
        Score score;
        score.maze = jScore["maze"].get<int>();
        score.name = jScore["name"].get<std::string>();
        score.score = jScore["score"].get<int>();
        state->scores.push_back(score);

        state->idLabels.push_back(new olc::QuickGUI::Label(state->scoreDisplay, std::to_string(score.maze), {200, static_cast<float>( 200 + 40*offset)}, {80, 30}));
        state->nameLabels.push_back(new olc::QuickGUI::Label(state->scoreDisplay, score.name, {300, static_cast<float>( 200 + 40*offset)}, {80, 30}));
        state->scoreLabels.push_back(new olc::QuickGUI::Label(state->scoreDisplay, std::to_string(score.score), {400, static_cast<float>( 200 + 40*offset)}, {80, 30}));
        state->playButtons.push_back(new olc::QuickGUI::Button(state->scoreDisplay, "Play", {500, static_cast<float>( 200 + 40*offset)}, {80, 30}));
        offset += 1;
    }
    state->fetching = false;

    emscripten_fetch_close(fetch);
}
#endif

ScoreState::ScoreState(GameGlobals *gameGlobals) : BaseState(gameGlobals), timer(0.0f) {
    scoreTitle = new olc::QuickGUI::Label(staticDisplay, "Score", {400, 150}, {80, 30});
    nameTitle = new olc::QuickGUI::Label(staticDisplay, "Username", {300, 150}, {80, 30});
    idTitle = new olc::QuickGUI::Label(staticDisplay, "Maze", {200, 150}, {80, 30});
    enterMaze = new olc::QuickGUI::TextBox(staticDisplay, "", {400, 650}, {80, 30});
    mazeButton = new olc::QuickGUI::Button(staticDisplay, "Maze", {500, 650}, {80, 30});
    backButton = new olc::QuickGUI::Button(staticDisplay, "Back", {100, 650}, {80, 30});
}

ScoreState::~ScoreState() = default;

GameGlobals::State ScoreState::onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) {
#if defined(OLC_PLATFORM_EMSCRIPTEN)
    pge->Clear(olc::BLANK);
    timer += fElapsedTime;
    if(timer > 30.0f) {
        timer -= 30.0f;
        if(!fetching){
            updateScores();
            return GameGlobals::State::SCORES;
        }
    }

    if(globals->debugText) {
        pge->DrawStringDecal({10, 10}, "State: SCORES", olc::RED);
        pge->DrawStringDecal({160, 10}, "Time: " + std::to_string(timer), olc::RED);
        pge->DrawStringDecal({310, 10}, "Fetching: " + std::to_string(fetching), olc::RED);
    }

    if(backButton->bPressed) {
         return GameGlobals::State::MENU;
    }

    if(fetching){
        const std::string playString = "Updating scores";
        const olc::vi2d textSize = pge->GetTextSize(playString);
        pge->DrawStringDecal((pge->GetScreenSize() - textSize) / 2, playString);
        return GameGlobals::State::SCORES;
    } else {
        scoreDisplay.Update(pge);
        scoreDisplay.Draw(pge);
        staticDisplay.Update(pge);
        staticDisplay.Draw(pge);

        pge->DrawLine({200, 190}, {500, 190}, olc::WHITE);

        for(int i = 0; i < playButtons.size(); i++) {
            if(playButtons[i]->bPressed){
                playButtons[i]->bPressed = false;
                globals->seed = scores[i].maze;
                return GameGlobals::State::EXPLORE;
            }
        }

        enterMaze->sText.erase(std::remove_if(enterMaze->sText.begin(), enterMaze->sText.end(),
                           []( auto const& c ) -> bool { return !std::isdigit(c); } ), enterMaze->sText.end());
        enterMaze->sText = enterMaze->sText.substr(0, 4);

        if(mazeButton->bPressed) {
            mazeId = enterMaze->sText;
            updateScores();
            return GameGlobals::State::SCORES;
        }
    }

    return GameGlobals::State::SCORES;

#endif
    return GameGlobals::State::MENU;
}

bool ScoreState::onEnter(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Entering score state" << std::endl;
    }
    fetching = false;
    mazeId = "";
#if defined(OLC_PLATFORM_EMSCRIPTEN)
    updateScores();
#endif
    return true;
}

bool ScoreState::onExit(olc::PixelGameEngine *pge) {
    if(globals->debugText) {
        std::cout << "Exiting score state" << std::endl;
    }
    backButton->bPressed = false;

    return true;
}

void ScoreState::updateScores() {
#if defined(OLC_PLATFORM_EMSCRIPTEN)
    fetching = true;
    std::string url = mazeId.empty() ? "http://localhost:5000/api/scores" : "http://localhost:5000/api/scores/" + mazeId;
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onListScoresSuccess;
    attr.onerror = onListScoresError;
    attr.userData = this;
    emscripten_fetch(&attr, url.c_str());
#endif
}
