#include "Engine/Engine.hpp"
#include "GameScene.hpp"
#include "headers/MenuScene.hpp"
#include <memory>

// TODO:
// Better Sprite System.
// GameScene switching

int main(void)
{
    TellyEngine* engine = TellyEngine::getInstance();

    engine->setWindowSize(Vec2f{800.f, 600.f});
    engine->setViewport(Vec2f{800.f, 600.f});
    engine->initialize();
    std::unique_ptr<MenuScene> menu = std::make_unique<MenuScene>();
    engine->PTRgetSceneManager()->loadScene(std::move(menu));

    std::unique_ptr<GameScene> game = std::make_unique<GameScene>("Image/qmark.png", "Image");
    engine->PTRgetSceneManager()->loadScene(std::move(game));

    engine->run();

    return 0;
}
