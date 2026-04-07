#include "GameScene.hpp"
#include "Engine/Engine.hpp"
#include <algorithm>
#include <random>

static constexpr int CARD_W    = 80;
static constexpr int CARD_H    = 80;
static constexpr int PADDING   = 15;
static constexpr int GRID      = BOARD_SIZE;
static constexpr int NUM_CARDS = GRID * GRID;

GameScene::GameScene(const std::string& hiddenPath, const std::string& revealDir)
: mBoardState(BoardState::IDLE)
{
    createBoard(hiddenPath, revealDir);
}

void GameScene::createBoard(const std::string& hiddenPath, const std::string& revealDir)
{
    // --- shuffle pair indices ---
    std::vector<int> indices;
    for (int i = 1; i <= NUM_CARDS / 2; i++) {
        indices.push_back(i);
        indices.push_back(i);
    }
    std::mt19937 rng(std::random_device{}());
    std::shuffle(indices.begin(), indices.end(), rng);

    // --- grid layout ---
    int vpW, vpH;
    SDL_GetCurrentRenderOutputSize(TellyEngine::getInstance()->PTRgetRenderer(), &vpW, &vpH);
    int totalW  = GRID * CARD_W + (GRID - 1) * PADDING;
    int totalH  = GRID * CARD_H + (GRID - 1) * PADDING;
    int startX  = (vpW - totalW) / 2;
    int startY  = (vpH - totalH) / 2;

    mCards.resize(NUM_CARDS);
    for (int i = 0; i < NUM_CARDS; i++)
    {
        auto& c = mCards[i];

        // position
        int row = i / GRID;
        int col = i % GRID;
        float x = static_cast<float>(startX + col * (CARD_W + PADDING));
        float y = static_cast<float>(startY + row * (CARD_H + PADDING));

        c.entity   = std::make_unique<Telly::Entity>(Vec2f{ x, y });
        c.hidden   = std::make_shared<Telly::Sprite>(hiddenPath);
        c.reveal   = std::make_shared<Telly::Sprite>(revealDir + "/" + std::to_string(indices[i]) + ".png");
        c.pair_idx = indices[i];
    }
}

void GameScene::render()
{
    for (auto& c : mCards)
    {
        Telly::Sprite* s = (c.clicked || c.matched)
                           ? c.reveal.get()
                           : c.hidden.get();
        Vec4f spriteDimensions { c.entity->getPos().x, c.entity->getPos().y, CARD_W, CARD_H };
        s->render(spriteDimensions);
    }
}

void GameScene::handle_event(SDL_Event& e)
{
    if (mInputLocked) return;

    SDL_FPoint cursor { e.motion.x, e.motion.y };

    for (int i = 0; i < (int)mCards.size(); i++)
    {
        auto& c = mCards[i];

        SDL_FRect bounds {
            c.entity->getPos().x, c.entity->getPos().y,
            (float)CARD_W,    (float)CARD_H
        };

        c.hovered = SDL_PointInRectFloat(&cursor, &bounds);

        // hover tint — skip already revealed cards
        if (!c.clicked && !c.matched)
            c.hidden->setAlpha(c.hovered ? 65 : 255);

        if (e.type != SDL_EVENT_MOUSE_BUTTON_DOWN) continue;
        if (!c.hovered || c.clicked || c.matched)  continue;

        c.clicked = true;

        switch (mBoardState)
        {
            case BoardState::IDLE:
                mFirstPick  = i;
                mBoardState = BoardState::SINGLE;
                break;

            case BoardState::SINGLE:
                mSecondPick = i;
                mBoardState = BoardState::DOUBLE;

                if (mCards[mFirstPick].pair_idx == mCards[mSecondPick].pair_idx)
                {
                    mCards[mFirstPick].matched = true;
                    mCards[mSecondPick].matched = true;
                    mFirstPick  = -1;
                    mSecondPick = -1;
                    mBoardState = BoardState::IDLE;
                    checkWin();
                }
                else
                {
                    mInputLocked = true;
                    mFlipTimer.start();
                }
                break;

            case BoardState::DOUBLE:
                break;
        }
    }
}

void GameScene::update(float dt)
{
    if (mBoardState == BoardState::DOUBLE && mInputLocked)
    {
        if (mFlipTimer.getNS() >= 1'000'000'000ULL)
        {
            mFlipTimer.stop();

            auto& f = mCards[mFirstPick];
            auto& s = mCards[mSecondPick];

            f.clicked = false;  s.clicked = false;
            f.hidden->setAlpha(255);
            s.hidden->setAlpha(255);

            mFirstPick  = -1;
            mSecondPick = -1;
            mInputLocked = false;
            mBoardState  = BoardState::IDLE;
        }
    }
}

void GameScene::checkWin()
{
    // bool allMatched = std::all_of(mCards.begin(), mCards.end(),
    //                               [](const Card& c){ return c.matched; });
    // if (allMatched)
        // SceneManager::getInstance()->switchScene("MenuScene");
}
