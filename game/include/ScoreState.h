#ifndef OLCJAM2023_SCORESTATE_H
#define OLCJAM2023_SCORESTATE_H

#include "BaseState.h"
#include "olcPGEX_QuickGUI.h"

struct Score {
    std::string name;
    int score;
    int maze;
};

class ScoreState : public BaseState {
public:
    explicit ScoreState(GameGlobals *gameGlobals);
    ~ScoreState() override;

    std::vector<Score> scores;
    float timer;
    bool fetching = false;

    std::string mazeId;
    olc::QuickGUI::Manager scoreDisplay;
    olc::QuickGUI::Manager staticDisplay;
    std::vector<olc::QuickGUI::Label*> idLabels;
    std::vector<olc::QuickGUI::Label*> nameLabels;
    std::vector<olc::QuickGUI::Label*> scoreLabels;
    std::vector<olc::QuickGUI::Button*> playButtons;

    olc::QuickGUI::Label* scoreTitle;
    olc::QuickGUI::Label* nameTitle;
    olc::QuickGUI::Label* idTitle;
    olc::QuickGUI::TextBox* enterMaze;
    olc::QuickGUI::Button* mazeButton;
    olc::QuickGUI::Button* backButton;

    GameGlobals::State onUpdate(olc::PixelGameEngine *pge, float fElapsedTime) override;
    bool onEnter(olc::PixelGameEngine *pge) override;
    bool onExit(olc::PixelGameEngine *pge) override;

    void updateScores();
};



#endif //OLCJAM2023_SCORESTATE_H
