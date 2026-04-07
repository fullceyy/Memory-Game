#pragma once
#include "Entity/Entity.hpp"
#include "Scene/IScene.hpp"
#include "Sprite/Sprite.hpp"
#include "Timer/Timer.hpp"
#include <memory>
#include <vector>
#include <string>

#define BOARD_SIZE 4

class GameScene : public Telly::IScene
{
private:
    enum class BoardState { IDLE, SINGLE, DOUBLE };

    struct Card {
        std::unique_ptr<Telly::Entity> entity;
        std::shared_ptr<Telly::Sprite> hidden;
        std::shared_ptr<Telly::Sprite> reveal;
        int  pair_idx = -1;
        bool clicked = false;
        bool matched = false;
        bool hovered = false;
    };

    std::vector<Card>   mCards;
    BoardState mBoardState;
    bool mInputLocked = false;
    int mFirstPick  = -1;
    int mSecondPick = -1;
    Timer mFlipTimer;

    void createBoard(const std::string& hiddenPath, const std::string& revealDir);
    void checkWin();

public:
    GameScene(const std::string& hiddenPath, const std::string& revealDir);
    ~GameScene() = default;

    void update(float dt) override;
    void render() override;
    void handle_event(SDL_Event& e) override;
};
